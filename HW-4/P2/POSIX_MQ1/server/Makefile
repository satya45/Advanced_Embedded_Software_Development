#Makefile
#Author Satya Mehta

#CC=arm-linux-gcc -g
CC=gcc -g
##posix_q: posix_q.o
#	$(CC) -o posix_q posix_q.o -lrt

posix_server: posix_server.o
	$(CC) -o posix_server posix_server.o -lrt

#posix_q.o: posix_q.c
#$(CC) -c posix_q.c


posix_server.o: posix_server.c
	$(CC) -c posix_server.c

clean: 
	rm -f posix_q posix_q.o posix_server posix_server.o
