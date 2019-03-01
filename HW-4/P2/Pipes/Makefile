#Makefile
#Author Satya Mehta

CC=arm-linux-gcc -g
#CC=gcc -g
ipc: ipc.o
	$(CC) -o ipc ipc.o

ipc.o: ipc.c
	$(CC) -c ipc.c

clean: 
	rm -f ipc ipc.o
