#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>

int main()
{

	int n = 5;	//no of processes
	int m = 3;	//no of resources

	int allocated[5][3] = {{0,1,0},
				{2,0,0},
				{3,0,2},
				{2,1,1},
				{0,0,2}};

	int max[5][3] = {{7,5,3},
			{3,2,2},
			{9,0,2},
			{2,2,2},
			{4,3,3}};

	int available[3] = {3,3,2};

	int need[n][m];

	for(int j=0; j<n; j++)
	{
		for(int i=0; i<m; i++)
			need[j][i] = max[j][i] - allocated[j][i];
	}

	int ans[5] = {0,0,0,0,0};
	int flag[5] = {0,0,0,0,0};
	int count=0;

	for(int j=0; j<n; j++)
	{
		for(int i=0; i<n; i++)
		{
			if(flag[i]==0)
			{
				if((need[i][0]>available[0])||(need[i][1]>available[1])||(need[i][2]>available[2]))
					flag[i] = 0;
				else
				{
					flag[i] = 1;
					available[0] += allocated[i][0];	//after resources utilized, then freed (available += max-need = allocated)
					available[1] += allocated[i][1];
					available[2] += allocated[i][2];
					ans[j]=i;	//jth step, ith process executed
					break;
				}
			}
		}
	}

	for(int i=0; i<n; i++)
	{
		if(flag[i]==0)
		{
			printf("Deadlock, cannot be executed\n");
			return -1;
		}
	}

	for(int i=0; i<n; i++)
	{
		printf(" P%d\n",ans[i]);
	}
}	


			
		 
