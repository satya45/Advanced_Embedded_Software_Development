/**
 * @file sync_objects.c
 * @brief This file consists of all the functions related to the synchronization primitives such as Semaphores and Mutexes.
 *
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/13/2019
 *
 * @copyright Copyright (c) 2019
 *
 */

#include "inc/sync_objects.h"


void sem_create(void)
{
    //Creating Temperature Semaphore
    g_temp = xSemaphoreCreateBinary();
    if(g_temp == NULL)
    {
        UARTprintf("\n\nTemperature Semaphore not created.\n");
        exit(EXIT_FAILURE);
    }

    //Creating LED Semaphore
    g_led = xSemaphoreCreateBinary();
    if(g_led == NULL)
    {
        UARTprintf("\n\nLED Semaphore not created.\n");
        exit(EXIT_FAILURE);
    }

}


void mutex_create(void)
{
    //Creating Mutex for UART
    g_uartsem = xSemaphoreCreateMutex();
    if(g_uartsem == NULL)
    {
        UARTprintf("\n\nUART Mutex not created.\n");
        exit(EXIT_FAILURE);
    }

    g_qsem = xSemaphoreCreateMutex();
    if(g_qsem == NULL)
    {
        UARTprintf("\n\nQueue Mutex not created.\n");
        exit(EXIT_FAILURE);
    }

}


