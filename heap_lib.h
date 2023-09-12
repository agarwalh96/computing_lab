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
