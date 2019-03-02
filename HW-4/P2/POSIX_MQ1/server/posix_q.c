#include <fcntl.h>
#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>

#define QUEUE_NAME  "/testqueue"
#define MAX_SIZE    1024

struct commands
{
    char *string;
    int length;
    uint32_t led;

};

struct file
{
	char *filename;
};

struct file f;
char *my_data;
char rc_buff[100];
int flag = 0;
char *strings[5] = {"Hello", "World", "LED Control", "I am user"};
FILE *fptr;
struct timespec timestamp;


int main()
{
    mqd_t mq;
char buffer[MAX_SIZE];
struct commands *x;
struct mq_attr attr;
unsigned int sender;
attr.mq_flags = 0;
attr.mq_maxmsg = 10;
attr.mq_msgsize = MAX_SIZE;
attr.mq_curmsgs = 0;
mq = mq_open(QUEUE_NAME, O_CREAT|O_RDWR, 0666,&attr);
int count = 0;
while(1)
{
//for(int i =0; i<10; i++)
//{
    printf("CLIENT: Send message... \n");
    int r = mq_send(mq, (char *)x, sizeof(struct commands), 0);
    printf("Sent data %d", r);
    sleep(1);
}
sleep(5);
    // for(int i=0; i<10; i++)
    // {
    //     mq_receive(mq, (char *)x, sizeof(struct commands), &sender);
    //     printf("SERVER: Received message: %s\n", x->string);
    //     sleep(1);
    // }
mq_close(mq);
while(1);
    
}