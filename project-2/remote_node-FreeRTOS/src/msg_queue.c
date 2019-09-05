/**
 * @file msg_queue.c
 * @brief This file consists of all the functions required for creating and implementing message queues.
 *
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/12/2019
 *
 * @copyright Copyright (c) 2019
 *
 */

#include "inc/msg_queue.h"


void queue_create(void)
{
    //Creating a message queue
    nrf_log_mq = xQueueCreate(MQ_SIZE, sizeof(event));
    {
        if( nrf_log_mq == NULL )
        {
            UARTprintf("\n\nMessage Queue not created.\n");
            exit(EXIT_FAILURE);
        }
    }

}

