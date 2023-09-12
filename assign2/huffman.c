#include "common.h"
#include "heap.c"

#define INTERNAL -5
#define MAX_ASCII_VAL 256

//typedef char string[20]; already defined in heap.c

int* freq;
string* codes;


typedef struct h_node
{
	int c;
	int prob;
	struct h_node* left;
	struct h_node* right;
}h_node;

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
	for(int i=0;i<MAX_ASCII_VAL;i++)
	{
		if(freq[i] !=0)
			printf("%c ascii %d has freq %d\n",i,i,freq[i]);
	}
}

void print_codes()
{
	for(int i=1;i<MAX_ASCII_VAL;i++)
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
	
	unsigned char buf = 0;
	int idx=8;
	int c;
	//printf("\n\n==========================Encoding starts =======================\n");
	while(1)
	{
		c = fgetc(fin);
		//printf("character to encode: %c ascii %d\n",c,c);
		//fprintf(fout,"%s",codes[c]);
		string code;
		if(c == EOF)
			strcpy(code, codes[0]);
		else
			strcpy(code, codes[c]);
		
		//printf("character to encode: %c ascii %d with code %s\n",c,c,code);
		for(int i=0;i<strlen(code);i++)
		{
			if(code[i] == '0')
			{

			}
			else
			{
				buf = buf | (1 << (idx - 1));
			}
			
			idx--;
			if(idx == 0)
			{
				fprintf(fout,"%c",buf);
				//printf("writing the number %d\n",buf);
				buf = 0;
				idx=8;
			}
			
		}
		
		if(c == EOF)
			break;
	}
	if(idx != 8)
	{
		fprintf(fout,"%c",buf);
		//printf("printing leftover string: %d, overshoot idx: %d\n",buf,idx);
	}		
	
	fclose(fin);
	fclose(fout);
}


void read_coded_file(char *input_file)
{
	FILE *fin = fopen(input_file,"r");
	int c;
	while(1)
	{
		c = fgetc(fin);
		printf("current encoded char is %d\n",c);
		if( EOF == c)
			break;
	}
	fclose(fin);
}

void decode_text(char *input_file, char *output_file, h_node* tree_root)
{
	//printf("\n\n========================Decoding starts =======================\n");
	FILE *fin = fopen(input_file,"r");
	FILE *fout = fopen(output_file,"w");
	if(fin == NULL || fout == NULL)
		exit(1);
	int c;
	h_node* tree_ptr=tree_root;
	
	int eof_flag = 0;
	while(1)
	{
		c = fgetc(fin);
		//printf("current encoded char is %d\n",c);
		if(EOF == c)
			break;
		for(int i = 8; i>0; i--)
		{
			if(c & (1 << (i - 1)))
			{
				tree_ptr = tree_ptr->right;
			}
			else
			{
				tree_ptr = tree_ptr->left;
			}
				
			if(tree_ptr->c != INTERNAL)
			{		
				//printf("writing to file character: %c, ascii %d\n",tree_ptr->c, tree_ptr->c);
				
				if(tree_ptr->c == 0)   // 0 is being used for EOF
				{
					eof_flag = 1;
					break;
				}
				
				fprintf(fout,"%c",tree_ptr->c);
				tree_ptr=tree_root;
			}	
		}
		if(eof_flag)
			break;
	}
	
	
	fclose(fin);
	fclose(fout);
}

static int compare_nodes(void* array,int i1,int i2)
{
	h_node s1 = ((h_node*)array)[i1];
	h_node s2 = *((h_node*) array + i2);
	
	return (s1.prob-s2.prob) * -1;
}

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
		
		free(new);
		
	}
	
	h_node* root = (h_node*) calloc(1,sizeof(h_node));
	deleteMax(huffman, root);
	
	return root;
	
	
}

void get_huff_tree_codes(h_node* root, char code[20])
{
	if(NULL == root)
	{
		if(strlen(code) != 0)
			code[strlen(code) - 1] = '\0'; 
		return;
	}
	
	if(root->c != INTERNAL)
	{
		//printf("character is %c ( %d ), code is %s\n",root->c,root->c,code);
		strcpy(codes[root->c],code);
	}
	get_huff_tree_codes(root->left,strcat(code,"0"));
	get_huff_tree_codes(root->right,strcat(code,"1"));
	if(strlen(code) != 0)
		code[strlen(code) - 1] = '\0';
}

void free_huff_tree(h_node* root)
{
	if(NULL == root)
		return;
	
	if(root->left == NULL && root->right == NULL)
	{
		free(root);
		return;
	}
	else
	{
		if(root->left != NULL)
			free_huff_tree(root->left);
		if(root->right != NULL)
			free_huff_tree(root->right);
		free(root);
	}
	
}

int main(int c,char **argv)
{
	if(c!=5)
	{
		printf("missing arguments\n");
		exit(1);
	}
	
	freq = (int *) calloc(MAX_ASCII_VAL,sizeof(int));
	codes = (string *) calloc(MAX_ASCII_VAL,sizeof(string));
	if(NULL == freq || NULL == codes)
		exit(1);
	
	read_training_data(argv[1]);
	//print_freq();
	
	HEAP huffman;
	initHeap(&huffman, sizeof(h_node), compare_nodes);
		
	//insert(&huffman,(void *)k);
	for(int i=0;i<MAX_ASCII_VAL;i++)
	{
		
		if(freq[i] !=0)
		{
			h_node node;
			node.c = i;
			node.prob = freq[i];
			node.right = node.left = NULL;
			//printf("inserting %c ascii %d has freq %d\n",i,i,freq[i]);
			insert(&huffman,(void *)&node);
		}
	}
	
	//print HEAP
	/*
	for(int i=1;i<=huffman.num_used;i++)
	{
		//printf("%s ",((char *)(h.array))+i);
		printf("character %c with freq %d\n",((h_node*)huffman.array)[i].c,((h_node*)huffman.array)[i].prob);
	}
	printf("\n");
	*/
	
	h_node* root = create_huff_tree(&huffman);
	if(NULL == root)
	{
		printf("SOME error in huffman tree creation\n");
		exit(1);
	}
	
	//print HEAP
	//printf("printing heap after tree creation\n");
	/*
	for(int i=1;i<=huffman.num_used;i++)
	{
		//printf("%s ",((char *)(h.array))+i);
		printf("character %c with freq %d\n",((h_node*)huffman.array)[i].c,((h_node*)huffman.array)[i].prob);
	}
	printf("\n");
	*/
	
	//store huffman codes in the codes array
	char code[20] = "";
	get_huff_tree_codes(root,code);
	
	//print_codes();
	
	
	encode_text(argv[2],argv[3]);
	printf("encode complete\n");
	
	//read_coded_file(argv[3]);
	
	decode_text(argv[3],argv[4],root);
	printf("decode complete\n");
	
	
	/*
	for(int i=1;i<huffman.num_used;i++)
	{
		free(huffman.array[i]);
	}
	*/
	
	free(freq);
	free(codes);
	free_huff_tree(root);
	free(huffman.array);
	
	return 0;
}
