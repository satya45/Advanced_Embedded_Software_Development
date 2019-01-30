/**
​ * ​ ​ @file​ ​main.h
​ * ​ ​ @brief​ ​ Initialization of Lookup Table and some functions.
​ *
​ * ​ ​ @author​ Satya Mehta
​ * ​ ​ @date​ ​ Jan 29 2019
​ * ​ ​ @version​ ​ 1.0
​ *
​ */
#ifndef _MAIN_H
#define _MAIN_H

#include <stdio.h>
#include <stdint.h>
#include "print_string.h"
#include "string.h"
#include "help.h"
#include "file_o.h"

typedef struct
{
 char *cmd;
 void (*myfunc)();
		
}table;

void ex(void);
uint32_t e=1;

#endif
