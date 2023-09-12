#include "common.h"
#include "avl.h"

#define TREE_MAX_NODES 1000

void print_bloom_arr(char *bloom_arr, int m)
{
	for(int i=0;i<m;i++)
	{
		
		int byte_idx = i/8;
		int bit_idx = i%8;
		//printf("byte and bit pos are: %d,%d\n",byte_idx,bit_idx);
		
		if(bloom_arr[byte_idx] & (1<<(7 - bit_idx)))
			printf("1 ");
		else
			printf("0 ");
		
	}
	putchar('\n');
}



void insert_bloom(char *bloom_arr,int num, int m, int k)
{
	print_bloom_arr(bloom_arr, m);
	srand(num);
	int flag_new=0;
	for(int i=0;i<k;i++)
	{
		int cur_hash = rand() % m;
		printf("cur hash is : %d\n",cur_hash);
		
		int byte_idx = cur_hash/8;
		int bit_idx = cur_hash%8;
		//printf("byte and bit pos are: %d,%d\n",byte_idx,bit_idx);
		
		if(bloom_arr[byte_idx] & (1<<(7 - bit_idx)))
			continue;
		
		bloom_arr[byte_idx] = bloom_arr[byte_idx] | (1<<(7 - bit_idx)) ;
		flag_new = 1;
	}
	if(flag_new)
	{
		printf("inserted\n");
	}
	else
		printf("duplicate\n");
	print_bloom_arr(bloom_arr, m);
}

int search_bloom(char *bloom_arr,int num, int m, int k)
{
	//print_bloom_arr(bloom_arr, m);
	srand(num);
	for(int i=0;i<k;i++)
	{
		int cur_hash = rand() % m;
		printf("cur hash is : %d\n",cur_hash);
		
		int byte_idx = cur_hash/8;
		int bit_idx = cur_hash%8;
		//printf("byte and bit pos are: %d,%d\n",byte_idx,bit_idx);
		
		if(!(bloom_arr[byte_idx] & (1<<(7 - bit_idx))))
			return 0;
	}
	return 1;
}


int main(int c,char **argv)
{
	//if(c!=5)
	//	exit(1);
	
	int m=atoi(argv[3]);
	int k=atoi(argv[4]);
	
	int bloom_arr_len = ceil((double)m/8);
	char* bloom_arr = (char *) calloc(bloom_arr_len, 1);
	if(NULL == bloom_arr)
		exit(1);
	
	AVL_NODE *nodeptr;
    TREE tree;
	init_tree(&tree, TREE_MAX_NODES);
	
	
	FILE *insert = fopen(argv[1],"r");
	if(NULL == insert)
		exit(1);
	
	int num;
	while(1)
	{
		int retval = fscanf(insert,"%d",&num);
		if(retval<=0)
			break;
		printf("\ncur num is : %d\n",num);
		insert_bloom(bloom_arr,num, m, k);
		insert_avl(&tree, &(tree.root), num);
	}
	fclose(insert);
	print_bloom_arr(bloom_arr, m);
	
	FILE *search = fopen(argv[2],"r");
	if(NULL == search)
		exit(1);
	
	int found_bloom=0,found_avl=0;
	while(1)
	{	
		int retval = fscanf(search,"%d",&num);
		if(retval<=0)
			break;
		printf("\ncur num to search is : %d\n",num);
		if(search_bloom(bloom_arr,num, m, k))
		{
			printf("Found in bloom\n");
			found_bloom++;
		}
		if(search_avl(&tree, tree.root, num))
		{
			printf("Found in avl\n");
			found_avl++;
		}
	}
	double fp_rate = (double)(found_bloom - found_avl)/found_bloom ; 
	fprintf(stderr,"false positive rate is : %0.6f\n",fp_rate);
	fclose(search);
	
	return 0;
}