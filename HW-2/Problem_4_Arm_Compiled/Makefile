#Makefile
#Author Satya Mehta

CC=arm-linux-gcc

main: main.o
	$(CC) -o main main.o

main.o: main.c
	$(CC) -c main.c

clean: 
	rm -f main.o main
