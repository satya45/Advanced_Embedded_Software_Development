/**
 * @file sync_objects.h
 * @brief Header file for sync_objects.c.
 *
 * The file consists of all the function declarations, macros and typedefs related to logging functionality.
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/13/2019
 *
 * @copyright Copyright (c) 2019
 *
 */

#ifndef INC_SYNC_OBJECTS_H_
#define INC_SYNC_OBJECTS_H_

#include "inc/main.h"



//******************************************************************************************/
//                              MUTEX HANDLES
//******************************************************************************************/
xSemaphoreHandle g_uartsem;         /**< Mutex for UART*/
xSemaphoreHandle g_qsem;            /**< Mutex to protect queue writes from simultaneous access from two threads*/

//******************************************************************************************/
//                              SEMAPHORE HANDLES
//******************************************************************************************/
xSemaphoreHandle g_temp;            /**< Semaphore to release Temperature Task*/
xSemaphoreHandle g_led;             /**< Semaphore to release LED Task*/


//******************************************************************************************/
//                              FUNCTION DECLARATIONS
//******************************************************************************************/

/**
 * @brief This function creates the required Semaphores.
 * @return void.
 */
void sem_create(void);


/**
 * @brief This function creates the required Mutexes.
 * @return void
 */
void mutex_create(void);


#endif /* INC_SYNC_OBJECTS_H_ */
