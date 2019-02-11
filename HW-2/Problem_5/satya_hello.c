/*
Author- Satya Mehta
*Generates Random numbers of 3 digits into array
* Calls a system call numbered 399 which sorts the array
* Prints the sorted array
*/


#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define RAN_NUM 256
#define NSEC_PER_SEC (1000000000)
#define NSEC_PER_MSEC (1000000)
#define NSEC_PER_MICROSEC (1000)

int32_t ran_num = RAN_NUM;
int32_t arr[RAN_NUM];
int32_t sorted[RAN_NUM] = {0};
int32_t temp = 0;

struct timespec start_time = {0, 0};
struct timespec finish_time0 = {0, 0};
struct timespec thread_dt0 = {0, 0};


/*Referred delta_t function code from Real-Time Embedded System Course*/
int delta_t(struct timespec *stop, struct timespec *start, struct timespec *delta_t)
{
  int dt_sec=stop->tv_sec - start->tv_sec;
  int dt_nsec=stop->tv_nsec - start->tv_nsec;

  if(dt_sec >= 0)
  {
    if(dt_nsec >= 0)
    {
      delta_t->tv_sec=dt_sec;
      delta_t->tv_nsec=dt_nsec;
    }
    else
    {
      delta_t->tv_sec=dt_sec-1;
      delta_t->tv_nsec=NSEC_PER_SEC+dt_nsec;
    }
  }
  return(1);
}




int main()
{
	srand(time(0));
	for(int i=0; i<RAN_NUM; i++)
	{
		arr[i] = rand() % 1000;
		printf("Random Numbers %d\n", arr[i]);
	}
	for(int i=0; i<1000; i++);
	
	clock_gettime(CLOCK_REALTIME, &start_time);
	int i = syscall(399,arr,sorted, RAN_NUM);
	clock_gettime(CLOCK_REALTIME, &finish_time0);
	delta_t(&finish_time0, &start_time, &thread_dt0);
	printf("Thread idx=0 timestamp %ld sec, %ld msec (%ld microsec)\n\n",thread_dt0.tv_sec, (thread_dt0.tv_nsec / NSEC_PER_MSEC), (thread_dt0.tv_nsec / NSEC_PER_MICROSEC));
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
	
	for(int i=0; i<1000; i++);
	for(int i=0; i<RAN_NUM; i++)
	{
		printf("Sorted Buffer is%d\n",sorted[i]);
	}
	return 0;
	
}

	
	
