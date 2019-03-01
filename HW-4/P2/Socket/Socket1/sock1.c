/*Inter process communication using sockets.
Author: Satya Mehta. 
This is a client. 
Referred from:- http://www.it.uom.gr/teaching/distrubutedSite/dsIdaLiu/labs/lab2_1/sockets.html 
*/

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
#include <signal.h>

#define PORT 3124 /* server's port number */
#define MAX_SIZE 100

#define LED_OFF 0
#define LED_ON 1
struct timespec timestamp;
uint8_t control[2] = {0,1};
uint8_t j,k,l; 
int len;
useconds_t usec = 10000;
struct sockaddr_in client_addr;
char *serv_host = "localhost";
struct hostent *hptr;
int buff_size = 0;
int client_fd, client_f, clilen, port;
char string[MAX_SIZE];
int flag = 0;
FILE *fptr;
char *strings[6] = {"Hello", "World", "LED Control", "I am user", "LED ON", "LED OFF"};

struct commands{

    char *string;
    int led;
};

void handler(int signo, siginfo_t *info, void *extra) 
{
	clock_gettime(CLOCK_REALTIME, &timestamp);
	unsigned long int timevalue =  timestamp.tv_sec*1000;
	timevalue += timestamp.tv_nsec/1000000;
	fptr = fopen("socket_client.txt", "a");
    fprintf(fptr,"Timestamp : %ld Thread EXITING \n",timevalue);
    fclose(fptr);
	close(client_fd);
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

void send_message()
{
    struct commands x;
    x.led = LED_OFF;
    int random = 0;
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
            x.string = strings[4]; 
        }
        if(random == 5)
        {
            x.string = strings[5];
        }
        for(int j=0; j<100000; j++); 
		clock_gettime(CLOCK_REALTIME, &timestamp);
	    unsigned long int timevalue =  timestamp.tv_sec*1000;
	    timevalue += timestamp.tv_nsec/1000000;
		fptr = fopen("socket_client.txt", "a");
        fprintf(fptr,"Timestamp : %ld Sending from client to server %s\n",timevalue, x.string);
        fclose(fptr);
        write(client_fd, x.string, 40);
		usleep(usec);
    }

}

int main(int argc, char* argv[])
{

	srand(time(0));
	set_sig_handler();
	port = PORT;
	if((hptr = gethostbyname(serv_host)) == NULL) 
	{
		perror("gethostbyname error");
		exit(1);
	}
  
  	if(hptr->h_addrtype !=  AF_INET) 
	{
		perror("unknown address type");
		exit(1);
	}

	bzero((char *) &client_addr, sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = ((struct in_addr *)hptr->h_addr_list[0])->s_addr;
	client_addr.sin_port = htons(port);

	if((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("CAN'T OPEN SOCKET");
		exit(EXIT_FAILURE);
	}
    
	if(connect(client_fd, (struct sockaddr *) &client_addr, sizeof(client_addr)) < 0) 		
	{
		perror("can't connect to server");
		exit(1);
	}
    send_message();

	for(int i=0; i<10; i++)
	{
		len = read(client_fd, string, MAX_SIZE);	
		if (len < 0)
		{
			printf("\nReceiving Read failed\n");
		}
		clock_gettime(CLOCK_REALTIME, &timestamp);
	    unsigned long int timevalue =  timestamp.tv_sec*1000;
	    timevalue += timestamp.tv_nsec/1000000;
		fptr = fopen("socket_client.txt", "a");
        fprintf(fptr,"Timestamp : %ld Received  from  client: %s\n",timevalue, string);
        fclose(fptr);
		printf("%s\n", string);
	}
	close(client_fd);
}
