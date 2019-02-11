#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio_ext.h>

FILE *ptr;
char filename[20] = "Satya.txt";
void print_string()
{
	printf("Hi Welcome to AESD");
}

void file_main(void)
{
	ptr=fopen(filename, "w"); //create a file and modify permission to write
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

	ptr=fopen(filename, "a");
	char *p = malloc(50*(sizeof(char*)));
	if (p == NULL)
	{
		printf("\r\nMemory allocation failed\r\n");
	}
	else
	{	
		printf("\r\nMemory allocation succssfull\r\n");
	}
	printf("\r\nWrite a string to the allocated memory\r\n");
	fgets(p, 20, stdin);
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
	fgets(p,20,ptr);
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
	return 0;
}

