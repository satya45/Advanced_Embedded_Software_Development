/**
 * @file msg_queue.h
 * @brief Header file for msg_queue.c.
 *
 * The file consists of all the function declarations, macros and typedefs related to message queue functionality.
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/12/2019
 *
 * @copyright Copyright (c) 2019
 *
 */


#ifndef INC_MSG_QUEUE_H_
#define INC_MSG_QUEUE_H_

#include "inc/main.h"

//******************************************************************************************/
//                                  MESSAGE QUEUE SIZE
//******************************************************************************************/
#define MQ_SIZE         (10)           /**< Size of the message queue*/


//******************************************************************************************/
//                                  MESSAGE QUEUE HANDLE
//******************************************************************************************/
QueueHandle_t nrf_log_mq;



//******************************************************************************************/
//                              FUNCTION DECLARATIONS
//******************************************************************************************/

/**
 * @brief This function creates the required Message Queues.
 * @return void
 */
void queue_create(void);

#endif /* INC_MSG_QUEUE_H_ */
