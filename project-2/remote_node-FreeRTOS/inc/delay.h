/**
 * @file delay.h
 * @brief Header file for delay functions.
 *
 * The file consists of all the function declarations, macros and typedefs related to delay functions.
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo.
 * @date 04/26/2019
 *
 * @copyright Copyright (c) 2019
 *
 */

#ifndef INC_DELAY_H_
#define INC_DELAY_H_

#include "inc/main.h"



inline void delay_ms(uint32_t ms)
{
    SysCtlDelay((g_ui32SysClock/1000)*ms);
}


inline void delay_us(uint32_t us)
{
    SysCtlDelay((g_ui32SysClock/1000000)*us);
}


#endif /* INC_DELAY_H_ */
