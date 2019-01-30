#include <stdio.h>
#include "main.h"

table lookup[]=
{
{"print\n",&print_string},
{"help\n",&help},
{"file\n",&file_operations},
{"ex\n",&ex}

};
char input[20];
void ex(void)
{
e=0;
}


void main()
{
printf("Type help to invoke help function");


	while(e)
	{
	 	fgets(input, 20, stdin);
		for (int i=0; i<sizeof(lookup)/sizeof(lookup[0]); i++)
		{ 
			if (strcmp(input,lookup[i].cmd)==0)
			{
				(*lookup[i].myfunc)();
			}

		}


	}

}
