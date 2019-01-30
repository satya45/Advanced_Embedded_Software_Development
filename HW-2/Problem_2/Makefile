#Makefile
#Author Satya Mehta

CC = gcc

CFLAGS = -Wall\
	-I inc

vpath %.c src

SRC := main.c print_string.c help.c file_o.c

OBJ := $(SRC:.c=.o)

final: $(OBJ)
	$(CC) -o final $(OBJ) $(CFLAGS)

clean:
	rm -f *.o final
