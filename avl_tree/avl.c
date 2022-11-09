#include "common.h"
#include <time.h>

typedef int DATA;
typedef struct node 
{
	DATA data;
	int left, right, parent, height;
	
} AVL_NODE;

typedef struct
{
	unsigned int num_nodes, max_nodes;
	int root, free_list;
	AVL_NODE *nodelist;
} TREE;

#define HEIGHT(T, nodeindex) ( ((nodeindex) == -1) ? -1 :T->nodelist[nodeindex].height )


void init_tree(TREE *tree, int N)
{
	tree->num_nodes = 0;
    tree->max_nodes = N;
    tree->root = -1;
    tree->free_list = 0;
    if (NULL == (tree->nodelist = Malloc(N, AVL_NODE)))
        ERR_MESG("bst: out of memory");
	
    for (int i = 0; i < N; i++)
        tree->nodelist[i].left = i+1;
    tree->nodelist[N-1].left = -1;
}


int find_successor(TREE *tree,int node) 
{
	assert(node!= -1);
	int child;
	child = tree->nodelist[node].right;
	
	if(child == -1)
		return -1;
	if(tree->nodelist[child].left == -1)
		return child;
	while(tree->nodelist[child].left != -1)
	{
		node = child;
		child = tree->nodelist[child].left;
	}
	return child;
}

int max(int a, int b)
{
	if (a>b)
		return a;
	return b;
}

// /* ---------------  Insert func from BST--------------------------
void insert(TREE *tree, int *rootptr, int d) {
    int root = *rootptr, next;
	
    if (root == -1) {
    	//printf("inserting %d\n",d);
        assert(tree->num_nodes < tree->max_nodes);
        next = tree->free_list;
        tree->free_list = tree->nodelist[tree->free_list].left;
        tree->num_nodes++;
        tree->nodelist[next].data = d;
        tree->nodelist[next].left = tree->nodelist[next].right = -1;
        tree->nodelist[next].height = 1;
        *rootptr = next;
        return;
    }

    if (d < tree->nodelist[root].data)
    {
        insert(tree, &(tree->nodelist[root].left), d);    
    }
    else if (d > tree->nodelist[root].data)
    {
        insert(tree, &(tree->nodelist[root].right), d);
    }
    
    int new_height = max(HEIGHT(tree, tree->nodelist[root].left), HEIGHT(tree, tree->nodelist[root].right)) + 1;
    tree->nodelist[root].height = new_height;
    
    return;
}
// */ --------------------------------------------------------------

void rotate_on_insert_LL(TREE *tree,int parent,int *node) 
{/*SeeWeiss,DS&AAinC++,4ed.,Section4.4.1,Figure4.34*/
#ifdef DEBUG 
	printf("LL(right)rotationat%d\n",
	tree->nodelist[*node].data);
#endif //DEBUG
	int top = *node;
	int top_l = tree->nodelist[k2].left;
	int top_r = tree->nodelist[k2].right;
	int top_ll = tree->nodelist[top_l].left;
	int top_lr = tree->nodelist[top_l].right;
	/*rotate*/
	tree->nodelist[k2].left = Y;
	tree->nodelist[k1].right = k2;
	/*parents(optional)*/
	tree->nodelist[k1].parent = parent;
	
	tree->nodelist[k2].parent = k1;
	if(Y != -1) tree->nodelist[Y].parent = k2;
	/*updateheights*/
	tree->nodelist[k2].height = 1 + MAX(HEIGHT(tree, Y), HEIGHT(tree,Z));
	tree->nodelist[k1].height = 1 + MAX(HEIGHT(tree, X), HEIGHT(tree,k2));
	*node = k1;
	return;
}


int main(int ac, char *av[])
{
    int N, i;
    AVL_NODE *nodeptr;
    TREE tree;

    if (ac != 2)
        ERR_MESG("Usage: bst <number>");
    N = atoi(av[1]); // max. number of nodes
    
    init_tree(&tree, N);
    
    //srandom((int) time(NULL));
    for (i = 0; i < N; i++)
    {
        insert(&tree, &(tree.root), rand() % 1000);
        //insert(&tree, &(tree.root), 10-i);
        //printf("free_list index : %d\n",tree.free_list);
    }

    //print_pstree(&tree, tree.root);
    printf("%u\n", tree.num_nodes);
    for (i = 0, nodeptr = tree.nodelist; i < tree.num_nodes; i++, nodeptr++)
        printf("%d %d %d %d %d\n", i, nodeptr->data, nodeptr->left, nodeptr->right, nodeptr->height);
   
}
	
