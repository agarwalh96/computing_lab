#include "common.h"

typedef struct {
    int data;
    int left, right, level, parent, deleted;
} TNODE;

typedef struct {
    int root, num_nodes;
    TNODE *nodelist;
} TREE;

void print_tree(TREE *t, char *filename)
{
    FILE *fp;
    TNODE *node;

    if (NULL == (fp = fopen(filename, "w")))
        ERR_MESG("print_tree: error opening output file");
    for (node = t->nodelist; node - t->nodelist < t->num_nodes; node++)
    {
		if(node->deleted == 1)
			continue;
		fprintf(fp, "%ld %d %d %d %d\n",node - t->nodelist, node->data, node->left, node->right,
                node->level);
	}
    fclose(fp);
    return;
}

void init_tree(TREE* t,int k)
{
	t->root =0;
	t->num_nodes = (1 << k+1) - 1;
	t->nodelist = (TNODE *) calloc( t->num_nodes, sizeof(TNODE));
	if(NULL == t->nodelist)
		exit(1);
	
	int lev=1;
	t->nodelist[0].parent = -1;
	for(int i = 0;i<t->num_nodes; i++)
	{
		if(i> (1 << lev)-2)
			lev++;
		//printf("i,l : %d,%d\n",i,lev);
		t->nodelist[i].data = lev;
		t->nodelist[i].level = lev;
		if(lev<=k)
		{
			t->nodelist[i].left = i*2 + 1;
			t->nodelist[i].right = i*2 + 2;
			
			t->nodelist[t->nodelist[i].left].parent = i;
			t->nodelist[t->nodelist[i].right].parent = i;
		}
		else
		{
			t->nodelist[i].left = -1;
			t->nodelist[i].right = -1;
		}
	}
}

void read_truth_table(TREE* t,int k, FILE *fp)
{
	
	for(int i=0;i<(1<<k); i++)
	{
		int node_idx = 0;
		int bit_read;
		for(int j=0;j<k;j++)
		{
			
			fscanf(fp,"%d",&bit_read);
			if(bit_read == 0)
			{
				node_idx = t->nodelist[node_idx].left;
			}
			else
			{
				node_idx = t->nodelist[node_idx].right;
			}
			
		}
		fscanf(fp,"%d",&bit_read);
		//printf("node at idx %d will have value %d\n",node_idx,bit_read);
		t->nodelist[node_idx].data = bit_read;
	}
}

TREE remove_duplicate_leaves(TREE* t,int k)
{
	TREE t_out;
	t_out.root = 2;
	t_out.num_nodes = (1<<k)+1; 
	t_out.nodelist = (TNODE *) calloc( t_out.num_nodes, sizeof(TNODE));
	if(NULL == t_out.nodelist)
		exit(1);
	//Initialize the two leaf nodes
	t_out.nodelist[0].data = 0;
    t_out.nodelist[0].left = -1;
    t_out.nodelist[0].right = -1;
    t_out.nodelist[0].level = k+1;
	t_out.nodelist[0].deleted = 0;
    t_out.nodelist[1].data = 1;
    t_out.nodelist[1].left = -1;
    t_out.nodelist[1].right = -1;
    t_out.nodelist[1].level = k+1;
	t_out.nodelist[0].deleted = 0;
	//-----------------------------
	
	for(int i=0;i < t_out.num_nodes-2; i++)
	{
		memcpy((void *)(t_out.nodelist+i+2), (void *)(t->nodelist+i), sizeof(TNODE));
		t_out.nodelist[i+2].left+=2;
		t_out.nodelist[i+2].right+=2;
		t_out.nodelist[i+2].parent+=2;
		t_out.nodelist[i+2].deleted = 0;
		
		if(t_out.nodelist[i+2].level == k)
		{
			if(t->nodelist[t->nodelist[i].left].data == 0)
				t_out.nodelist[i+2].left = 0;
			else
				t_out.nodelist[i+2].left = 1;
			
			if(t->nodelist[t->nodelist[i].right].data == 0)
				t_out.nodelist[i+2].right = 0;
			else
				t_out.nodelist[i+2].right = 1;
		}
	}
	
	return t_out;
}

int reduction_rule2(TREE* t,int root, int *modified_flag)
{
	if(root == 0 || root == 1)
		return root;
	
	t->nodelist[root].left = reduction_rule2(t,t->nodelist[root].left,modified_flag);
	t->nodelist[root].right = reduction_rule2(t,t->nodelist[root].right,modified_flag);
	
	
	
	if(t->nodelist[root].left == t->nodelist[root].right)
	{
		t->nodelist[root].deleted = 1;
		*modified_flag = 1;
		//printf("rule 2 on node idx %d\n",root);
		return t->nodelist[root].left;
	}
	else
	{
		return root;
	}
}

int is_isomorphic(TREE* t, int idx1, int idx2)
{
	if(t->nodelist[idx1].left == t->nodelist[idx2].left && t->nodelist[idx1].right == t->nodelist[idx2].right)
		return 1;
	else
		return 0;
}

int reduction_rule3(TREE* t,int root, int num_vars, int *modified_flag)
{
	for(int i=1;i<=num_vars; i++)
	{
		int lower = (1 << (i-1)) + 1;
		int upper = (1 << i);
		
		for(;lower<upper;lower++)
		{
			if(1 == t->nodelist[lower].deleted)
				continue;
			
			for(int temp = lower+1; temp<=upper; temp++)
			{
				if(1 == t->nodelist[temp].deleted)
					continue;
				if(is_isomorphic(t, lower, temp))
				{
					//printf("isomorphic found at %d and %d\n",lower, temp);
					//printf("parent idx is %d \n",par_idx);
					int par_idx = t->nodelist[temp].parent;
					if(t->nodelist[par_idx].left == temp)
						t->nodelist[par_idx].left = lower;
					else if(t->nodelist[par_idx].right == temp)
						t->nodelist[par_idx].right = lower;
					
					t->nodelist[temp].deleted = 1;
					*modified_flag = 1;
				}
			}
		}
		
	}
}


int main(int c, char** argv)
{
	TREE t;
	FILE *fp;
    if (NULL == (fp = fopen(argv[1], "r")))
        ERR_MESG("print_tree: error opening output file");
	
	int num_vars;
	fscanf(fp,"%d",&num_vars);
	
	init_tree(&t,num_vars);
	read_truth_table(&t,num_vars,fp);
	fclose(fp);
	
	//Tree after reading input
	//print_tree(&t, argv[2]);

	TREE t_out = remove_duplicate_leaves(&t,num_vars);
	//print reduced tree after Rule C1
	//print_tree(&t_out, argv[3]);
	
	//Repeatedly apply C2 & C3
	int modified_flag = 1;
	while(modified_flag)
	{
		modified_flag = 0;
		t_out.root = reduction_rule2(&t_out,t_out.root, &modified_flag);
		//print_tree(&t_out,"rule2.txt");
		
		reduction_rule3(&t_out,t_out.root, num_vars, &modified_flag);
		//print_tree(&t_out,"rule3.txt");
	}
	
	
	printf("Final reduced tree root is at %d\n",t_out.root);
	//print reduced tree after Rule C2 & C3 applied iteratively
	print_tree(&t_out,"reduced_BDD.txt");
	for (TNODE *node = t_out.nodelist; node - t_out.nodelist < t_out.num_nodes; node++)
    {
		if(node->deleted == 1)
			continue;
		printf("%ld %d %d %d %d\n",node - t_out.nodelist, node->data, node->left, node->right, node->level);
	}
	
	free(t.nodelist);
	free(t_out.nodelist);
	return 0;
}
