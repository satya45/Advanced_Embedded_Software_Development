/**************************
 * Inter process communication using Shared memory
 * Author @Satya Mehta
 * This is consumer code and should be executed after the producer for 
 * successfull operation.
 *
*/

#include <stdio.h> 
#include <stdint.h>
#include <string.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <string.h> 
#include <sys/wait.h> 
#include <time.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>



#define LED_ON (1)
#define LED_OFF (0)


const char *name = "/shared_mem";
const char *sem_p = "sem_p";
const char *sem_c = "sema_c";


struct commands
{
    char string[30];
    int length;
    uint32_t led;

};

char *my_data;
char rc_buff[100];
int flag = 0;
char *strings[5] = {"Hello", "World", "LED Control", "I am user"};
FILE *fptr;
struct timespec timestamp;


void handler(int signo, siginfo_t *info, void *extra) 
{
	printf("Thread exited\n");
    clock_gettime(CLOCK_REALTIME, &timestamp);
	unsigned long int timevalue =  timestamp.tv_sec*1000;
	timevalue += timestamp.tv_nsec/1000000;
    fptr = fopen("consumer_log.txt", "a");
    fprintf(fptr,"TimeStamp: - %ld and Thread EXITED \n", timevalue);
    fclose(fptr);
    if (0 > sem_unlink(sem_p)) 
    {
		perror("semaphore unlink issue");
    }
	if (0 > sem_unlink(sem_c))
    {
        perror("unlink issue");
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



int main(int argc, char *argv[])
{
    FILE *fptr;
    fptr = fopen("consumer_log.txt", "a");
    fclose(fptr);
    set_sig_handler();
    srand(time(0));
    int random = 0;
    const int SIZE = 4096;
    struct commands x;
    struct commands y = {0};
    int shm_fd;
    int led;
    void *ptr;
    char *received = (char *)&y;
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SIZE);
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if(ptr < 0)
    {
        perror("Maping failed");
        exit(0);
    }
    sem_t *semp = sem_open(sem_p, O_CREAT, 0666, 0);

	sem_t *semc = sem_open(sem_c, O_CREAT, 0666, 0);
	if(semc == SEM_FAILED){
		perror("ERROR: sem_open");
		exit(1);
	}

	if(0 > sem_wait(semp))
    perror("ERROR: sem_wait");
    for(int i=0; i<10; i++)
    {
    clock_gettime(CLOCK_REALTIME, &timestamp);
	unsigned long int timevalue =  timestamp.tv_sec*1000;
	timevalue += timestamp.tv_nsec/1000000; 
    memcpy(received, (char *)ptr, sizeof(struct commands ));
    fptr = fopen("consumer_log.txt", "a");
    fprintf(fptr,"TimeStamp: - %ld and message received from producer %s and length  %d\n", timevalue, y.string, y.length);
    fclose(fptr);
    printf("%s\n", y.string);
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
        x.led = LED_ON; //send LED_ON request
        strcpy(x.string, strings[3]);
        x.length = strlen(strings[3]);
    }
    if(random == 5)
    {
        x.led = LED_OFF; //send LED_ON request
        strcpy(x.string, strings[3]);
        x.length = strlen(strings[3]);
    }
    printf("%s\n", x.string);
    clock_gettime(CLOCK_REALTIME, &timestamp);
    timevalue =  timestamp.tv_sec*1000;
    timevalue += timestamp.tv_nsec/1000000;
    memcpy((struct commands *)ptr, &x, sizeof(struct commands));
    fptr = fopen("consumer_log.txt", "a");
    fprintf(fptr,"TimeStamp: - %ld and message sent from consumer to producer %s and length %d\n", timevalue, x.string, x.length);
    fclose(fptr);

    if(0 > sem_post(semc))
	perror("ERROR: sem_post consumer");
    }

    if(-1 == sem_unlink(sem_p))
		perror("ERROR: sem_unlink");

	if(-1 == sem_unlink(sem_c))
		perror("ERROR: sem_unlink");
    printf("Cosumer exiting");
    shm_unlink(name);

}
