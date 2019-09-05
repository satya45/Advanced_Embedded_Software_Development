/**
 * @file myuart.h
 * @brief Header file for myuart.c.
 *
 * The file consists of all the function declarations, macros and datatype declarations required for UART Communication.
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/22/2019
 *
 * @copyright Copyright (c) 2019
 *
 */


#ifndef INC_MYUART_H_
#define INC_MYUART_H_


#include "inc/main.h"



#define BAUDRATE_FP         (9600)



//******************************************************************************************/
//                          VARIABLE DECLARATIONS
//******************************************************************************************/
typedef enum
{
   UART0 = UART0_BASE,
   UART1 = UART1_BASE,
   UART_FP = UART2_BASE,
   UART3 = UART3_BASE,
   UART4 = UART4_BASE,
   UART5 = UART5_BASE,
   UART_BBG = UART6_BASE,
   UART7 = UART7_BASE

}uart_t;



//******************************************************************************************/
//                           FUNCTION DECLARATIONS
//******************************************************************************************/

/**
 * @brief Configures the Particular UART required.
 * @param uart The UART line used.
 * @param clock The clock used for the peripheral. The macro is defined is CLOCK IN main.h.
 * @param baudrate The baudrate defined for the peripheral.
 * @param irq = TRUE: interrupt functionality enabled for UART, FALSE: Interrupt functionality disabled.
 * @return void.
 */
void uart_configure(uart_t uart, uint32_t clock, uint32_t baudrate, bool irq);



/**
 * @brief This function flushes the UART RX FIFO.
 * @param uart The UART line used.
 * @return void.
 */
void uart_comm_flushRX(uart_t uart);



/**
 * @brief This function flushes the UART TX FIFO.
 * @param uart The UART line used.
 * @return void.
 * @note The UART needs to be enabled again after using this function for the UART to function again.
 * @see UARTEnable().
 */
void uart_comm_flushTX(uart_t uart);



/**
 * @brief This function sends data over UART.
 * @param uart The UART line being used.
 * @param value The value to be sent on UART.
 * @return void.
 */
void uart_send(uart_t uart, uint8_t value);


/**
 * @brief This function checks is the specified UART line is busy and waits till it is free.
 * @param uart The UART line used.
 * @return void.
 */
inline void uart_busy(uart_t uart)
{
    while(UARTBusy(uart));
}



#endif /* INC_MYUART_H_ */
