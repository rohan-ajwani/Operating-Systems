#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>

#define MAX_RESOURCES 5

int available_resources = MAX_RESOURCES;

sem_t mutex;

int main()
{
	if(sem_init(&mutex,1,1)<0)
	{
		perror("Semaphore error");
		return 1;
	}

}

int decrease_count(int count)
{
	while(available_resources<count) //to deal with insufficient resources
		; //busy wait	

	sem_wait(&mutex); // to deal with race condition

	//critical section
	available_resources -= count;

	sem_post(&mutex); // reset mutex to 1 after critical section is over
	
	return 0;
}

int increase_count(int count)
{
	sem_wait(&mutex); // to deal with race condition

	//critical section
	available_resources += count;

	sem_post(&mutex); // reset mutex to 1 after critical section is over

	return 0;	
}

