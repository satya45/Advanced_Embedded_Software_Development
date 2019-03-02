/**************************
 * Inter process communication using POSIX Queues
 * Author @Satya Mehta
 * Client code which sends messages to server and receives from client.
 * Execute after the client
 *
*/
#include <signal.h>
#include <pthread.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>           
#include <sys/stat.h>        
#include <string.h>
#include <time.h>

#define QUEUE_N "/test1queue"

                      
struct commands{

    char string[32];
	int length;
    int led;
};
mqd_t mq;
FILE *fptr;
struct timespec timestamp;
unsigned long int timevalue;
char *strings[5] = {"Hello", "World", "LED Control", "I am user"};

void handler(int signo, siginfo_t *info, void *extra) 
{
    int rc;
	printf("Thread exited\n");
    clock_gettime(CLOCK_REALTIME, &timestamp);
	unsigned long int timevalue =  timestamp.tv_sec*1000;
	timevalue += timestamp.tv_nsec/1000000;
    fptr = fopen("q_client_log.txt", "a");
    fprintf(fptr,"TimeStamp: - %ld and Thread EXITED \n", timevalue);
    fclose(fptr);
    if(rc = mq_close(mq) < 0)
    {
        perror("Cannot close");
        mq_unlink(QUEUE_N);
        exit(1);
    }
    exit(0);
}

void set_sig_handler(void)
{
	struct sigaction action;
	action.sa_flags = SA_SIGINFO; 
	action.sa_sigaction = handler;
	if (sigaction(SIGINT, &action, NULL) == -1)
	{ 
		perror("sigusr: sigaction");
		exit(1);
	} 
}




int main()
{
    int rc;
	struct mq_attr attr;
	struct commands x;
	struct commands y;
    set_sig_handler();
	attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 50;
    attr.mq_curmsgs = 0;
	strcpy(x.string, strings[0]);
    x.length = strlen(strings[0]);
    x.led = 1;
	mq = mq_open(QUEUE_N, O_RDWR|O_CREAT, 0666, &attr);
    if(mq == -1)
	{
        perror("mq_open");
    }
    for(int i=0; i<10; i++)
	{
		rc = mq_receive(mq, (char*)&y, 50, NULL);
		if(rc == -1)
		{
        	perror("RECEIVE IN SERVER");
		}
        clock_gettime(CLOCK_REALTIME, &timestamp);
	    timevalue =  timestamp.tv_sec*1000;
	    timevalue += timestamp.tv_nsec/1000000; 
        fptr = fopen("q_client_log.txt", "a");
        printf("%s\n", y.string);
        fprintf(fptr,"TimeStamp: - %ld and message received from server %s,  length %d, LED status %d\n", timevalue, y.string, y.length, y.led);
        fclose(fptr);
	}
	for(int i=0; i<10; i++)
    {
        int random;
        random = rand() % 6;
        // printf("Random number : %d\n", random);
        if(random == 0)
        {
            strcpy(x.string, strings[0]);  
            x.led = 0;
            x.length = strlen(strings[0]);
        }
        if(random == 1)
        {
            strcpy(x.string, strings[1]);  
            x.led = 0;
            x.length = strlen(strings[1]);
        }
        if(random == 2)
        {
            strcpy(x.string, strings[2]);
            x.led = 0;
            x.length = strlen(strings[2]);
        }
        if(random == 3)
        {
            strcpy(x.string, strings[3]);
            x.led = 0;
            x.length = strlen(strings[3]);
        }
        if(random == 4)
        {
            x.led = 1; //send LED_ON request
            strcpy(x.string, strings[3]);
            x.length = strlen(strings[3]);
        }
        if(random == 5)
        {
            x.led = 0; //send LED_ON request
            strcpy(x.string, strings[3]);
            x.length = strlen(strings[3]);
        }
        printf("%s\n", x.string);
        clock_gettime(CLOCK_REALTIME, &timestamp);
	    unsigned long int timevalue =  timestamp.tv_sec*1000;
	    timevalue += timestamp.tv_nsec/1000000;
		rc = mq_send(mq, (char*)&x, sizeof(struct commands), 0);
		if(rc == -1)
		{
        	 perror("SENDING FROM SERVER");
		}
        clock_gettime(CLOCK_REALTIME, &timestamp);
	    timevalue =  timestamp.tv_sec*1000;
	    timevalue += timestamp.tv_nsec/1000000; 
        fptr = fopen("q_client_log.txt", "a");
        fprintf(fptr,"TimeStamp: - %ld and message sent from client to server %s,  length %d, LED status %d\n", timevalue, x.string, x.length, x.led);
        fclose(fptr);
        sleep(1);
    }

	return 0;
}