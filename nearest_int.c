#include "common.h"

typedef struct node {
    int data;
    int left, right;
} NODE;

typedef struct {
    unsigned int num_nodes, max_nodes;
    int root, free_list; 
    NODE *nodelist;
} TREE;


void init_tree(TREE *tree, int N)
{
	tree->num_nodes = 0;
    tree->max_nodes = N;
    tree->root = -1;
    tree->free_list = 0;
    if (NULL == (tree->nodelist = Malloc(N, NODE)))
        ERR_MESG("bst: out of memory");
	
    for (int i = 0; i < N; i++)
        tree->nodelist[i].left = i+1;
    tree->nodelist[N-1].left = -1;
}

void insert(TREE *tree, int *rootptr, int d) {
    int root = *rootptr, next;

    if (root == -1) {
        assert(tree->num_nodes < tree->max_nodes);
        next = tree->free_list;
        tree->free_list = tree->nodelist[tree->free_list].left;
        tree->num_nodes++;
        tree->nodelist[next].data = d;
        tree->nodelist[next].left = tree->nodelist[next].right = -1;
        *rootptr = next;
        return;
    }

    if (d < tree->nodelist[root].data)
        insert(tree, &(tree->nodelist[root].left), d);
    else if (d > tree->nodelist[root].data)
        insert(tree, &(tree->nodelist[root].right), d);
    return;
}

int abs_diff(int a, int b)
{
	return abs(a-b);
}

int nearest_search(TREE *tree, int root, int target, int nearest)
{
	if(root == -1)
		return -1;
	
	if(tree->nodelist[root].data == target)
		return root;
	
	if(target < tree->nodelist[root].data)
	{
		if(tree->nodelist[root].left == -1)
			return root;
		else
		{
			int temp = nearest_search(tree,tree->nodelist[root].left , target, nearest);
			if(abs_diff(tree->nodelist[root].data, target) < abs_diff(tree->nodelist[temp].data, target))
				return root;
			else
				return temp;
		}
	}
	else
	{
		if(tree->nodelist[root].right == -1)
			return root;
		else
		{
			int temp = nearest_search(tree,tree->nodelist[root].right , target, nearest);
			if(abs_diff(tree->nodelist[root].data, target) < abs_diff(tree->nodelist[temp].data, target))
				return root;
			else
				return temp;
		}
	}
	
	
}

int main(int ac, char *av[])
{
    int N, i;
    NODE *nodeptr;
    TREE tree;

    if (ac != 2)
        ERR_MESG("Usage: bst <number>");
    N = atoi(av[1]); // max. number of nodes
    
    init_tree(&tree, N);
    
    //srandom((int) time(NULL));
    for (i = 0; i < N; i++)
    {
        insert(&tree, &(tree.root), random() % 1000);
        //printf("free_list index : %d\n",tree.free_list);
    }

    //print_pstree(&tree, tree.root);
    printf("%u\n", tree.num_nodes);
    for (i = 0, nodeptr = tree.nodelist; i < tree.num_nodes; i++, nodeptr++)
        printf("%d %d %d %d\n", i, nodeptr->data, nodeptr->left, nodeptr->right);
   
   int target=0;
   scanf("%d",&target);
   int nearest_idx = nearest_search(&tree, tree.root, target, 0);
   printf("nearest int found is :%d\n",tree.nodelist[nearest_idx].data);
    
}
    
    
    
