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


void print_pstree(TREE *tree)
{
	int i;
    AVL_NODE *nodeptr;
	//print_pstree(&tree, tree.root);
    printf("Printing tree with numnode : %u\n", tree->num_nodes);
    for (i = 0, nodeptr = tree->nodelist; i < tree->num_nodes; i++, nodeptr++)
        printf("%d %d %d %d %d \n", i, nodeptr->data, nodeptr->left, nodeptr->right, nodeptr->height);
	
}

void print_preorder(TREE *tree, int root)
{
	if(root == -1)
		return;
	printf("%d ",tree->nodelist[root].data);
	print_preorder(tree, tree->nodelist[root].left);
	print_preorder(tree, tree->nodelist[root].right);
}

void print_inorder(TREE *tree, int root)
{
	if(root == -1)
		return;
	print_inorder(tree, tree->nodelist[root].left);
	printf("%d ",tree->nodelist[root].data);
	print_inorder(tree, tree->nodelist[root].right);
	
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


void rotate_on_insert_LL(TREE *tree,int parent,int *node) 
{/*SeeWeiss,DS&AAinC++,4ed.,Section4.4.1,Figure4.34*/
#ifdef DEBUG 
	printf("LL(right)rotation at %d\n",
	tree->nodelist[*node].data);
#endif //DEBUG
	int k2 = *node;
	int k1 = tree->nodelist[k2].left;
	int Z = tree->nodelist[k2].right;
	int X = tree->nodelist[k1].left;
	int Y = tree->nodelist[k1].right;
	/*rotate*/
	tree->nodelist[k2].left = Y;
	tree->nodelist[k1].right = k2;
	/*parents(optional)
	tree->nodelist[k1].parent = parent;
	tree->nodelist[k2].parent = k1;
	if(Y != -1) 
		tree->nodelist[Y].parent = k2;
	*/
	
	/*updateheights*/
	tree->nodelist[k2].height = 1 + MAX(HEIGHT(tree, Y), HEIGHT(tree,Z));
	tree->nodelist[k1].height = 1 + MAX(HEIGHT(tree, X), HEIGHT(tree,k2));
	*node = k1;
	return;
}

void rotate_on_insert_RR(TREE *tree,int parent,int *node) 
{/*SeeWeiss,DS&AAinC++,4ed.,Section4.4.1,Figure4.34*/
#ifdef DEBUG 
	printf("RR(left)rotation at %d\n",
	tree->nodelist[*node].data);
#endif //DEBUG
	int k2 = *node;
	int k1 = tree->nodelist[k2].right;
	int Z = tree->nodelist[k2].left;
	int X = tree->nodelist[k1].left;
	int Y = tree->nodelist[k1].right;
	/*rotate*/
	tree->nodelist[k2].right = X;
	tree->nodelist[k1].left = k2;
	/*parents(optional)
	tree->nodelist[k1].parent = parent;
	tree->nodelist[k2].parent = k1;
	if(X != -1) 
		tree->nodelist[X].parent = k2;
	*/
	
	/*updateheights*/
	tree->nodelist[k2].height = 1 + MAX(HEIGHT(tree, X), HEIGHT(tree,Z));
	tree->nodelist[k1].height = 1 + MAX(HEIGHT(tree, Y), HEIGHT(tree,k2));
	*node = k1;
	return;
}

void rotate_on_insert_LR(TREE *tree,int parent,int *node)
{
#ifdef DEBUG 
	printf("LR(double)rotation at %d\n",
	tree->nodelist[*node].data);
#endif //DEBUG	
	int k1 = *node;
	rotate_on_insert_RR(tree, k1, &(tree->nodelist[k1].left));
	//print_inorder(tree, tree->root);
	//print_pstree(tree);
	rotate_on_insert_LL(tree, parent, node);
	
	return;
}


void rotate_on_insert_RL(TREE *tree,int parent,int *node)
{
#ifdef DEBUG 
	printf("RL(double)rotation at %d\n",
	tree->nodelist[*node].data);
#endif //DEBUG	
	int k1 = *node;
	rotate_on_insert_LL(tree, k1, &(tree->nodelist[k1].right));
	rotate_on_insert_RR(tree, parent, node);
	
	return;
}

void balance(TREE *tree,int parent,int *node) 
{
	int thisnode = *node;
	int left = tree->nodelist[thisnode].left;
	int right = tree->nodelist[thisnode].right;
	
	if(HEIGHT(tree, left) - HEIGHT(tree, right) > 1)
	{
		printf("left subtree too high at %d\n",tree->nodelist[thisnode].data);
		
		if(HEIGHT(tree, tree->nodelist[left].left) >= HEIGHT(tree,tree->nodelist[left].right))
			rotate_on_insert_LL(tree, parent, node);
		else
			rotate_on_insert_LR(tree, parent, node);
	}
	else if(HEIGHT(tree, right) - HEIGHT(tree, left) > 1)
	{
		printf("right subtree too high at %d\n",tree->nodelist[thisnode].data);
		
		if(HEIGHT(tree, tree->nodelist[right].right) >= HEIGHT(tree,tree->nodelist[right].left))
			rotate_on_insert_RR(tree, parent, node);
		else
			rotate_on_insert_RL(tree, parent, node);
	}
	
	thisnode = *node;
	left = tree->nodelist[thisnode].left;
	right = tree->nodelist[thisnode].right;
	tree->nodelist[thisnode].height = 1 + MAX(HEIGHT(tree, left), HEIGHT(tree, right));
	
	return;
}

// /* ---------------  Insert func from BST--------------------------
void insert_avl(TREE *tree, int *rootptr, int d) {
    int root = *rootptr, next;
	
    if (root == -1) {
    	//printf("inserting %d\n",d);
        assert(tree->num_nodes < tree->max_nodes);
        next = tree->free_list;
        tree->free_list = tree->nodelist[tree->free_list].left;
        tree->num_nodes++;
        tree->nodelist[next].data = d;
        tree->nodelist[next].left = tree->nodelist[next].right = -1;
        tree->nodelist[next].height = 0;
        *rootptr = next;
		printf("Inserted in AVL\n");
        return;
    }

    if (d < tree->nodelist[root].data)
    {
        insert_avl(tree, &(tree->nodelist[root].left), d);    
    }
    else if (d > tree->nodelist[root].data)
    {
        insert_avl(tree, &(tree->nodelist[root].right), d);
    }
    else if (d == tree->nodelist[root].data)
    {
        printf("Duplicate in AVL\n");
		return;
    }
    int new_height = max(HEIGHT(tree, tree->nodelist[root].left), HEIGHT(tree, tree->nodelist[root].right)) + 1;
    tree->nodelist[root].height = new_height;
	balance(tree, 0, rootptr);
    
    return;
}
// */ --------------------------------------------------------------


int delete(TREE *tree,int parent,int *root, DATA d) 
{
	int thisnode = *root;
	if(thisnode == -1)
		return 0;
	if(d < tree->nodelist[thisnode].data)
	{
#ifdef DEBUG 
		printf("deleting recursively from left subtrree\n");
#endif //DEBUG	
		if(FAILURE == delete(tree, thisnode,&(tree->nodelist[thisnode].left), d))
			return FAILURE;
	}
	else if(d > tree->nodelist[thisnode].data)
	{
#ifdef DEBUG 
		printf("deleting recursively from right subtrree\n");
#endif //DEBUG	
		if(FAILURE == delete(tree, thisnode,&(tree->nodelist[thisnode].right), d))
			return FAILURE;
	}
	else
	{
		/*delete this node*/
		printf("delete this node\n");
		if(tree->nodelist[thisnode].left != -1 && tree->nodelist[thisnode].right != -1)
		{
			int successor = find_successor(tree, thisnode);
			assert(successor != -1);
			tree->nodelist[thisnode].data = tree->nodelist[successor].data;
#ifdef DEBUG 
			printf("replacing by successor : %d\n",tree->nodelist[successor].data);
#endif		
			if(FAILURE == delete(tree, thisnode,&(tree->nodelist[thisnode].right), tree->nodelist[successor].data))
				return FAILURE;
		}
		else
		{
			if(tree->nodelist[thisnode].left != -1)
			{
				*root = tree->nodelist[thisnode].left;
				//tree->nodelist[*root].parent = parent;
				
			}
			else if(tree->nodelist[thisnode].right != -1)
			{
				*root = tree->nodelist[thisnode].right;
				//tree->nodelist[*root].parent = parent;
				
			}
			else
			{
				*root = -1;
				
			}
			//free_up_node(tree, thisnode);
			tree->num_nodes--;
			return 0;
		}
	}
	int new_height = max(HEIGHT(tree, tree->nodelist[*root].left), HEIGHT(tree, tree->nodelist[*root].right)) + 1;
    tree->nodelist[*root].height = new_height;
	balance(tree, parent, root);
	
	return 0;

}

int search_avl(TREE *tree, int root, int d) 
{
	if(root == -1)
	{
		return 0;
	}
	
	if (d < tree->nodelist[root].data)
    {
        return search_avl(tree, tree->nodelist[root].left, d);    
    }
    else if (d > tree->nodelist[root].data)
    {
        return search_avl(tree, tree->nodelist[root].right, d);
    }
	else
	{
		return 1;
	}
}

/*
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
    for (i = 0; i < 6; i++)
    {	
		int inserting = rand() % 1000;
		printf("inserting %d\n",inserting);
        insert(&tree, &(tree.root),inserting );
        //insert(&tree, &(tree.root), 10-i);
        //printf("free_list index : %d\n",tree.free_list);
    }
	print_pstree(&tree);
	
}
*/	
