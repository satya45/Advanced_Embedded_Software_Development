/**
 * @file logger.h
 * @brief Header file for logger.c.
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

#ifndef INC_LOGGER_H_
#define INC_LOGGER_H_

#include "inc/main.h"



//******************************************************************************************/
//                              FUNCTION DECLARATIONS
//******************************************************************************************/

/**
 * @brief The function creates the Logger Task.
 * @return uint8_t Returns if the Task initialization is successful.
 */
uint8_t nrf_loggertask_init(void);


/**
 * @brief The Callback function for Logger Task.
 * @param pvParameters Not in use.
 */
static void nrf_logger(void *pvParameters);


/**
 * @brief This function receives all the data from other tasks via message queues and logs the data to the host console.
 * @return void.
 */
void nrf_log(void);


#endif /* INC_LOGGER_H_ */
