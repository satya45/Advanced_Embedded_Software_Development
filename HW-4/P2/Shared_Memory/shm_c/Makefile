#Makefile
#Author Satya Mehta

#CC=arm-linux-gcc -g
CC=gcc -g
shm_c: shm_c.o
	$(CC) -o shm_c shm_c.o -lrt -lpthread

shm_c.o: shm_c.c
	$(CC) -c shm_c.c

clean: 
	rm -f shm_c shm_c.o
