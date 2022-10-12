#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int *numbers;
int size;
double mean = 0.0;
double stdev = 0.0;
int numelements = 0;
int min = 0;
int max = 0;
int range = 0;

void *NumElements(void *param);
void *Mean(void *param);
void *Min(void *param);
void *Max(void *param);
void *Range(void *param);
void *StDev(void *param);

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		fprintf(stderr, "usage: a.out <integer value>\n");
        	return -1;
	}		
		
	--argc;
	++argv;

	numbers = calloc(argc, sizeof(int));

	for(int i=0; i<argc; i++)
	{
		numbers[i] = atoi(argv[i]);
		size++;
	}
	
	pthread_t calcNumElements;
	pthread_t calcMean;
	pthread_t calcMin;
	pthread_t calcMax;
	pthread_t calcRange;
	pthread_t calcStDev;

	pthread_attr_t attr;
	pthread_attr_init(&attr);

	pthread_create(&calcNumElements, &attr, NumElements, numbers);
	pthread_create(&calcMean, &attr, Mean, numbers);
	pthread_create(&calcMin, &attr, Min, numbers);
	pthread_create(&calcMax, &attr, Max, numbers);
	pthread_create(&calcRange, &attr, Range, numbers);
	pthread_create(&calcStDev, &attr, StDev, numbers);

	pthread_join(calcNumElements, NULL);
	pthread_join(calcMean, NULL);
	pthread_join(calcMin, NULL);
	pthread_join(calcMax, NULL);
	pthread_join(calcRange, NULL);
	pthread_join(calcStDev, NULL);

	printf("\n Number of Elements = %d \n",numelements);
	printf("\n Mean = %f \n",mean);
	printf("\n Min = %d \n",min);
	printf("\n Max = %d \n",max);
	printf("\n Range = %d \n",range);
	printf("\n Std Deviation = %f \n",stdev);
}

void *NumElements(void *param)
{
	int *numbers;

	numelements = size;

	return (void *)0;
}

void *Mean(void *param)
{
	int sum = 0;
	int *numbers;

	numbers = param;

	for(int i=0; i<size; i++)
		sum += numbers[i];

	mean = sum/((double)size);

	return (void *)0;
}

void *Min(void *param)
{
	int *numbers;
	numbers = param;
	min = numbers[0];
	
	for(int i=0; i<size; i++)
	{
		if(numbers[i]<min)
		min = numbers[i];
	}
	
	return (void *)0;
}

void *Max(void *param)
{
	int *numbers;
	numbers = param;
	max = numbers[0];
	
	for(int i=0; i<size; i++)
	{
		if(numbers[i]>max)
		max = numbers[i];
	}
	
	return (void *)0;
}

void *Range(void *param)
{
	int *numbers;
	numbers = param;
	range = max - min;
	return (void *)0;
}

void *StDev(void *param)
{
	double sqsum = 0;
	int *numbers;

	numbers = param;

	for(int i=0; i<size; i++)
		sqsum += (numbers[i]-mean)*(numbers[i]-mean);

	stdev = sqrt(sqsum/((double)(size-1)));

	return (void *)0;
}
