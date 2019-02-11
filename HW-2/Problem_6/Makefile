#Makefile
#Author Satya Mehta

#CC=arm-linux-gcc
CC=gcc
cron: cron.o
	$(CC) -o cron cron.o

cron.o: cron.c
	$(CC) -c cron.c

clean: 
	rm -f cron cron.o
