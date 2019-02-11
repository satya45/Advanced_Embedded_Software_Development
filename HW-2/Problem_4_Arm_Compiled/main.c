/*
Author- Satya Mehta
*Main Program for File IO Operation
* File Main-Creates a file with write permission.
* File Open - Opens a file and writes Character F
* File Append - Opens a file in append mode, allocates a memory, writes a string and flushes the file
* File read - Opens a file, reads a character and reads a string

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *ptr;
char filename[20] = "Satya.txt";
void print_string()
{
	printf("Hi Welcome to AESD");
}

void file_main(void)
{
	ptr=fopen(filename, "w");
	printf("\r\nNew File created with name %s\r\n", filename);
	if(ptr == NULL)
	{
		printf("\r\nFile Creation failed\r\n");
		return;
	}

}

void file_open(void)
{
	char a = 'F';
	ptr=fopen(filename, "w");
	fputc(a,ptr);
	printf("\r\ncharacter written and file saved and closed\r\n");
	fclose(ptr);
}

void file_append(void)
{
	int a=50;
	ptr=fopen(filename, "a");
	char *p = malloc(sizeof(char)* a);
	if (p == NULL)
	{
		printf("\r\nMemory allocation failed\r\n");
	}
	else
	{	
		printf("\r\nMemory allocation succssfull\r\n");
	}
	printf("\r\nWrite a string to the allocated memory\r\n");
	scanf("%s", p);
	//strcpy(p, "SatyaMehta");
	fprintf(ptr,"%s", p);
	printf("\r\nString successfully written into the file\r\n");
	printf("\r\nFlushing the output file\r\n");
	fflush(ptr);
	fclose(ptr);
	free(p);
}

void file_read(void)
{
	
	char a;
	int x = 10;
	ptr=fopen(filename, "r");
	char r = fgetc(ptr);
	printf("\r\nCharacter is %c\r\n", r);
	char p[x+1];
	fgets(p,r,ptr);
	printf("\r\nString is %s\r\n", p);
	fclose(ptr);
	
}

int main()
{
	print_string();
	file_main();
	file_open();
	file_append();
	file_read();
}

