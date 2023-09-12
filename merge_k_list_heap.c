#include "heap_lib.h"

typedef struct
{
	int *list_ptr;
	int list_idx;
	int length;
}list_ind;

static int compare(void* array,int i1,int i2)
{
	//printf("inside compare function\n");
	//char *s1 = *((char**) array + i1);
	list_ind s1 = ((list_ind*)array)[i1];
	//printf("string 1 is : %s\n",s1);
	list_ind s2 = ((list_ind*)array)[i2];
	//printf("string 2 is : %s\n",s2);
	//printf("comparing elements %d and %d\n",s1.list_ptr[s1.list_idx] , s2.list_ptr[s2.list_idx]);
	return (s1.list_ptr[s1.list_idx] - s2.list_ptr[s2.list_idx]) * -1;
}

int main()
{
	HEAP h;
	initHeap(&h, sizeof(list_ind), compare);
	
	int num_list;
	scanf("%d",&num_list);
	int** k_list = (int **) calloc(num_list,sizeof(int *));
	for(int i=0;i<num_list;i++)
	{
		printf("num elements in list?\n");
		int num=0;
		scanf("%d",&num);
		k_list[i] = (int *)calloc(num,sizeof(int));
		for(int j=0;j<num;j++)
			scanf("%d",&k_list[i][j]);
		
		list_ind l;
		l.list_ptr = k_list[i];
		l.list_idx = 0;
		l.length = num;
		insert(&h,(void *)&l);
	}
	
	while(h.num_used > 0)
	{
		list_ind min;
		deleteMax(&h,(void *)&min);
		printf("curr elements is %d\n",min.list_ptr[min.list_idx]);
		min.list_idx++;
		if(min.list_idx < min.length)
			insert(&h,(void *)&min);
	}
	
	
	
	
	return 0;
}
