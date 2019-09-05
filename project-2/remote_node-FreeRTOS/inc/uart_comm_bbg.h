/**
 * @file uart_comm_bbg.h
 * @brief Header file for uart_comm_bbg.c.
 *
 * The file consists of all the function declarations, macros and datatype declarations for UART Communication.
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/20/2019
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef INC_UART_COMM_BBG_H_
#define INC_UART_COMM_BBG_H_

#include "inc/main.h"
#include "inc/myuart.h"
#include "inc/packet.h"



#define BAUDRATE_BBG                    (115200)


//******************************************************************************************/
//                           FUNCTION DECLARATIONS
//******************************************************************************************/

/**
 * @brief This function configures the UART to communicate with beaglebone.
 * @return void.
 */
void configureUART_bbg(void);


/**
 * @brief This function handles the received packet.
 * @param datap_rcv The packet structure object.
 * @return void.
 */
void uart_packet_handler(packet datap_rcv);


/**
 * @brief Interrupt Handler for UART Communication with BBG.
 * @return void.
 */
void uart_commhandler(void);



/**
 * @brief This function is used for testing UART communication with beaglebone.
 * @return void.
 */
void uart_bbg_test(void);



/**
 * @brief This function sends an acknowledgment for the receoved packet.
 * @param uart The UART line used.
 * @return void.
 */
inline void ack_send_uart(uart_t uart)
{
    uint8_t payload_arr[1];
    payload_arr[0] = DONT_CARE;
    packet_send_uart(UART_BBG, packet_make(ACK_ID, payload_arr, 1, FALSE));
}


#endif /* INC_UART_COMM_BBG_H_ */
