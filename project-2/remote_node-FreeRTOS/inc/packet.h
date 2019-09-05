/**
 * @file packet.h
 * @brief Header file for packet.c.
 *
 * The file consists of all the function declarations, macros and typedefs related to Creating packet structure.
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo.
 * @date 04/26/2019
 *
 * @copyright Copyright (c) 2019
 *
 */

#ifndef INC_PACKET_H_
#define INC_PACKET_H_


#include "inc/main.h"
#include "inc/myuart.h"


//******************************************************************************************/
//                              PACKET STRUCTURE MACROS
//******************************************************************************************/
#define PREAMBLE                            (0xAB)
#define POSTAMBLE                           (0xBA)



typedef struct __attribute__((__packed__))
{
    uint8_t preamble;
    uint8_t event_id;
    uint16_t size;
    uint8_t payload[50];
//    uint8_t *payload;
    uint8_t ack;
    uint16_t crc_check;
    uint8_t postamble;
}packet;



//******************************************************************************************/
//                              FUNCTION DECLARATIONS
//******************************************************************************************/

/**
 * @brief This function populates the structure with payload to send to beaglebone.
 * @param event_id This event signifies the type of data contained by the payload.
 * @param payload The data that needs to be passed via payload.
 * @param size The size of the array.
 * @param ack This bit is set if acknowledge feature is enabled.
 * @return packet The packet structure.
 */
packet packet_make(uint8_t event_id, uint8_t *payload, uint16_t size, uint8_t ack);


/**
 * @brief This function sends the packet over the specified UART line and starts timer for packet acknowledgment retries.
 * @param uart The UARt line used.
 * @param data_send The packet structure object to send.
 * @return void.
 */
void packet_send_uart(uart_t uart, packet data_send);


/**
 * @brief This function receives the packet and populates the received data in a Packet structure and sends acknowledgement if ack bit is set.
 * @param uart The UARt line used.
 * @return packet The packet structure in which the data has been populated.
 */
packet packet_rcv_uart(uart_t uart);


/**
 * @brief This function sends a message to be logged to the Beaglebone (Control Node).
 * @param uart The UART line used.
 * @param msg_log The String to send to the Control Node.
 */
void packet_msglog_uart(uart_t uart, uint8_t *msg_log);

/**
 * @brief This function is used to test send and receive data.
 * @return void.
 */
void packet_loopback_test(void);


#endif /* INC_PACKET_H_ */
