#include "common.h"

typedef int DATA;

typedef struct node {
    DATA data;
    int left, right;
} NODE;

typedef struct {
    unsigned int num_nodes, max_nodes;
    int root, free_list; 
    NODE *nodelist;
} TREE;


void read_tree(TREE *tree, char *filename)
{
	FILE *fp = fopen(filename,"r");
	if(NULL == fp)
		exit(1);
	
	fscanf(fp,"%d",&(tree->num_nodes));
    tree->max_nodes = tree->num_nodes;
    tree->root = 0;
    tree->free_list = 0;
    if (NULL == (tree->nodelist = Malloc(tree->num_nodes, NODE)))
        ERR_MESG("bst: out of memory");
	
    for (int i = 0; i < tree->num_nodes; i++)
	{
		int idx;
		fscanf(fp,"%d",&idx);
		//printf("index is : %d ",idx);
		fscanf(fp,"%d",&(tree->nodelist[idx].data));
		fscanf(fp,"%d",&(tree->nodelist[idx].left));
		fscanf(fp,"%d",&(tree->nodelist[idx].right));
	}
	putchar('\n');
	fclose(fp);
}

void inorder(TREE *tree, int root)
{
	if(root == -1)
		return;
	inorder(tree, tree->nodelist[root].left);
	//printf("%d %d %d %d \n",root, tree->nodelist[root].data,tree->nodelist[root].left, tree->nodelist[root].right);
	printf("%d ",tree->nodelist[root].data);
	inorder(tree, tree->nodelist[root].right);
	
}

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
	{
		s->capacity = s->capacity * 2;
		s->contents = (DATA *) realloc(s->contents, s->capacity * sizeof(DATA));
		if(s->contents == NULL)
			exit(1);
	}
	
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

DATA peek(STACK *s)
{
	if(s->top == 0)
		return -1;	//if stack empty return -1
	//printf("elemnt getting popped is : %d\n",s->contents[s->top]);
	return s->contents[s->top - 1];
	
}

int is_empty(STACK *s)
{
	if(s->top == 0)
		return 1; // if empty return 1;
	else
		return 0;
}

int main(int c,char **argv)
{
	
	TREE set1,set2;
	read_tree(&set1,argv[1]);
	read_tree(&set2,argv[2]);
	
	printf("inorder set1:\n");
	inorder(&set1,set1.root);
	putchar('\n');
	
	printf("inorder set2:\n");
	inorder(&set2,set2.root);
	putchar('\n');
	
	STACK s1,s2;
	init_stack(&s1,10);
	init_stack(&s2,10);
	
	int num_set1 = set1.num_nodes, num_set2 = set2.num_nodes, num_common = 0;
	
	int ptr1=set1.root,ptr2=set2.root;
	//printf("set1 element : %d, set2 : %d\n",set1.nodelist[ptr1].data , set2.nodelist[ptr2].data);
	//printf("ptr1 is %d, ptr2 is %d\n",ptr1,ptr2);
	while(1)
	{
		//printf("ptr1 is %d, ptr2 is %d\n",ptr1,ptr2);
		if(ptr1 != -1)
		{
			//printf("ptr1 data is : %d\n",set1.nodelist[ptr1].data);
			push(&s1,ptr1);
			ptr1 = set1.nodelist[ptr1].left;
		}
		else if(ptr2 != -1)
		{
			//printf("ptr2 data is : %d\n",set2.nodelist[ptr2].data);
			push(&s2,ptr2);
			ptr2 = set2.nodelist[ptr2].left;
		}
		else if(!is_empty(&s1) && !is_empty(&s2))
		{
			
			ptr1 = peek(&s1);
			ptr2 = peek(&s2);
			//printf("Current ptr1 : %d, ptr2 : %d\n",ptr1 , ptr2);
			//printf("Current set1 element : %d, set2 : %d\n",set1.nodelist[ptr1].data , set2.nodelist[ptr2].data);
			if(set1.nodelist[ptr1].data == set2.nodelist[ptr2].data)
			{
				//printf("match. set1 element : %d, set2 : %d\n",set1.nodelist[ptr1].data , set2.nodelist[ptr2].data);
				num_common++;
				pop(&s1);
				pop(&s2);
				
				ptr1 = set1.nodelist[ptr1].right;
				ptr2 = set2.nodelist[ptr2].right;
			}
			else if(set1.nodelist[ptr1].data < set2.nodelist[ptr2].data)
			{
				//printf("mis match. set1 element : %d, set2 : %d\n",set1.nodelist[ptr1].data , set2.nodelist[ptr2].data);
				printf("%d ",set1.nodelist[ptr1].data);
				pop(&s1);
				
				ptr1 = set1.nodelist[ptr1].right;
				ptr2 = -1;
			}
			else if(set1.nodelist[ptr1].data > set2.nodelist[ptr2].data)
			{
				//printf("mis match. set1 element : %d, set2 : %d\n",set1.nodelist[ptr1].data , set2.nodelist[ptr2].data);
				printf("%d ",set2.nodelist[ptr2].data);
				pop(&s2);
				
				ptr1 = -1;
				ptr2 = set2.nodelist[ptr2].right;
			}	
		}
		else if(!is_empty(&s1))
		{
			ptr1 = pop(&s1);
			printf("%d ",set1.nodelist[ptr1].data);
			ptr1 = set1.nodelist[ptr1].right;
		}
		else if(!is_empty(&s2))
		{
			ptr2 = pop(&s2);
			printf("%d ",set2.nodelist[ptr2].data);
			ptr2 = set2.nodelist[ptr2].right;
		}
		
		else
		{
			break;
		}
	}
	putchar('\n');
	
	float dice = (double) 2 * num_common / (num_set1 + num_set2);
	printf("Dice coefficient is : %0.4f\n",dice);
	
	return 0;
}