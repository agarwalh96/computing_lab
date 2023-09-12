#include "common.h"
#include "heap_lib.h"

#define MAX_NAME_LEN 64
//remove
#define NUM_FIELDS 5

typedef struct {
	unsigned int id;
    char manufact[MAX_NAME_LEN];
	char model[MAX_NAME_LEN];
    unsigned int year;
    float capacity;
    float mileage;
	float price;
    float emissions;
}CAR_DATA;

void read_car_data(CAR_DATA *car_data, FILE *fp, int num_cars)
{
    char inbuf[BUF_LEN], *s;
    CAR_DATA *sptr;

    for (sptr = car_data; sptr < car_data + num_cars; sptr++) 
	{
        if (NULL == fgets(inbuf, BUF_LEN, fp))
            ERR_MESG("read-car-data: premature end of input\n");
        printf("current inbuf : %s\n",inbuf);
		
		if (NULL == (s = strtok(inbuf, " \t\n")))
            ERR_MESG("read-car-data: malformed line 1\n");
        strncpy(sptr->manufact, s, MAX_NAME_LEN - 1);
        sptr->manufact[MAX_NAME_LEN] = '\0';  //change to -1
		
		if (NULL == (s = strtok(NULL, " \t\n")))
			ERR_MESG("read-car-data: malformed line2\n");
        strncpy(sptr->model, s, MAX_NAME_LEN - 1);
        sptr->model[MAX_NAME_LEN] = '\0';		//change to -1

        if (NULL == (s = strtok(NULL, " \t\n")))
			ERR_MESG("read-car-data: malformed line3\n");
        sptr->year = atoi(s);

        if (NULL == (s = strtok(NULL, " \t\n")))
			ERR_MESG("read-car-data: malformed line4\n");
        sptr->capacity = (float) atof(s);

        if (NULL == (s = strtok(NULL, " \t\n")))
			ERR_MESG("read-car-data: malformed line5\n");
        sptr->mileage = (float) atof(s);
		
		if (NULL == (s = strtok(NULL, " \t\n")))
			ERR_MESG("read-car-data: malformed line6\n");
		sptr->price = (float) atof(s);
		
		if (NULL == (s = strtok(NULL, " \t\n")))
			ERR_MESG("read-car-data: malformed line7\n");
		sptr->emissions = (float) atof(s);
		
		printf("data read\n");
    }

    //return car_data;
}

void print_car_data(CAR_DATA *car_data, int num_cars)
{
    CAR_DATA *ptr;

    for (ptr = car_data; ptr < car_data + num_cars; ptr++)
        printf("%s %s %u %f %f %f %f\n",
               ptr->manufact, ptr->model, ptr->year, ptr->capacity, ptr->mileage,
               ptr->price, ptr->emissions);

    return;
}

static int compare_mileage(void* array,int i1,int i2)
{
	float m1 = ((CAR_DATA *)array)[i1].mileage;
	float m2 = ((CAR_DATA *)array)[i2].mileage;
	return (m1 - m2) * (-1);
}

static int compare_emissions(void* array,int i1,int i2)
{
	float m1 = ((CAR_DATA *)array)[i1].emissions;
	float m2 = ((CAR_DATA *)array)[i2].emissions;
	return m1 - m2;
}

static int compare_id(void* array,int i1,int i2)
{
	unsigned int m1 = ((CAR_DATA *)array)[i1].id;
	unsigned int m2 = ((CAR_DATA *)array)[i2].id;
	return m1 - m2;
}

CAR_DATA read_car(FILE *fp)
{
	char inbuf[BUF_LEN], *s;
	CAR_DATA car;
	if (NULL == fgets(inbuf, BUF_LEN, fp))
        ERR_MESG("read-car-data: premature end of input\n");
    //printf("current inbuf : %s\n",inbuf);
		
	if (NULL == (s = strtok(inbuf, " \t\n")))
		ERR_MESG("read-car-data: malformed line 1\n");
	strncpy(car.manufact, s, MAX_NAME_LEN - 1);
	car.manufact[MAX_NAME_LEN] = '\0';  //change to -1
		
	if (NULL == (s = strtok(NULL, " \t\n")))
		ERR_MESG("read-car-data: malformed line2\n");
	strncpy(car.model, s, MAX_NAME_LEN - 1);
	car.model[MAX_NAME_LEN] = '\0';		//change to -1

	if (NULL == (s = strtok(NULL, " \t\n")))
		ERR_MESG("read-car-data: malformed line3\n");
	car.year = atoi(s);

	if (NULL == (s = strtok(NULL, " \t\n")))
		ERR_MESG("read-car-data: malformed line4\n");
	car.capacity = (float) atof(s);

	if (NULL == (s = strtok(NULL, " \t\n")))
		ERR_MESG("read-car-data: malformed line5\n");
	car.mileage = (float) atof(s);
	
	if (NULL == (s = strtok(NULL, " \t\n")))
		ERR_MESG("read-car-data: malformed line6\n");
	car.price = (float) atof(s);
	
	if (NULL == (s = strtok(NULL, " \t\n")))
		ERR_MESG("read-car-data: malformed line7\n");
	car.emissions = (float) atof(s);
	
	//printf("data read\n");
	
	return car;
}

int main(int c,char **argv)
{
	if(c!=3)
		exit(1);
	int best = atoi(argv[2]);
	FILE *fp = fopen(argv[1],"r");
	if(NULL == fp)
		exit(1);
	
	int num_cars;
	fscanf(fp,"%d\n",&num_cars);
	CAR_DATA car_data[num_cars];
	//read_car_data(car_data, fp,  num_cars);
	
	HEAP minh_mileage,maxh_emissions;
	initHeap(&minh_mileage, sizeof(CAR_DATA), compare_mileage);
	initHeap(&maxh_emissions, sizeof(CAR_DATA), compare_emissions);
	
	for(int i =0; i<num_cars;i++)
	{
		CAR_DATA car = read_car(fp);
		car.id = i;
		
		if(i<best)
		{
			insert(&minh_mileage,(void *)&car);
			insert(&maxh_emissions,(void *)&car);
			//printf("inserted\n");
			//printf("current mileage : %f\n",car.mileage);
		}
		else
		{
			//mileage heap
			if(car.mileage > ((CAR_DATA *)minh_mileage.array)[1].mileage)
			{
				CAR_DATA temp;
				deleteMax(&minh_mileage,&temp);
				insert(&minh_mileage,(void *)&car);
				//printf("Upserted\n");
				//printf("current mileage : %f\n",car.mileage);
			}	
			else
			{
				//printf("not inserted\n");
			}
			
			//emissions heap
			if(car.emissions < ((CAR_DATA *)maxh_emissions.array)[1].emissions)
			{
				CAR_DATA temp;
				deleteMax(&maxh_emissions,&temp);
				insert(&maxh_emissions,(void *)&car);
				printf("Upserted\n");
				printf("current emissions : %f\n",car.emissions);
			}
		}
	}
	
	heapsort(minh_mileage.array, best, sizeof(CAR_DATA),compare_id);
	heapsort(maxh_emissions.array, best, sizeof(CAR_DATA),compare_id);
	
	printf("\nprinting car best mileage data:\n");
	for(int i =1; i<=minh_mileage.num_used;i++)
	{
		CAR_DATA ptr = ((CAR_DATA *)minh_mileage.array)[i];
		printf("%u %s %s %u %f %f %f %f\n",
               ptr.id, ptr.manufact, ptr.model, ptr.year, ptr.capacity, ptr.mileage,
               ptr.price, ptr.emissions);
	}
	
	printf("\nprinting car best emissions data:\n");
	for(int i =1; i<=maxh_emissions.num_used;i++)
	{
		CAR_DATA ptr = ((CAR_DATA *)maxh_emissions.array)[i];
		printf("%u %s %s %u %f %f %f %f\n",
               ptr.id, ptr.manufact, ptr.model, ptr.year, ptr.capacity, ptr.mileage,
               ptr.price, ptr.emissions);
	}
	
	printf("\nprinting common best:\n");
	int ptr1=1, ptr2=1,flag=0;
	while(ptr1 <=best && ptr2<=best)
	{
		CAR_DATA c1 = ((CAR_DATA *)minh_mileage.array)[ptr1];
		CAR_DATA c2 = ((CAR_DATA *)maxh_emissions.array)[ptr2];
		if(c1.id == c2.id)
		{
			printf("%s %s %u %f %f %f %f\n",
               c1.manufact, c1.model, c1.year, c1.capacity, c1.mileage,
               c1.price, c1.emissions);
			   ptr1++;
			   ptr2++;
			   flag=1;
		}
		else if(c1.id < c2.id)
		{
			ptr1++;
		}
		else
		{
			ptr2++;
		}
	}
	if(!flag)
		printf("No qualifying car\n");
	
	//printf("\nprinting car data:\n");
	//print_car_data(car_data, num_cars);
	
	fclose(fp);
	return 0;
}