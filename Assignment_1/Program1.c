// Question 2b

#include <stdio.h> 
#include <string.h>
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <sys/time.h> 

int main() 
{

	key_t key = ftok("shmfile",65);	//Generates unique key 
  
	int shmid = shmget(key,1024,0666|IPC_CREAT);	//Returns an identifier in shmid 
  
	char *str = (char*) shmat(shmid,(void*)0,0);	//Attach to shared memory

	while(1)
	{
		printf("Enter Line : "); 
		scanf("%[^\n]%*c", str);
	}   
      
	shmdt(str);	//detach from shared memory   
  
	return 0; 
} 
