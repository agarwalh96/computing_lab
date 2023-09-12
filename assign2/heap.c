#include "common.h"

typedef struct
{
	size_t element_size;
/*generic=>needtostorethis*/
unsigned int num_allocated, num_used;
void* array;
/*one-basedindexingused(cf.SEDGEWICKANDWAYNE)*/
int(*comparator)(void*,int,int);
/*returns-ve,0,or+ve,asforqsort*/
} HEAP;

typedef char string[20];

void initHeap(HEAP *h, size_t element_size,int(*comparator)(void*,int,int))
{
	h->element_size = element_size;
	h->num_allocated = 10;
	h->num_used = 0;
	if(NULL == (h->array = malloc(h->num_allocated * element_size)))
	{
		perror("initHeap:outofmemory");
		exit(-1);
	}
	h->comparator = comparator;
	return;
}

static void swap(HEAP *h,int i,int j)
{
	/*NOTE:One-basedindexingisused.h->array[0]isunusedand*canbeusedasthetemporarylocationwhileswapping*/
	char *ip = (char*) h->array + i * h->element_size;
	char *jp = (char*) h->array + j * h->element_size;
	char *tp = (char*) h->array;
	memcpy((void*) tp, (void*) ip, h->element_size);
	memcpy((void*) ip, (void*) jp, h->element_size);
	memcpy((void*) jp, (void*) tp, h->element_size);
	return;
}

static void swapUp(HEAP *h,int k)
{	
	while(k > 1 && (h->comparator(h->array, k/2, k) < 0))
	{
		swap(h, k, k/2);
		k = k/2;
	}
	return;
}

void insert(HEAP *h,void *x)
{
	/*First,makesurethere’sspaceforanotherelement*/
	if(h->num_used + 1 == h->num_allocated) 
	{
		h->num_allocated *= 2;
		if(NULL == (h->array = realloc(h->array, h->num_allocated *h->element_size))) 
		{
			perror("insert:outofmemory");
			exit(-1);
		}
	}
	/*Insertelementatend*/
	h->num_used++;
	memcpy((char*) h->array + h->num_used * h->element_size,x,h->element_size);
	/*Restoreheapproperty*/
	swapUp(h, h->num_used);	
	return;
}


static void swapDown(HEAP *h,int k)
{
	while(2*k <= h->num_used) 
	{
		int j = 2*k;
		/*choosechildwithlargerkey*/
		if(j < h->num_used && (h->comparator(h->array, j, j+1) < 0))
			j++;
		if(h->comparator(h->array, k, j) >= 0)
			break;
		swap(h, k, j);
		k = j;
	}
	return;
}


void deleteMax(HEAP *h,void* max)
{
	/*Maxisattheroot(index1)*/
	memcpy(max, h->array + h->element_size, h->element_size);
	/*Copylastelementtoroot*/
	memcpy(h->array + h->element_size,h->array + h->num_used * h->element_size,h->element_size);
	h->num_used--;
	/*Restoreheapproperty*/
	swapDown(h, 1);	
	return;
}

void heapsort(void* a,int N, size_t element_size,int (*comparator)(void* arr,int ,int)) 
{
	int k;
	HEAP h;
	h.element_size = element_size;
	h.num_allocated = h.num_used = N;
	h.array = a;
	h.comparator = comparator;
	/*Makeheapoutofarray*/
	for(k = N/2; k >= 1; k--)
	swapDown(&h, k);
	/*SortbysuccessivedeleteMax*/
	while(h.num_used > 1) 
	{
		swap(&h, 1, h.num_used);
		//movemaxtoend
		h.num_used--;
		swapDown(&h, 1);
	}
}

//=============================== Application functions below =====================================

static int compare_int(void* array,int i1,int i2)
{
	int n1 = *((int*) array + i1);
	int n2 = *((int*) array + i2);
	return n1 - n2;
}

static int compare_str(void* array,int i1,int i2)
{
	//printf("inside compare function\n");
	//char *s1 = *((char**) array + i1);
	char *s1 = ((string*)array)[i1];
	//printf("string 1 is : %s\n",s1);
	char *s2 = *((string*) array + i2);
	//printf("string 2 is : %s\n",s2);
	
	return strcmp(s1,s2);
}

/*
int main()
{
	HEAP h;
	initHeap(&h, sizeof(string), compare_str);
	
	string k="start";
	insert(&h,(void *)k);
	// k="glass";
	insert(&h,(void *)"glass");
	//k="mug";
	insert(&h,(void *)"mug");
	//k="apple";
	insert(&h,(void *)"apple");
	//k="beer";
	insert(&h,(void *)"beer");
	//k="hex";
	insert(&h,(void *)"hex");
	//k="curl";
	insert(&h,(void *)"curl");

	for(int i=1;i<=h.num_used;i++)
	{
		//printf("%s ",((char *)(h.array))+i);
		printf("%s ",((string*)h.array)[i]);
	}
	printf("\n");
	
	char* deleted_key= (char *) calloc(50,sizeof(char));
	deleteMax(&h, deleted_key);
	printf("deleted key is : %s \n",deleted_key);
	free(deleted_key);
	for(int i=1;i<=h.num_used;i++)
	{
		printf("%s ",((string *)(h.array)+i));
	}
	printf("\n");
	
	insert(&h,(void *)"zoro");
	
	for(int i=1;i<=h.num_used;i++)
	{
		//printf("%s ",((char *)(h.array))+i);
		printf("%s ",((string*)h.array)[i]);
	}
	printf("\n");
	
	return 0;
}
*/