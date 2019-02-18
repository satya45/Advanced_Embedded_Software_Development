#Makefile
#Author Satya Mehta

#CC= gcc
CC = arm-linux-gcc -g

main: main.o
	$(CC) -o main main.o

main.o: main.c
	$(CC) -c main.c

clean: 
	rm -f main.o main
