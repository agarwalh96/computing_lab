#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define DATA int

typedef struct tnode 
{
	DATA d;
	int left, right;
	int level;
	int parent; //optional
}TNODE;

typedef struct {
	int root,free;
	int capacity;
	TNODE *nodelist;
} TREE;


void init_tree(TREE *t, int capacity)
{
	t->root = -1;
	t->free = 0;
	t->capacity = capacity;	
	if(NULL == (t->nodelist = (TNODE *) malloc(t->capacity * sizeof(TNODE)))) 
	{
		fprintf(stderr,"Outofmemory\n");
		exit(1);
	}
	for(int i=0;i < t->capacity;i++)
	{
		t->nodelist[i].left = i+1;
		t->nodelist[i].right = -1;
		t->nodelist[i].parent = -1;
	}
	t->nodelist[t->capacity - 1].left = -1;
}

void read_from_file(TREE *t)
{
	FILE *fp;
	fp = fopen("input0.txt","r");
	int numNodes;
	fscanf(fp,"%u", &numNodes);
	/*
	if(NULL == (t->nodelist = (TNODE *) malloc(t->capacity * sizeof(TNODE)))) 
	{
		fprintf(stderr,"Outofmemory\n");
		exit(1);
	}
	*/
	TNODE *node;
	int i;
	for(node = t->nodelist, i = 0; i < numNodes; node++, i++)
	{
		fscanf(fp,"%d%d%d", &(node->d), &(node->left), &(node->right));
		t->nodelist[node->left].parent = i;
		t->nodelist[node->right].parent = i;	
	}
	
	t->root =0 ;
	t->free = numNodes;
	
	fclose(fp);
}

void store_level(TREE *t, int root, int curr_level)
{
	if(root == -1)
		return;
	t->nodelist[root].level = curr_level;
	store_level(t,t->nodelist[root].left,1+curr_level);
	store_level(t,t->nodelist[root].right,1+curr_level);
	
}

int traverse_to_parent(TREE *t, int idx, int count)
{
	if (idx==-1 || count ==0)
		return idx;
	//printf("%d ",t->nodelist[idx].d);
	return traverse_to_parent(t, t->nodelist[idx].parent, --count);
	
}

int common_ancestor(TREE *t, int idx_1, int idx_2)
{
	if(idx_1 ==-1 && idx_2 ==-1)
		return -1;
	printf("%d %d \n",t->nodelist[idx_1].d,t->nodelist[idx_2].d);
	if(idx_1 == idx_2)
		return idx_1;
	return 	common_ancestor(t, t->nodelist[idx_1].parent, t->nodelist[idx_2].parent);
	
}


void swap(int *a,int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

int main()
{
	TREE t;

	init_tree(&t, 10);
	
	read_from_file(&t);
	
	store_level(&t,t.root,1);
	
	//traverse_to_parent(&t,4,12);
	
	int idx_1=5,idx_2=2;
	scanf("%d %d",&idx_1,&idx_2);
	
	int lvl_1 = t.nodelist[idx_1].level, lvl_2 = t.nodelist[idx_2].level;
	
	
	if(lvl_1 < lvl_2)
	{
		swap(&idx_1,&idx_2);
		swap(&lvl_1,&lvl_2);
	}
	
	printf("parent at same level : %d\n",t.nodelist[traverse_to_parent(&t,idx_1,(lvl_1-lvl_2))].d);
	
	int	lca_idx = common_ancestor(&t,traverse_to_parent(&t,idx_1,(lvl_1-lvl_2)),idx_2);
	
	printf("LCA is : %d\n",t.nodelist[lca_idx].d);
	

	
	return 0;
	
}


















