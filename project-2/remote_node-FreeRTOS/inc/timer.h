/**
 * @file timer.h
 * @brief Header file for timer.c.
 *
 * The file consists of all the function declarations, macros and typedefs related to TIMER functionality.
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/12/2019
 *
 * @copyright Copyright (c) 2019
 *
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "inc/main.h"



typedef enum
{
    timer_retry = TIMER0_BASE,
    timer_otp = TIMER1_BASE
}timer_t;


static uint8_t send_failcount;


#define OTP_INPUT_TIME          (120000)    /**< Time to input OTP is in Milliseconds (120 seconds)*/
#define PACKET_RETRY_TIME       (500)       /**< Retry time in Milliseconds*/
#define PACKET_RETRY_COUNT      (3)         /**< Max Packet Retry Counts*/


//******************************************************************************************/
//                              FUNCTION DECLARATIONS
//******************************************************************************************/

/**
 * @brief This function creates, initializes and enables TIMER0A and TIMER1A.
 * @param timer The enum of type timer_t.
 * @param ms Timer Period in milliseconds.
 * @return void
 */
void timer_config(timer_t timer, uint32_t ms);


/**
 * @brief This function enbale the Timer Interrupts.
 * @param timer The enum of type timer_t.
 * @return void.
 */
void enable_timer_int(timer_t timer);

/**
 * @brief This function disables the timer and resets it.
 * @param timer The enum of type timer_t.
 * @return void
 */
inline void disable_timer(timer_t timer);


/**
 * @brief This function resets the timer to the specified time.
 * @param timer The enum of type timer_t.
 * @param ms Timer Period in milliseconds.
 * @return void.
 */
inline void reset_timer(timer_t timer, uint32_t ms);


/**
 * @brief Callback Function for Temperature Timer(TIMER0A).
 * @return void.
 */
void timer0handler(void);


/**
 * @brief Callback Function for LED Timer(TIMER0A).
 * @return void.
 */
void timer1handler(void);


#endif /* INC_TIMER_H_ */
