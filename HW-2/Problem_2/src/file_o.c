#include "file_o.h"
void file_main(void)
{
char filename[20];
printf("\r\nType Filename to create new file\r\n");
scanf("%s", filename);
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
char filename[20];
char a;
printf("\r\nType Filename to open\r\n");
scanf("%s", filename);
ptr=fopen(filename, "w");
printf("\r\nFile should have opened if correct filename specified\r\n");
printf("\r\nEnter a character to write to the file\r\n");
scanf(" %c", &a);
fputc(a,ptr);
printf("\r\ncharacter written and file saved and closed\r\n");
fclose(ptr);
}

void file_append(void)
{
char filename[20];
int a;
printf("\r\nType Filename to open\r\n");
scanf("%s", filename);
ptr=fopen(filename, "a");
printf("\r\nEnter the number of character to be allocated\r\n");
scanf("%d", &a);
char *p = malloc(sizeof(char)* a);
if (p == NULL)
	{
	printf("\r\nMemory allocation failed\r\n");
	}
else
	{	
	printf("\r\nMemory allocation succssfull\r\n");
	}
printf("\r\nEnter the string to be appended in the file\r\n");
scanf("%s", p);
fprintf(ptr,"%s", p);
printf("\r\nString successfully written into the file\r\n");
printf("\r\nFlushing the output file\r\n");
fflush(ptr);
fclose(ptr);
free(p);
}

void file_read(void)
{
char filename[20];
char a;
int r;
printf("\r\nType Filename to read\r\n");
scanf("%s", filename);
ptr=fopen(filename, "r");
printf("\r\nPress c for single character and s for string\r\n");
scanf(" %c",&a);
if(a=='c')
	{
	char r = fgetc(ptr);
	printf("\r\nCharacter is %c\r\n", r);
	}
else if(a == 's')
	{
	printf("\r\nEnter number of Characters to print\r\n");
	scanf("%d", &r);
	char p[r+1];
	fgets(p,r,ptr);
	printf("\r\nString is %s\r\n", p);
	fclose(ptr);
	}
else {
printf("\r\nInvalid option\r\n");
}

}

void file_operations(void)
{
char file[10];
printf("\r\nType newf for new file\r\n");
printf("\r\nType mf to modify file permission\r\n");
printf("\r\nType openf to open existing file\r\n");
printf("\r\nType openfa to open existing file in append mode\r\n");
printf("\r\nType openfr to read existing file\r\n");

scanf("%s",file);

if(strcmp(file,"newf")==0)
{
file_main();
}

if(strcmp(file,"openf")==0)
{
file_open();
}


if(strcmp(file,"openfa")==0)
{
file_append();
}

if(strcmp(file,"openfr")==0)
{
file_read();
}


}
