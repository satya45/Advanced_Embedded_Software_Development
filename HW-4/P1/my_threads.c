/****************************************************************************************************************************************
*				File Name : USING POSIX THREADS and TIMER				
				Author : Satya Mehta	
				Two threads are created one counts alphapbets from a file and second thread checks
				cpu utilization every time interval

*/
#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <stdint.h>
#include <ctype.h>
#include <sys/time.h>

#define THREADS 2
#define NSEC_PER_SEC (1000000000)
#define NSEC_PER_MSEC (1000000)
#define NSEC_PER_MICROSEC (1000)


/*Global variables*/
pthread_t my_thread[THREADS];
pthread_attr_t my_attributes;
pid_t main_pid;
pid_t t1_tid, t2_tid;
unsigned long int t1_pid, t2_pid;
char *cmd  = "cat /proc/stat | grep 'cpu'";
pthread_mutex_t lock;
static timer_t timerid;
struct itimerspec trigger;
struct timespec start_time = {0, 0};
struct timespec finish_time0 = {0, 0};
struct timespec thread_dt0 = {0, 0};
struct timespec timestamp;
clockid_t clk;
struct sigevent sev;
uint8_t a = 97;
uint8_t z = 122;
char cpu[300];
char c;
int flag = 0;
double times;
FILE *files;
uint8_t j, k;
uint32_t arr[25] = {0};
struct file
{
	char *filename;
};

/*Referred from Real-Time Embedded Systems course*/ 
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


void mask_sig(void)
{
	sigset_t mask;
	sigemptyset(&mask); 
  sigaddset(&mask, SIGUSR1);       
  pthread_sigmask(SIG_BLOCK, &mask, NULL);
        
}

void thread_handler(union sigval sv)
{
	char *new = (char*)sv.sival_ptr;
	files = popen(cmd, "r");
	if(files == NULL)
	{
		perror("Popen failed");
		exit(EXIT_FAILURE);
	}
	fread(cpu, sizeof(cpu), 1, files);
	pclose(files);
	clock_gettime(CLOCK_REALTIME, &timestamp);
	unsigned long int timevalue =  timestamp.tv_sec*1000;
	timevalue += timestamp.tv_nsec/1000000;
	pthread_mutex_lock(&lock);
	files = fopen(new, "a");
	fprintf(files,"\n***************************CPU Utilization*************************************\n timestamp: %ld\n%s\n",timevalue,cpu);
	fclose(files);
	pthread_mutex_unlock(&lock);


}

void handler(int signo, siginfo_t *info, void *extra) 
{
	printf("Thread exited\n");
	pthread_cancel(my_thread[1]);
	flag = 1;
}

void *thread1(void* thread)
{	
	mask_sig();
	time_t r_time;
	struct tm *timeinfo;
	time(&r_time);
	timeinfo = localtime(&r_time);
	clock_gettime(CLOCK_REALTIME, &start_time);
	struct file *x = (struct file *)thread; 
	FILE *fptr;
	fptr = fopen("gdb.txt", "r");
	//while((c = fgetc(fptr)) != '\0')
	while(!feof(fptr))
	{
		//printf("%c", c);
		c = fgetc(fptr);
		c = tolower(c);
		if((uint8_t)c >= a && (uint8_t)c<=z)
		{	
			j = (uint8_t)c - a;
			arr[j]++;
		}
	}
	fclose(fptr);
	for(k=0; k<26; k++)
	{
		if(arr[k] < 100)
		{
			printf("Character %c and its count: %d\n", (char)(k+a), arr[k]);
			pthread_mutex_lock(&lock);
			files = fopen(x->filename, "a");
			fprintf(files,"Time stamp: %s\nCharacter %c and its count: %d\n",asctime(timeinfo), (char)(k+a), arr[k]);
			pthread_mutex_unlock(&lock);
			fclose(files);
		}
	}
	t1_tid = syscall(SYS_gettid);
	t1_pid = pthread_self();
	printf("Linux thread id: %d and Posix thread id: %ld\n", t1_tid, t1_pid);
	clock_gettime(CLOCK_REALTIME, &finish_time0);
	delta_t(&finish_time0, &start_time, &thread_dt0);
	printf("Thread idx=0 timestamp %ld sec, %ld msec (%ld microsec)\n\n",thread_dt0.tv_sec, (thread_dt0.tv_nsec / NSEC_PER_MSEC), (thread_dt0.tv_nsec / NSEC_PER_MICROSEC));
	
}

void *thread2(void* thread)
{	
	struct file *x = (struct file *)thread; 
	files = fopen(x->filename, "a");
	t2_tid = syscall(SYS_gettid);
	t2_pid = pthread_self();
	fprintf(files, "Linux thread id : %d and Posix thread id: %ld\n", t2_tid, t2_pid);
	fclose(files);
	trigger.it_interval.tv_sec = 2;
	//trigger.it_value.tv_nsec = 100000000;
	timer_settime(timerid, 0, &trigger, NULL);
	while(1)
	{
		if(flag == 1)
		{
			files = fopen(x->filename, "a");
			fprintf(files,"Thread 2 exited successfully");
			fclose(files);
			pthread_cancel(my_thread[2]);
			timer_delete(timerid);
		}
		sleep(5);
	}
	
}

void set_sig_handler(void)
{
	struct sigaction action;
	action.sa_flags = SA_SIGINFO; 
	action.sa_sigaction = handler;
	if (sigaction(SIGUSR1, &action, NULL) == -1)
	{ 
		perror("sigusr: sigaction");
		exit(1);
	} 
	if (sigaction(SIGUSR2, &action, NULL) == -1)
	{ 
		perror("sigusr: sigaction");
		exit(1);
	} 
}

int main(int argc, char *argv[])
{
	int i;
	struct file f;
	f.filename  = argv[1];
	set_sig_handler();
	main_pid = getpid();
	sev.sigev_notify = SIGEV_THREAD;
  sev.sigev_notify_function = &thread_handler;
  sev.sigev_value.sival_ptr = argv[1];
	if(timer_create(CLOCK_REALTIME, &sev, &timerid))
	{
		perror("Timer creation failed");
		exit(EXIT_FAILURE);
	}
	trigger.it_value.tv_sec = 2;
	//trigger.it_value.tv_nsec = 100000000;
	printf("Main thread and its id : %d\n", main_pid);
	if(pthread_mutex_init(&lock, NULL))
	{
		perror("Mutex initialization failed");
		exit(1);
	}	
	if(pthread_create(&my_thread[0],   // pointer to thread descriptor
                  (void *)&my_attributes,     // use default attributes
                  thread1,// thread function entry point
                  (void *)&f // parameters to pass in
				  ))
	{
		perror("Pthread 1 create failed");
		exit(1);
		
	}
	if(pthread_create(&my_thread[1],   // pointer to thread descriptor
                  (void *)&my_attributes,     // use default attributes
                  thread2, // thread function entry point
                  (void *)&f // parameters to pass in
				  ))
	{
		perror("Pthread 2 create failed");
		exit(1);
	}
	for(i=0; i<THREADS;i++)
	{
		pthread_join(my_thread[i], NULL);
	}
	pthread_mutex_destroy(&lock);
	
}
