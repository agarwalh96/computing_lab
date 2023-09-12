#include "common.h"
#include "heap.c"

#define INTERNAL -5
/*
typedef struct hashitem
{
	
}hashitem;

typedef struct hashtable
{
	hashitem **freq;
	int (*hash_func)(int);
}hashtable;
*/

//typedef char string[10];

static int freq[200];
static string codes[200];


typedef struct h_node
{
	int c;
	int prob;
	struct h_node* left;
	struct h_node* right;
}h_node;

/*
typedef struct huff_tree_node
{
	int data;
	struct huff_tree_node* left;
	struct huff_tree_node* right;
}huff_tree_node;
*/

void read_training_data(char *filename)
{
	FILE *fp = fopen(filename,"r");
	if(fp == NULL)
		exit(1);
	
	int c;
	while(EOF != (c = fgetc(fp)))
	{
		freq[c]++;
	}
	freq[0] = 1; //used for EOF
	fclose(fp);
}

void print_freq()
{
	for(int i=0;i<200;i++)
	{
		if(freq[i] !=0)
			printf("%c ascii %d has freq %d\n",i,i,freq[i]);
	}
}

void print_codes()
{
	for(int i=1;i<200;i++)
	{
		if(freq[i] !=0)
			printf("%c ascii %d has coded value %s\n",i,i,codes[i]);
	}
}

void encode_text(char *input_file, char *output_file)
{
	FILE *fin = fopen(input_file,"r");
	FILE *fout = fopen(output_file,"w");
	if(fin == NULL || fout == NULL)
		exit(1);
	
	
	int c;
	while(EOF != (c = fgetc(fin)))
	{
		//fprintf(fout,"%s",codes[c]);
	}
	
	
	fclose(fin);
	fclose(fout);
}


void decode_text(char *input_file, char *output_file, h_node* tree_root)
{
	FILE *fin = fopen(input_file,"r");
	FILE *fout = fopen(output_file,"w");
	if(fin == NULL || fout == NULL)
		exit(1);
	int c;
	h_node* tree_ptr=tree_root;
	while(EOF != (c = fgetc(fin)))
	{
		if(c=='0')
		{
			tree_ptr = tree_ptr->left;
		}
		else
		{
			tree_ptr = tree_ptr->right;
		}
		
		if(tree_ptr->c != INTERNAL)
		{
			fprintf(fout,"%c",tree_ptr->c);
			tree_ptr=tree_root;
		}
	}
	
	
	fclose(fin);
	fclose(fout);
}

static int compare_nodes(void* array,int i1,int i2)
{
	//printf("inside compare function\n");
	//char *s1 = *((char**) array + i1);
	h_node s1 = ((h_node*)array)[i1];
	//printf("string 1 is : %s\n",s1);
	h_node s2 = *((h_node*) array + i2);
	//printf("string 2 is : %s\n",s2);
	
	return (s1.prob-s2.prob) * -1;
}

/*
huff_tree_node* init_huff_tree_node(h_node *heap_node)
{
	huff_tree_node* tree_node = (huff_tree_node*) calloc(1,sizeof(huff_tree_node));
	if(NULL == tree_node)
		exit(1);
	
	tree_node->data = heap_node->c;
	tree_node->left = NULL;
	tree_node->right = NULL;
	
	return tree_node;
}
*/

h_node* create_huff_tree(HEAP *huffman)
{
	if(huffman->num_used == 0)
		return NULL;
	
	
	while(huffman->num_used > 1)
	{
		h_node* s1 = (h_node*) calloc(1,sizeof(h_node));
		h_node* s2 = (h_node*) calloc(1,sizeof(h_node));
		h_node* new = (h_node*) calloc(1,sizeof(h_node));
		if(NULL == s1 || NULL == s2 || NULL == new)
			exit(1);
		
		deleteMax(huffman, s1);
		deleteMax(huffman, s2);
		new->c = INTERNAL;
		new->prob = s1->prob + s2->prob;
		new->left = s1;
		new->right = s2;
		insert(huffman,(void *)new);
	}
	
	h_node* root = (h_node*) calloc(1,sizeof(h_node));
	deleteMax(huffman, root);
	//huff_tree_node* tree_node = init_huff_tree_node(&((h_node*)huffman->array)[0]);
	return root;
	
	
}

void print_huff_tree(h_node* root, char code[10])
{
	if(NULL == root)
	{
		if(strlen(code) != 0)
			code[strlen(code) - 1] = '\0'; 
		return;
	}
	
	if(root->c != INTERNAL)
	{
		printf("character is %c ( %d ), code is %s\n",root->c,root->c,code);
		strcpy(codes[root->c],code);
	}
	print_huff_tree(root->left,strcat(code,"0"));
	print_huff_tree(root->right,strcat(code,"1"));
	if(strlen(code) != 0)
		code[strlen(code) - 1] = '\0';
}

int main(int c,char **argv)
{
	if(c<2)
	{
		printf("missing arguments\n");
		exit(1);
	}
	
	read_training_data(argv[1]);
	//print_freq();
	
	HEAP huffman;
	initHeap(&huffman, sizeof(h_node), compare_nodes);
	
	
	
	//insert(&huffman,(void *)k);
	for(int i=1;i<200;i++)
	{
		
		if(freq[i] !=0)
		{
			h_node node;
			node.c = i;
			node.prob = freq[i];
			node.right = node.left = NULL;
			printf("inserting %c ascii %d has freq %d\n",i,i,freq[i]);
			insert(&huffman,(void *)&node);
		}
	}
	
	//print HEAP
	for(int i=1;i<=huffman.num_used;i++)
	{
		//printf("%s ",((char *)(h.array))+i);
		printf("character %c with freq %d\n",((h_node*)huffman.array)[i].c,((h_node*)huffman.array)[i].prob);
	}
	printf("\n");
	
	h_node* root = create_huff_tree(&huffman);
	if(NULL == root)
	{
		printf("SOME error in huffman tree creation\n");
		exit(1);
	}
	
	//print HEAP
	printf("printing heap after tree creation\n");
	for(int i=1;i<=huffman.num_used;i++)
	{
		//printf("%s ",((char *)(h.array))+i);
		printf("character %c with freq %d\n",((h_node*)huffman.array)[i].c,((h_node*)huffman.array)[i].prob);
	}
	printf("\n");
	
	printf("printing huff tree:\n");
	char code[10] = "";
	print_huff_tree(root,code);
	
	print_codes();
	
	
	encode_text(argv[2],argv[3]);
	
	
	decode_text(argv[3],argv[4],root);
	
	return 0;
}