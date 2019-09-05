//*****************************************************************************
//
//
// Copyright (c) 2013-2017 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.1.4.178 of the EK-TM4C1294XL Firmware Package.
//
//*****************************************************************************

/**
 * @file main.c
 * @brief File containing the main functionality of the remote node.
 *
 *  The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/16/2019
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 */


#include "inc/main.h"
#include "inc/temp.h"
#include "inc/msg_queue.h"
#include "inc/sync_objects.h"
#include "inc/timer.h"
#include "inc/logger.h"
#include "inc/spi.h"
#include "inc/led.h"
#include "inc/nrf_driver.h"
#include "inc/buzzer.h"
#include "inc/keypad.h"
#include "inc/uart_comm_bbg.h"
#include "inc/checksum.h"
#include "inc/myuart.h"
#include "inc/fingerprint.h"
#include "inc/delay.h"
#include "inc/lcd.h"
#include "inc/packet.h"
#include "inc/application.h"



// Counter value used by the FreeRTOS run time stats feature.
// http://www.freertos.org/rtos-run-time-stats.html
volatile unsigned long g_vulRunTimeStatsCountValue;

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}

#endif



//*****************************************************************************
//
// This hook is called by FreeRTOS when an stack overflow error is detected.
//
//*****************************************************************************
void
vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName)
{
    //
    // This function can not return, so loop forever.  Interrupts are disabled
    // on entry to this function, so no processor interrupts will interrupt
    // this loop.
    //
    while(1)
    {
    }
}


void ConfigureUART(void)
{
    // Enable the GPIO Peripheral used by the UART.
     ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

     // Enable UART0
     ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

     // Configure GPIO Pins for UART mode.
     ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
     ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
     ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

     // Use the internal 16MHz oscillator as the UART clock source.
     UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);


     // Initialize the UART for console I/O.
     UARTStdioConfig(0, 115200, 16000000);
}



/**
 * @brief  Initialize FreeRTOS and start the initial set of tasks.
 * @return int
 */
int main(void)
{
    // Configure the system frequency.
    g_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                             SYSCTL_OSC_MAIN |
                                             SYSCTL_USE_PLL |
                                             SYSCTL_CFG_VCO_480), CLOCK);

    system_init();

    printf("Welcome to the Remote Node!!!\n");
    packet_msglog_uart(UART_BBG, "Hello");

    //Uart testing with BBG
    //uart_bbg_test();

    //Buzzer Testing
    //buzzer_test();

    //LCD testing
    //lcd_test();

    //Fingerprint Test Function.
    //fingerprint_test();

//    Packet Data testing
//    packet_loopback_test();

    // Start the scheduler.  This should not return.
    vTaskStartScheduler();

    // In case the scheduler returns for some reason, print an error and loop forever.
    UARTprintf("RTOS scheduler returned unexpectedly.\n");
    while(1)
    {
        // Do Nothing.
    }
}
