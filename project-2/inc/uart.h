#ifndef _UART_H
#define _UART_H

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>

#define UART_PORT "/dev/ttyO4"

uint8_t uart_init();
uint8_t send_uart(int*, uint8_t bytes);
uint8_t rcv_byte_uart(void);
uint8_t rcv_uart(int *, uint8_t bytes);
uint8_t send_byte_uart(uint8_t);
int uart_fd;

#endif

