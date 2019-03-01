/****************************************************************************************************************************************
*				File Name : Inter Process Communication using PIPES		
				Author : Satya Mehta	
                Referred signals part from http://devarea.com/linux-handling-signals-in-a-multithreaded-application/#.XHR3doVlDrI
*/

#include <stdio.h> 
#include <stdint.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <string.h> 
#include <sys/wait.h> 
#include <time.h>


#define LED_ON (1)
#define LED_OFF (0)

struct commands{
    char *string;
    int length;
    uint32_t led;

};

struct file
{
	char *filename;
};

FILE *fptr;
int flag = 0;
void handler(int signo, siginfo_t *info, void *extra) 
{
	printf("Thread exited\n");
    fclose(fptr);
	flag = 1;
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

struct file f;
char *my_data;
char rc_buff[100];
int fd[2];
int fd1[2];
char *strings[5] = {"Hello", "World", "LED Control", "I am user"};

struct timespec timestamp;
void send_message()
{
    struct commands x;
    x.led = LED_OFF;
    srand(time(0));
    int random = 0;
    close(fd[0]);
    for(int i=0; i<10; i++)
    {
        random = rand() % 6;
        // printf("Random number : %d\n", random);
        if(random == 0)
        {
        x.string = strings[0];
        }
        if(random == 1)
        {
            x.string = strings[1];   
        }
        if(random == 2)
        {
            x.string = strings[2];
        }
        if(random == 3)
        {
            x.string = strings[3];
        }
        if(random == 4)
        {
            x.led = LED_ON; //send LED_ON request
        }
        if(random == 5)
        {
            x.led = LED_OFF;
        }
        for(int j=0; j<100000; j++); //delay provided to read
        fptr = fopen(f.filename, "a");
        clock_gettime(CLOCK_REALTIME, &timestamp);
	    unsigned long int timevalue =  timestamp.tv_sec*1000;
	    timevalue += timestamp.tv_nsec/1000000;
        fprintf(fptr,"Timestamp: %ld Sent from Parent\nString: %s and string length %ld and LED status: %d\n", timevalue, x.string, strlen(x.string), x.led);
        fclose(fptr);
        write(fd[1], &x, sizeof(x)+1); 
    }
    close(fd[1]);
    
}

void send_message1()
{
    struct commands x;
    x.led = LED_OFF;
    srand(time(0));
    int random = 0;
    close(fd1[0]);
    for(int i=0; i<10; i++)
    {
        random = rand() % 6;
        // printf("Random number : %d\n", random);
        if(random == 0)
        {
        x.string = strings[0];
        }
        if(random == 1)
        {
            x.string = strings[1];   
        }
        if(random == 2)
        {
            x.string = strings[2];
        }
        if(random == 3)
        {
            x.string = strings[3];
        }
        if(random == 4)
        {
            x.led = LED_ON; //send LED_ON request
        }
        if(random == 5)
        {
            x.led = LED_OFF; //send LED_ON request
        }
        for(int j=0; j<1000000; j++); //delay provided to read
        clock_gettime(CLOCK_REALTIME, &timestamp);
	    unsigned long int timevalue =  timestamp.tv_sec*1000;
	    timevalue += timestamp.tv_nsec/1000000;
        fptr = fopen(f.filename, "a");
        fprintf(fptr,"Timestamp: %ld Sent from Child\nString: %s and string length %ld and LED status: %d\n", timevalue, x.string, strlen(x.string), x.led);
        fclose(fptr);
        write(fd1[1], &x, sizeof(x)+1); 
    }
    close(fd1[1]);
    
}

void rc_msg(int pipe)
{   
    struct commands *y;
    if(pipe == 0)
    {
        close(fd[1]);
        while(read(fd[0], rc_buff, sizeof(rc_buff))!=0)
        {
            y = (struct commands *)rc_buff;
            printf("String %s and LED status %d\n", y->string, y->led);
            clock_gettime(CLOCK_REALTIME, &timestamp);
	        unsigned long int timevalue =  timestamp.tv_sec*1000;
	        timevalue += timestamp.tv_nsec/1000000;
            fptr = fopen(f.filename, "a");
            fprintf(fptr,"Timestamp: %ld Received from Parent\nString: %s and string length %ld and LED status: %d\n", timevalue, y->string, strlen(y->string), y->led);
            fclose(fptr);
        }
    }
    if(pipe == 1)
    {
        close(fd[1]);
        while(read(fd1[0], rc_buff, sizeof(rc_buff))!=0)
        {
            y = (struct commands *)rc_buff;
            printf("String %s and LED status %d\n", y->string, y->led);
            clock_gettime(CLOCK_REALTIME, &timestamp);
	        unsigned long int timevalue =  timestamp.tv_sec*1000;
	        timevalue += timestamp.tv_nsec/1000000;
            fptr = fopen(f.filename, "a");
            fprintf(fptr,"Timestamp: %ld Received from Child\nString: %s and string length %ld and LED status: %d\n", timevalue, y->string, strlen(y->string), y->led);
            fclose(fptr);
        }
    }
}

int main(int argc, char *argv[])
{
    pid_t p;
    int flag, flag1;
    f.filename  = argv[1];
    set_sig_handler();
    if (pipe(fd)==-1) 
    { 
        perror("Pipe failed\n");
        return 1; 
    }
    if (pipe(fd1)==-1) 
    { 
        perror("Pipe failed\n");
        return 1; 
    }
    p = fork(); 
    if (p < 0) 
    { 
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    else if (p == 0) 
    { 
        flag = 0;
        send_message(flag); 
        flag = 1;
        rc_msg(flag); 
        while(1);
    }
    else
    { 
        flag1 = 0;
        rc_msg(flag1);
        send_message1();
    } 
    return 0;
}  
