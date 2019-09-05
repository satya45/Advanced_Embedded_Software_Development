/**
 * @file packet.c
 * @brief This file consists of all the functions required for creating Packets.
 *
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/26/2019
 *
 * @copyright Copyright (c) 2019
 *
 */

#include <string.h>
#include "inc/packet.h"
#include "inc/delay.h"
#include "inc/uart_comm_bbg.h"
#include "inc/checksum.h"
#include "inc/timer.h"


packet packet_make(uint8_t event_id, uint8_t *payload, uint16_t size, uint8_t ack)
{
    int i;
    packet datap;

    datap.preamble = PREAMBLE;
    datap.event_id = event_id;
    datap.size = size;
//    printf("Size: %d\n", datap.size);
    for(i = 0; i < datap.size; i++)
    {
        datap.payload[i] = payload[i];
//        printf("Payload : %d\n", datap.payload[i]);
    }
    datap.ack = ack;
    datap.crc_check = checksum_calc(payload, datap.size);
    datap.postamble = POSTAMBLE;

    return datap;
}

void packet_send_uart(uart_t uart, packet datap_send)
{
    uint16_t i;
    uint16_t payload_size;

    uart_send(uart, datap_send.preamble);
    uart_send(uart, datap_send.event_id);
    uart_send(uart, datap_send.size & 0x00FF);
    uart_send(uart, (datap_send.size & 0xFF00)>>8);

    payload_size = datap_send.size;

    for(i = 0; i < payload_size; i++)
    {
        uart_send(uart, datap_send.payload[i]);
    }

    uart_send(uart, datap_send.ack);
    uart_send(uart, datap_send.crc_check & 0x00FF);
    uart_send(uart, (datap_send.crc_check & 0xFF00)>>8);
    uart_send(uart, datap_send.postamble);

    //Start Timer for acknowledgment reception over here.
    if(datap_send.ack)
    {
        enable_timer_int(timer_retry);
    }
}


packet packet_rcv_uart(uart_t uart)
{
    uint16_t i;
    packet datap_rcv;

    datap_rcv.preamble = UARTCharGetNonBlocking(uart);
//    printf("PREAMBLE: %d.\n", datap_rcv.preamble);

    datap_rcv.event_id = UARTCharGetNonBlocking(uart);
//    printf("EVENT ID: %d.\n", datap_rcv.event_id);

    datap_rcv.size = UARTCharGetNonBlocking(uart);
    datap_rcv.size = datap_rcv.size | (UARTCharGetNonBlocking(uart)<<8);
//    printf("SIZE: %d.\n", datap_rcv.size);


    for(i = 0; i < datap_rcv.size; i++)
    {
        datap_rcv.payload[i] = (uint8_t)UARTCharGetNonBlocking(uart);
//        printf("PAYLOAD RECEIVED: %d.\n", datap_rcv.payload[i]);
    }


    datap_rcv.ack = UARTCharGetNonBlocking(uart);
//    printf("ACK: %d.\n", datap_rcv.ack);


    datap_rcv.crc_check = UARTCharGetNonBlocking(uart);
    datap_rcv.crc_check = datap_rcv.crc_check | (UARTCharGetNonBlocking(uart)<<8);
//    printf("CHECKSUM: %x.\n", datap_rcv.crc_check);


    datap_rcv.postamble = UARTCharGetNonBlocking(uart);
//    printf("POSTAMBLE: %d.\n", datap_rcv.postamble);

    //Send Acknowledgement here if ACK bit is set..
    if(datap_rcv.ack && datap_rcv.crc_check == checksum_calc(datap_rcv.payload, datap_rcv.size))
    {
        ack_send_uart(UART_BBG);
//        printf("Acknowledgement Sent.\n");
    }

    return datap_rcv;
}


void packet_msglog_uart(uart_t uart, uint8_t *msg_log)
{
    packet_send_uart(uart, packet_make(MSG_LOG_ID, msg_log, strlen((char *)msg_log), TRUE));
//    packet_send_uart(uart, packet_make(MSG_LOG_ID, msg_log, 8, TRUE));
}


void packet_loopback_test(void)
{
    packet datap_send;
    configureUART_bbg();

    packet_rcv_uart(UART_BBG);
    datap_send = packet_make(1,"Siddhant", 8, 0);
    packet_send_uart(UART_BBG, datap_send);
    delay_ms(1000);

}

