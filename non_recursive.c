#include "common.h"

#define UNVISITED 0
#define VISITED 1

typedef int DATA;

typedef struct node {
    DATA data;
    int left, right;
    int state;
} NODE;

typedef struct {
    unsigned int num_nodes, max_nodes;
    int root, free_list; 
    NODE *nodelist;
} TREE;

typedef struct {
    unsigned int capacity, top;
    DATA *contents;
} STACK;

void init_stack(STACK *s, int capacity)
{	
	s->capacity = capacity;
	s->top = 0;
	s->contents = (DATA *) malloc(capacity * sizeof(DATA));
	if(s->contents == NULL)
		exit(1);
}

void push(STACK *s, DATA element)
{
	if(s->top == s->capacity)
		return;
	
	s->contents[s->top] = element;
	s->top++;
}
DATA pop(STACK *s)
{
	if(s->top == 0)
		return -1;	//if stack empty return -1
	//printf("elemnt getting popped is : %d\n",s->contents[s->top]);
	return s->contents[--s->top];
}

int is_empty(STACK *s)
{
	if(s->top == 0)
		return 1; // if empty return 1;
	else
		return 0;
}

void print_stack(STACK *s)
{
	printf("Stack is : ");
	for(int i=0;i<s->top; i++)
		printf("%d ",s->contents[i]);
	putchar('\n');
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

void preorder_iterative(TREE *t, int root)
{
	STACK s;
	init_stack(&s, t->max_nodes);
	push(&s,root);
	//printf("stack top at %d\n",s.top);
	int x;
	while(!is_empty(&s))
	{
		x = pop(&s);
		//printf("current x values is %d\n",x);
		if(x == -1)
		{
			continue;
		}
		
		printf("%d ",t->nodelist[x].data);
		//printf("pushing left,right values %d %d \n",t->nodelist[x].left,t->nodelist[x].right);
		push(&s,t->nodelist[x].right);
		push(&s,t->nodelist[x].left);
		//printf("current top idx of stack is %d\n",s.top);
		//print_stack(&s);
	}
	putchar('\n');
}

void inorder_iterative(TREE *t, int root)
{
	STACK s;
	init_stack(&s, t->max_nodes);
	t->nodelist[root].state = UNVISITED;
	push(&s,root);
	//printf("stack top at %d\n",s.top);
	int x;
	while(!is_empty(&s))
	{
		//print_stack(&s);
		x = pop(&s);
		
		if(x == -1)
			continue;
		
		if(t->nodelist[x].state == UNVISITED)
		{
			if(t->nodelist[x].right != -1)
				t->nodelist[t->nodelist[x].right].state = UNVISITED;
			push(&s,t->nodelist[x].right);

			t->nodelist[x].state = VISITED;
			push(&s,x);
			
			if(t->nodelist[x].left != -1)
				t->nodelist[t->nodelist[x].left].state = UNVISITED;
			push(&s,t->nodelist[x].left);
			
		}
		else
		{
			printf("%d ",t->nodelist[x].data);
		}
	}
	putchar('\n');
}

void postorder_iterative(TREE *t, int root)
{
	STACK s;
	init_stack(&s, t->max_nodes);
	t->nodelist[root].state = UNVISITED;
	push(&s,root);
	//printf("stack top at %d\n",s.top);
	int x;
	while(!is_empty(&s))
	{
		//print_stack(&s);
		x = pop(&s);
		
		if(x == -1)
			continue;
		
		if(t->nodelist[x].state == UNVISITED)
		{
			t->nodelist[x].state = VISITED;
			push(&s,x);
		
			if(t->nodelist[x].right != -1)
				t->nodelist[t->nodelist[x].right].state = UNVISITED;
			push(&s,t->nodelist[x].right);			
			
			if(t->nodelist[x].left != -1)
				t->nodelist[t->nodelist[x].left].state = UNVISITED;
			push(&s,t->nodelist[x].left);
			
		}
		else
		{
			printf("%d ",t->nodelist[x].data);
		}
	}
	putchar('\n');
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
	
    for (i = 0; i < N; i++)
        tree.nodelist[i].left = i+1;
    tree.nodelist[N-1].left = -1;
    
    /* Insert nodes */
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
    
    printf("Printing preorder\n");
    preorder_iterative(&tree, tree.root);
    
    printf("Printing inorder\n");
    inorder_iterative(&tree, tree.root);
    
    printf("Printing postorder\n");
    postorder_iterative(&tree, tree.root);
    
}
    

