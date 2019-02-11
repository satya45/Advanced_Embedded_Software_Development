#Makefile
#Author Satya Mehta

CC=arm-linux-gcc
#CC=gcc
satya_hello: satya_hello.o
	$(CC) -o satya_hello satya_hello.o

satya_hello.o: satya_hello.c
	$(CC) -c satya_hello.c

clean: 
	rm -f satya_hello satya_hello.o
