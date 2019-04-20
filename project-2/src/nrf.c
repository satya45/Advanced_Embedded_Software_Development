/**
 * @file nrf.c
 * @author Satya Mehta and Siddhant Jajoo
 * @brief 
 * @version 0.1
 * @date 2019-04-17
 * SPI_Test https://raw.githubusercontent.com/torvalds/linux/master/tools/spi/spidev_test.c
 * @copyright Copyright (c) 2019
 * 
 */
#include "nrf.h"

uint8_t spi_init()
{
    int ret = 0;

    spi_fd = open(device, O_RDWR);
    if (spi_fd < 0)
    {
        error_log("ERROR: Open failed in spi_init()", ERROR_DEBUG, P2);
        perror("ERROR: Open failed in spi_init()");
    }
    /*
	 * spi mode
	 */
    ret = ioctl(spi_fd, SPI_IOC_WR_MODE32, &mode);
    if (ret == -1)
    {
        error_log("ERROR: Ioctl failed setting mode", ERROR_DEBUG, P2);
        perror("ERROR: Ioctl failed setting mode");
    }
    ret = ioctl(spi_fd, SPI_IOC_RD_MODE32, &mode);
    if (ret == -1)
    {
        error_log("ERROR: Ioctl failed in reading bits", ERROR_DEBUG, P2);
        perror("ERROR: Ioctl failed in reading bits");
    }
    /*
	 * bits per word
	 */
    ret = ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1)
    {
        error_log("ERROR: Ioctl failed in WR bits", ERROR_DEBUG, P2);
        perror("ERROR: Ioctl failed in WR bit");
    }
    ret = ioctl(spi_fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret == -1)
    {
        error_log("ERROR: Ioctl failed in RD bits", ERROR_DEBUG, P2);
        perror("ERROR: Ioctl failed in RD bits");
    }
    /*
	 * max speed hz
	 */
    ret = ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret == -1)
    {
        perror("ERROR: Ioctl failed in set speed");
    }
    ret = ioctl(spi_fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (ret == -1)
    {
        error_log("ERROR: Ioctl failed in reading speed", ERROR_DEBUG, P2);
        perror("ERROR: Ioctl failed in reading speed");
    }
    return 0;
}

uint8_t nrf_set_addr()
{
    int ret;
    struct rx_pipe_p0 a;
	a.rx_address[0] = 1;
	a.rx_address[1] = 0;
	a.rx_address[2] = 0;
	a.rx_address[3] = 0;
	a.rx_address[4] = 0;
	spi_write_mul_bytes(RX_PIPE, (a.rx_address), 5);
	ret = spi_read_mul_bytes(RX_PIPE, 5);
   // printf("RX Pipe Address 0 set as ");
    // for(int i = 4; i >=0; i--)
    // {
    //    // printf("%d",rx_buff[i]);
    // }
   // printf("\n");
    struct tx_pipe b;
	b.tx_address[0] = 2;
	b.tx_address[1] = 0;
	b.tx_address[2] = 0;
	b.tx_address[3] = 0;
	b.tx_address[4] = 0;
	spi_write_mul_bytes(TX_PIPE, (b.tx_address), 5);
	ret = spi_read_mul_bytes(TX_PIPE, 5);
 //   printf("TX Pipe Address set as ");
    // for(int i = 4; i >=0; i--)
    // {
    //     printf("%d",rx_buff[i]);
    // }
  //  printf("\n");

}

uint8_t nrf_init()
{
    spi_write(0x06, 0x06);    //Power and Speed
    spi_write(0x05, 0x22); //set frequency channel;

}


uint32_t tx_packet()
{
    spi_write(00, 0x0A);
}

uint32_t rx_packet()
{
    int ret;
    uint8_t received_pipe;
    spi_read(STATUS);
    printf("Read status register value %x\n", read_status);
    received_pipe = (read_status & RX_P_NO_MASK) >> 1;
    printf("Data received in %d pipe\n", received_pipe);
    if(read_status & RX_DR_MASK)
    {
        ret =  command_nrf(RX_PAYLOAD);
        if(ret == 2)
        {
            for(int  i = 0; i < 32; i++)
            {
                printf("Bytes rcvd %d\n", rx_buff[i]);
            }   
        }
        spi_write(STATUS, 0x40);
        spi_read(STATUS);
        printf("Interrupt cleared new Status value %x\n", read_status);
        gpio_ctrl(GPIO51, GPIO51_V, 0); //set CE low
        command_nrf(RX_FLUSH);
    }
}

void gpio60_h2l(void)
{
    gpio_ctrl(GPIO60, GPIO60_V, 1);
    gpio_ctrl(GPIO60, GPIO60_V, 0);
}

void gpio60_l2h(void)
{
       gpio_ctrl(GPIO60, GPIO60_V, 1);
}

uint8_t command_nrf(uint8_t comm)
{
    int ret;
    uint8_t tx_buffer = comm;
    gpio60_h2l();
	ret = write(spi_fd, &tx_buffer, 1);
    if(ret < 1)
    {
        perror("write failed\n\n");
    }
    if(comm == RX_PAYLOAD)
    {
	    ret = read(spi_fd, rx_buff, 1);
        if(ret < 1)
        {
            perror("Read failed\n");
            return 0;
        }
        return 2;
        gpio60_l2h();
    }
	
}

uint8_t nrf_configure_mode(mode_t mode)
{
    if (mode == mode_rx)
    {

        command_nrf(RX_FLUSH);
        spi_write(0x00, 0x0B); //PRIM_RX =1, and PWR_UP = 1
        spi_read(0x00);
        spi_write(0x01, 0x00); //disable ACK
        spi_read(0x01);
        spi_write(0x11, 0x01); //set RX_PW_P0
        spi_read(0x11);
        nrf_set_addr();
        gpio_ctrl(GPIO51, GPIO51_V, 1); //set CE high
        usleep(130);
    }
}

uint8_t spi_read(uint8_t reg)
{
    int ret;
    uint8_t tx_buffer = reg;
    uint8_t rx_buffer = 0;
    // struct spi_ioc_transfer tr = {
    //     .tx_buf = (unsigned long)&tx_buffer,
    //     .rx_buf = (unsigned long)&rx_buffer,
    //     .len = 1,
    //     .delay_usecs = delay,
    //     .speed_hz = 0,
    //     .bits_per_word = 0,
    // };
    // gpio60_h2l();
    // // ret = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);
    // // usleep(100);
    // // if (ret == -1)
    // // {
    // //     error_log("ERROR: Ioctl failed in SPI_READ", ERROR_DEBUG, P2);
    // //     perror("ERROR: Ioctl failed in SPI_READ\n");
    // // }
    // ret = write(spi_fd, &tx_buffer, 1);
    // if(ret < 1)
    // {
    //     perror("write failed\n\n");
    // }
    // ret = read(spi_fd, &rx_buffer, 1);
    // if(ret < 1)
    // {
    //     perror("Read failed\n");
    // }
    // printf("Data read from the NRF %x\n", rx_buffer);
    // gpio60_l2h();
    // return rx_buffer;
	gpio60_h2l();
	ret = write(spi_fd, &tx_buffer, 1);
    if(ret < 1)
    {
        perror("write failed\n\n");
    }
	ret = read(spi_fd, &read_status, 1);
    if(ret < 1)
    {
        perror("Read failed\n");
    }
	gpio60_l2h();
}

uint8_t spi_write(uint8_t reg, uint8_t data)
{
    int ret;
    uint8_t tx_buffer = reg|WRITE_MASK;
    uint8_t rx_buffer;
	gpio60_h2l();
	ret = write(spi_fd, &tx_buffer, 1);
    if(ret < 1)
    {
        perror("write failed\n\n");
    }
	tx_buffer = data;
	ret = write(spi_fd, &tx_buffer, 1);
    if(ret < 1)
    {
        perror("write failed\n\n");
    }
	gpio60_l2h();
}

uint8_t spi_write_mul_bytes(uint8_t reg, uint8_t *data, uint8_t bytes)
{
    int ret;
    uint8_t tx_buffer = reg|WRITE_MASK;
	gpio60_h2l();
	ret = write(spi_fd, &tx_buffer, 1);
    if(ret < 1)
    {
        perror("write failed\n\n");
    }
	//tx_buffer = data;
	ret = write(spi_fd, data, bytes);
    if(ret < 1)
    {
        perror("write failed\n\n");
    }
	gpio60_l2h();
}

uint8_t spi_read_mul_bytes(uint8_t reg, uint8_t bytes)
{
    int ret;
    uint8_t tx_buffer = reg;
    gpio60_h2l();
	ret = write(spi_fd, &tx_buffer, 1);
    if(ret < 1)
    {
        perror("write failed\n\n");
    }
	ret = read(spi_fd, &rx_buff, bytes);
    if(ret < 1)
    {
        perror("Read failed\n");
    }
	gpio60_l2h();
    //return rx_buffer;
}