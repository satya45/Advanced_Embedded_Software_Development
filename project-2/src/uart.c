#include "uart.h"


uint8_t uart_init(void)
{
    int ret;
    struct termios term;
    uart_fd = open(UART_PORT, O_RDWR|O_NOCTTY|O_NDELAY);
    if(uart_fd == -1)
    {
        perror("UART Open failed\n");
        return 1;
    }
    else
    fcntl(uart_fd, F_SETFL, 0);
    tcgetattr(uart_fd, &term);
    ret =  cfsetispeed(&term, B115200);
    printf("RET from set cfispeed %d\n", ret);
    if(ret == -1)
    {
        perror("Input baud failed");
        return 1;
    }
    ret = cfsetospeed(&term, B115200);
    printf("RET from set cfispeed %d\n", ret);
    if(ret == -1)
    {
        perror("Output baud failed");
        return 1;
    } 

    
    term.c_cflag |= (CLOCAL);
   // term.c_cflag &= ~CSIZE;
    term.c_cflag |= CS8;
   // term.c_cflag &= ~PARENB;

    term.c_lflag &= ~(ICANON | ECHO | ECHONL | ISIG | IEXTEN);
    term.c_iflag &= ~(ISTRIP | IXON | INLCR | PARMRK | ICRNL | IGNBRK);
    term.c_oflag &=  ~(OPOST);

    tcsetattr(uart_fd, TCSAFLUSH, &term);

    return 0;
}

uint8_t send_uart(int *data, uint8_t bytes)
{
    int ret;
    ret = write(uart_fd, data, bytes);
    if(ret != bytes)
    {
        perror("Write failed");
        return 1;
    }
    return 0;
}

uint8_t send_byte_uart(uint8_t data)
{
    int ret;
    ret = write(uart_fd, &data, 1);
    if(ret != 1)
    {
        perror("Write one byte through UART failed");
    }
}

uint8_t rcv_byte_uart()
{
    printf("inside rcv data\n");
    int ret;
    uint8_t data;
    ret = read(uart_fd, &data, 1);
    if(ret != 1)
    {
        perror("Write one byte through UART failed");
    }
    printf("After read\n");
    return data;
}

uint8_t rcv_uart(int *data, uint8_t bytes)
{
    int ret;
    ret = read(uart_fd, data, bytes);
    if(ret != bytes)
    {
        perror("Write failed");
        return 1;
    }
    return 0;

}

