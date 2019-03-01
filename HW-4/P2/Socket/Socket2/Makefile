#Makefile
#Author Satya Mehta

#CC=arm-linux-gcc -g
CC=gcc -g
sock2: sock2.o
	$(CC) -o sock2 sock2.o -lrt

sock2.o: sock2.c
	$(CC) -c sock2.c

clean: 
	rm -f sock2 sock2.o
