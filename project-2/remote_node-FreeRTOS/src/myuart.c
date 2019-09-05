/**
 * @file myuart.c
 * @brief This file consists of all the functions required for UART initialization, enable and send/receive functions.
 *
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/22/2019
 *
 * @copyright Copyright (c) 2019
 *
 */

#include "inc/myuart.h"

void uart_configure(uart_t uart, uint32_t clock, uint32_t baudrate, bool irq)
{
    if(uart == UART_FP)
    {
        // Enable the GPIO Peripheral used by the UART.
        ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);        //No need already Done.

        // Enable UART0
        ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);

        // Configure GPIO Pins for UART mode.
        ROM_GPIOPinConfigure(GPIO_PA6_U2RX);
        ROM_GPIOPinConfigure(GPIO_PA7_U2TX);
        ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7);

        // Use the internal 16MHz oscillator as the UART clock source.
        UARTClockSourceSet(uart, UART_CLOCK_PIOSC);

        // Initialize the UART for console I/O.
        //    UARTStdioConfig(uart, BAUDRATE_BBG, CLOCK);



        UARTConfigSetExpClk(uart, clock, baudrate, UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8);

        //    UARTFIFOEnable(uart);

        //    UARTFIFOLevelSet(uart, UART_FIFO_TX1_8, UART_FIFO_RX1_8);
//        UARTTxIntModeSet(uart, UART_TXINT_MODE_EOT);

//        uart_comm_flushRX(uart);
//        uart_comm_flushTX(uart);

        //UARTIntRegister(uart, uart_commhandler);

        //Enabling UART interrupt for TRansmitter and Receiver.
//        UARTIntEnable(uart, UART_INT_RT | UART_INT_RX | UART_INT_TX);

        UARTEnable(uart);

    }
}



void uart_send(uart_t uart, uint8_t value)
{
    UARTCharPut(uart, value);
    uart_busy(uart);
}



void uart_comm_flushRX(uart_t uart)
{
    uint32_t ui32Int;

    // Temporarily turn off interrupts.
    ui32Int = IntMasterDisable();


    //Removes Characters from FIFO until there are none left.
    while(UARTCharsAvail(uart))
    {
        UARTCharGetNonBlocking(uart);
    }


    // If interrupts were enabled when we turned them off, turn them
    // back on again.
    if(!ui32Int)
    {
        IntMasterEnable();
    }
}



void uart_comm_flushTX(uart_t uart)
{

    //Disables the UART and flushes the data in TX FIFO.
    UARTDisable(uart);

    //Need to enable the UART again to resume normal operation of UART.

}
