/*Inter process communication using sockets.
Author: Satya Mehta. 
This is a Server. 
Referred from:-http://www.it.uom.gr/teaching/distrubutedSite/dsIdaLiu/labs/lab2_1/sockets.html 
*/


#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>


#define PORT 3124 /* server's port number */
#define MAX_SIZE 100

#define LED_OFF 0
#define LED_ON 1
char *strings[6] = {"Hello", "World", "LED Control", "I am user", "LED ON", "LED OFF"};
struct timespec timestamp;
uint8_t control[2] = {0,1};
uint8_t j,k,l; 
//srand(time(0));
int len;
useconds_t usec = 10000;
struct sockaddr_in serv_addr, client_addr;
struct hostent *hptr;
int buff_size = 0;
int flag = 0;
int serv, ser, client_len, port;
char string[MAX_SIZE];
FILE *fptr;
struct commands x;
struct commands{

    char *string;
    int led;
};

void handler(int signo, siginfo_t *info, void *extra) 
{
	clock_gettime(CLOCK_REALTIME, &timestamp);
	unsigned long int timevalue =  timestamp.tv_sec*1000;
	timevalue += timestamp.tv_nsec/1000000;
	fptr = fopen("socket_server.txt", "a");
    fprintf(fptr,"Timestamp : %ld Thread EXITING \n",timevalue);
    fclose(fptr);
	close(serv);
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
		exit(0);
	} 
}


void send_message()
{
    struct commands x;
    x.led = LED_OFF;
    int random = 0;
    for(int i=0; i<10; i++)
    {
        random = rand() % 6;
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
            x.string = strings[4]; //send LED_ON request
        }
        if(random == 5)
        {
            x.string = strings[5];
        }
        sleep(1);

        clock_gettime(CLOCK_REALTIME, &timestamp);
	    unsigned long int timevalue =  timestamp.tv_sec*1000;
	    timevalue += timestamp.tv_nsec/1000000;
		fptr = fopen("socket_server.txt", "a");
        fprintf(fptr,"Timestamp : %ld Sending from  server to client: %s\n",timevalue, x.string);
        fclose(fptr);
		write(ser, x.string, 10);
		usleep(usec);
    }

}


int main(int argc, char* argv[])
{
	port = PORT;
	set_sig_handler();
	fptr = fopen("socket_server.txt", "a");
	fclose(fptr);
	if((serv = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("SOCKET FAILED");
		exit(EXIT_FAILURE);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);
    
    if(bind(serv, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
	{
		perror("can't bind local address");
		exit(1);
    }

    listen(serv, 5); 
    client_len = sizeof(client_addr);
    ser = accept(serv, (struct sockaddr *) &client_addr, &client_len);
	for(int i=0; i<10; i++)
	{
		len = read(ser, string, MAX_SIZE);		// read a message from the client  
		if (len < 0)
		{
			printf("\nReceiving Read failed\n");
		}
		string[len] = 0;		//make sure it's a proper string
		clock_gettime(CLOCK_REALTIME, &timestamp);
	    unsigned long int timevalue =  timestamp.tv_sec*1000;
	    timevalue += timestamp.tv_nsec/1000000;
		fptr = fopen("socket_server.txt", "a");
        fprintf(fptr,"Timestamp : %ld Received  from  client: %s\n",timevalue, string);
        fclose(fptr);
		printf("%s\n", string);
	}
	send_message();

}