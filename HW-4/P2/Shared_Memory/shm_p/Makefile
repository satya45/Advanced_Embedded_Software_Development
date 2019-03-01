#Makefile
#Author Satya Mehta

#CC=arm-linux-gcc -g
CC=gcc -g
shm_p: shm_p.o
	$(CC) -o shm_p shm_p.o -lrt -lpthread

shm_p.o: shm_p.c
	$(CC) -c shm_p.c

clean: 
	rm -f shm_p shm_p.o
