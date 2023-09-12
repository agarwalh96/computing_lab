#include "common.h"

#define NUM_SYMS 26

typedef unsigned int TRIE_NODE[NUM_SYMS + 1];

unsigned int max_nodes, num_nodes;

TRIE_NODE *trie;

int init_trie()
{
	max_nodes = 10000;
	if(NULL == (trie = (TRIE_NODE *) calloc(max_nodes,sizeof(TRIE_NODE))))
		ERR_MESG("init-trie:outofmemory\n");
	
	num_nodes = 1;
	return 0;
}

int insert_node()
{
	if(num_nodes == max_nodes) 
	{
		max_nodes *= 2;
		if(NULL == (trie = Realloc(trie, max_nodes, TRIE_NODE)))
			ERR_MESG("insert-node:outofmemory\n");
		bzero((void*) (trie + num_nodes), num_nodes *sizeof(TRIE_NODE));
	}
	
	num_nodes++;
	return num_nodes - 1;
}

int insert_string(char* s)
{
	unsigned int index = 0;
	int c, new_index;
	while(*s) 
	{
		c = *s;
		if(c >='A'&& c <='Z')
			c ='a'+ c -'A';
		if(c >='a'&& c <='z') 
		{
			c = c -'a';
			if(trie[index][c] != 0)
			/*justfollowthepointer*/
				index = trie[index][c];
			else
			{
				/*neednewnode*/
				if(UNDEF == (new_index = insert_node()))
					return UNDEF;
				index = trie[index][c] = new_index;
			}
		}
		else
			fprintf(stderr,"Unexpected character %d\n", c);
		s++;
	}
	trie[index][NUM_SYMS]++;
	
	return 0;
}

int search_string(char* s)
{
	unsigned int index = 0;
	int c, new_index;
	//found_flag = 0;
	while(*s) 
	{
		c = *s;
		if(c >='A'&& c <='Z')
			c ='a'+ c -'A';
		if(c >='a'&& c <='z') 
		{
			c = c -'a';
			if(trie[index][c] != 0)
			/*justfollowthepointer*/
				index = trie[index][c];
			else
			{
				return 0;
			}
		}
		else
			fprintf(stderr,"Unexpected character %d\n", c);
		s++;
	}
	if(trie[index][NUM_SYMS])
		return 1;
	else
		return 0;
}


void print_trie()
{
	for(int i=0;i<num_nodes;i++)
	{
	
	}
}
				
int main()
{
	init_trie();
	printf("enter number of strings\n");
	int n;
	scanf("%d",&n);
	for(int i=0;i<n;i++)
	{
		char *str;
		scanf("%ms",&str);
		insert_string(str);
		free(str);
	}
	
	printf("what to search?\n");
	char *str;
	scanf("%s",str);
	if(search_string(str))
		printf("found\n");
	else
		printf("not found\n");
	
	return 0;
}		
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
