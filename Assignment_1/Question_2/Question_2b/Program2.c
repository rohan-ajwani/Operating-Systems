// Question 2b

#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <string.h>
#include <sys/time.h>

int main() 
{
	key_t key = ftok("shmfile",65);	//Generates unique key
  
	int shmid = shmget(key,1024,0666|IPC_CREAT);	//Returns an identifier in shmid 
   
	char *str = (char*) shmat(shmid,(void*)0,0);	//Attach to shared memory

	while(1)
	{
		sleep(3);

		int vowels=0, consonants=0, digits=0, words=0;

		for (int i = 0; str[i] != '\0'; ++i) 
		{
			if (str[i] == 'a' || str[i] == 'e' || str[i] == 'i' ||
			str[i] == 'o' || str[i] == 'u' || str[i] == 'A' ||
			str[i] == 'E' || str[i] == 'I' || str[i] == 'O' ||
			str[i] == 'U') 
			{
				++vowels;
			} 
			else if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z')) 
			{
				++consonants;
			}
			else if (str[i] >= '0' && str[i] <= '9') 
			{
				++digits;
			}
			else if (str[i] == ' ')
			{
				++words;
			}
		}
	
		printf("Vowels: %d", vowels);
		printf("\nConsonants: %d", consonants);
		printf("\nDigits: %d", digits);
		printf("\nWords: %d", (words+1));
		printf("\n\n");

		sleep(2);
	}
      
	shmdt(str); 	//detach from shared memory  
 
	shmctl(shmid,IPC_RMID,NULL);	// destroy the shared memory 
     
	return 0; 
} 

