#include <stdio.h>
#include <unistd.h>
char* paramsList[] = { "/bin/bash", "-c",
                        "/usr/bin/python sms-send.py satya", NULL };


    
int main()
{
    execv("/bin/bash",paramsList); 
    return 0;

}