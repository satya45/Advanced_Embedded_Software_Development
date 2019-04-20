#Makefile
#Author Satya Mehta

CC=arm-linux-gnueabihf-gcc -g
#CC=gcc -g
my_threads: my_threads.o
	$(CC) -o my_threads my_threads.o -lpthread -lrt

my_threads.o: my_threads.c
	$(CC) -c my_threads.c

clean: 
	rm -f my_threads my_threads.o
