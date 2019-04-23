#ifndef _NRF_H
#define _NRF_H

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <sys/stat.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include "gpio.h"


typedef enum {

    mode_tx = 1,
    mode_rx = 2

}modes;

#define WRITE_MASK 0x20


/*Commands for NRF*/
#define TX_FLUSH 0xE1
#define RX_FLUSH 0xE2
#define RX_PAYLOAD (0x61)
#define TX_PAYLOAD (0xA0)



/*Addresses of the register in NRF module*/
#define RX_PIPE (0x0A)
#define TX_PIPE (0x10)
#define RX_DR_MASK (0x40)
#define TX_DS_MASK (0x20)
#define RX_P_NO_MASK (0x0E)
#define STATUS (0x07)


uint8_t spi_init(void);
uint8_t spi_read(uint8_t reg);
uint8_t spi_write(uint8_t,uint8_t);
uint8_t spi_write_mul_bytes(uint8_t , uint8_t *, uint8_t bytes);
uint8_t spi_read_mul_bytes(uint8_t, uint8_t bytes);
void gpio60_h2l(void);
void gpio60_l2h(void);
uint32_t tx_packet(void);
uint32_t rx_packet(void);
uint8_t command_nrf(uint8_t);
uint8_t nrf_configure_mode(modes);
uint8_t nrf_set_addr(void);



static const char *device = "/dev/spidev1.0";
static uint32_t mode = 0;
static uint8_t bits = 8;
static uint32_t speed = 100000;
static uint16_t delay;
int spi_fd;
uint8_t nrf_init(void);
uint8_t rx_buff[32];
uint8_t read_status;
struct rx_pipe_p0{

    uint8_t rx_address[5];
};

struct tx_pipe{

    uint8_t tx_address[5];
};

struct send_packet{

    uint32_t data;
};

#endif