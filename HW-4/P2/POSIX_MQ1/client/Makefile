#Makefile
#Author Satya Mehta

#CC=arm-linux-gcc -g
CC=gcc -g
posix_client: posix_client.o
	$(CC) -o posix_client posix_client.o -lrt -lpthread

posix_client.o: posix_client.c
	$(CC) -c posix_client.c

clean: 
	rm -f posix_client posix_client.o
