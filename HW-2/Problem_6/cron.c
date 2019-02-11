/*
Author- Satya Mehta
*CRON .c File
* Prints Process ID
* Prints User ID
*Calls System Call
* Prints Current Date and Time
*/


#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdint.h>

#define RAN_NUM 256
#define INTERVAL 10
FILE *ptr;
char filename[20] = "Satya_cron.txt";
struct timeval times;

int32_t ran_num = RAN_NUM;
int32_t arr[RAN_NUM];
int32_t sorted[RAN_NUM] = {0};
int32_t temp = 0;
char buffer[40];
unsigned long timer;
struct timespec ts = {5,0};
int32_t n;
time_t t;
int main()
{
	gettimeofday(&times, NULL);
	printf("%ld", times.tv_sec);

	srand(time(0));
	for(int i=0; i<RAN_NUM; i++)
	{
		
		arr[i] = rand() % 1000;
		printf("Random Numbers %d\n", arr[i]);
	}
	
	n = getpid();
	printf("Process id is %d\n", n);
	n = getuid();
	printf("User id is %d\n", n);
	for(int i=0; i<1000; i++);
	int i = syscall(399,arr,sorted, RAN_NUM);
	if(i == 0)
	{
		printf("System Call successfully executed\n");
		
	}
	else if(i == -2)
	{
	printf("Length Not appropriate%d\n",i);
	} 
		
	else
	{
		printf("System Called failed and returned value is :%d\n", i);
		
	}
	for(int i=0; i<RAN_NUM; i++)
	{
		printf("Sorted Buffer is%d\n",sorted[i]);
	}
	time(&t);
	printf("Data and Time : %s", ctime(&t));		
	
}


	
	
