#include "common.h"
#include "bst-alt.h"
#include <time.h>

void insert(TREE *tree, int *rootptr, int d) {
    int root = *rootptr, next;

    if (root == -1) {
        assert(tree->num_nodes < tree->max_nodes);
        next = tree->num_nodes++;
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

void inorder(TREE *tree, int root)
{
	if(root == -1)
		return;
	inorder(tree, tree->nodelist[root].left);
	printf("%d %d %d %d \n",root, tree->nodelist[root].data,tree->nodelist[root].left, tree->nodelist[root].right);
	inorder(tree, tree->nodelist[root].right);
	
}

int search(TREE* tree, int root, DATA d)
{
	//printf("current root : %d\n",root);
	if(root == -1)
		return root;
	
	if(tree->nodelist[root].data == d)
		return root;
	
	else if(tree->nodelist[root].data > d)
		return search(tree, tree->nodelist[root].left, d);
	else if(tree->nodelist[root].data < d)
		return search(tree, tree->nodelist[root].right, d);	
}


int detach_successor(TREE *tree, int node)
{
	int nptr = tree->nodelist[node].right;
	if(nptr == -1)
		return -1;
	
	if(tree->nodelist[nptr].left == -1)
	{
		tree->nodelist[node].right = tree->nodelist[nptr].right;
		return nptr;
	}
	while(tree->nodelist[nptr].left != -1)
	{
		node = nptr;
		nptr = tree->nodelist[nptr].left;
	}
	tree->nodelist[node].left = tree->nodelist[nptr].right;
	return nptr;
}


void delete(TREE *tree, int *rootptr, DATA d)
{
	
	int node = *rootptr, successor;
	if(node == -1)
		return;
	printf("current node : %d\n",node);
	
	if(d < tree->nodelist[node].data)
		delete(tree, &(tree->nodelist[node].left), d);
	else if(d > tree->nodelist[node].data)
		delete(tree, &(tree->nodelist[node].right), d);
	else
	{
		if(tree->nodelist[node].left == -1 && tree->nodelist[node].right == -1)
		{
			*rootptr = -1;
			tree->num_nodes--;
			return;
		}
		if(tree->nodelist[node].left == -1)
		{
			*rootptr = tree->nodelist[node].right;
			tree->nodelist[node].right = -1;
			tree->num_nodes--;
			return;
		}
		if(tree->nodelist[node].right == -1)
		{
			*rootptr = tree->nodelist[node].left;
			tree->nodelist[node].left = -1;
			tree->num_nodes--;
			return;
		}
		
		successor = detach_successor(tree,node);
		tree->nodelist[node].data = tree->nodelist[successor].data;
		tree->num_nodes--;
		tree->nodelist[successor].right = -1;
		tree->nodelist[successor].left = -1;
	}
	return;
}


int main(int ac, char *av[])
{
    int N, i;
    NODE *nodeptr;
    TREE tree;

    if (ac != 2)
        ERR_MESG("Usage: bst <number>");
    N = atoi(av[1]); // max. number of nodes

    /* Initialise tree */
    tree.num_nodes = 0;
    tree.max_nodes = N;
    tree.root = -1;
    tree.free_list = 0;
    if (NULL == (tree.nodelist = Malloc(N, NODE)))
        ERR_MESG("bst: out of memory");

    /* Insert nodes */
    //srandom((int) time(NULL));
    for (i = 0; i < N; i++)
        insert(&tree, &(tree.root), random() % 1000);

    //print_pstree(&tree, tree.root);
    printf("%u\n", tree.num_nodes);
    for (i = 0, nodeptr = tree.nodelist; i < tree.num_nodes; i++, nodeptr++)
        printf("%d %d %d %d\n", i, nodeptr->data,
#ifdef LINENUM
               (nodeptr->left > 0) ? nodeptr->left + 2 : nodeptr->left,
               (nodeptr->right > 0) ? nodeptr->right + 2 : nodeptr->right
#else
               nodeptr->left, nodeptr->right
#endif
            );
	
	printf("free_list index : %d\n",tree.free_list);
	
	/*
	printf("Inorder traversal\n");
	inorder(&tree, tree.root);
	putchar('\n');
	*/
	
	/*
	int search_key;
	scanf("%d",&search_key);
	printf("Search key is %d\n",search_key);
	int idx = search(&tree, tree.root, search_key);
	printf("key found at : %d\n",idx);
	*/
	
	/*
	int node;
	scanf("%d",&node);
	printf("successor index: %d\n",detach_successor(&tree,node));
	*/
	
	printf("Deleting: \n");
	int delete_key;
	scanf("%d",&delete_key);
	delete(&tree, &(tree.root), delete_key);
	
	printf("Printing tree array\n%u\n", tree.num_nodes);
    for (i = 0, nodeptr = tree.nodelist; i < tree.max_nodes; i++, nodeptr++)
        printf("%d %d %d %d\n", i, nodeptr->data,nodeptr->left, nodeptr->right);
	
	
	printf("Inorder traversal\n");
	inorder(&tree, tree.root);
	putchar('\n');
	
	
	//printf("num_nodes : %d\n",tree.num_nodes);
	
	
    return 0;
}
