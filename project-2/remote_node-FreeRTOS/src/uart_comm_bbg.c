/**
 * @file uart_comm_bbg.c
 * @brief This file consists of all the functions required for the UART Communication between TIVA and Beaglebone, TIVA and Fingerprint sensor GT521F32.
 *
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/19/2019
 *
 * @copyright Copyright (c) 2019
 *
 */

#include "inc/uart_comm_bbg.h"
#include "inc/packet.h"
#include "inc/lcd.h"
#include "inc/timer.h"
#include "inc/keypad.h"
#include "inc/fingerprint.h"
#include "inc/buzzer.h"
#include "inc/checksum.h"
#include "inc/delay.h"



//Change for generalised UARt configuration.
void configureUART_bbg(void)
{
    // Enable the GPIO Peripheral used by the UART.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);

    // Enable UART0
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART6);

    // Configure GPIO Pins for UART mode.
    ROM_GPIOPinConfigure(GPIO_PP0_U6RX);
    ROM_GPIOPinConfigure(GPIO_PP1_U6TX);
    ROM_GPIOPinTypeUART(GPIO_PORTP_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Use the internal 16MHz oscillator as the UART clock source.
    UARTClockSourceSet(UART6_BASE, UART_CLOCK_PIOSC);

    // Initialize the UART for console I/O.
//    UARTStdioConfig(UART6_BASE, BAUDRATE_BBG, CLOCK);



    UARTConfigSetExpClk(UART6_BASE, CLOCK, BAUDRATE_BBG, UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8);

//    UARTFIFOEnable(UART6_BASE);

//    UARTFIFOLevelSet(UART6_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8);
    UARTTxIntModeSet(UART6_BASE, UART_TXINT_MODE_EOT);

    uart_comm_flushRX(UART_BBG);
    uart_comm_flushTX(UART_BBG);

    UARTIntRegister(UART6_BASE, uart_commhandler);

    //Enabling UART interrupt for Transmitter and Receiver.
    UARTIntEnable(UART6_BASE, UART_INT_RT | UART_INT_RX | UART_INT_TX);

    UARTEnable(UART6_BASE);

}


void uart_packet_handler(packet datap_rcv)
{
    if(datap_rcv.preamble == PREAMBLE && datap_rcv.crc_check == checksum_calc(datap_rcv.payload, datap_rcv.size))
    {
        switch(datap_rcv.event_id)
        {

        case OTP_SENT_USER_ID:
        {
            //Disable Fingerprint Sensor Interrupts
            fp_interrupt_disable();

            //Enable Timer for Input OTP.
            enable_timer_int(timer_otp);

            //Raising global flag to accept OTP.
            otp_flag = 1;

            printf("OTP has been Sent to the Registered Email id.\n");
            LCD_write("ENTER OTP....SENT TO REGISTERED EMAIL ID");

            break;
        }

        case ACCESS_STATUS_RCV_ID:
        {
            if(datap_rcv.payload[0])
            {
                printf("Access Granted.\n");
                LCD_write("Access Granted");
                packet_msglog_uart(UART_BBG, "Access Granted");
            }
            else
            {
                printf("Access Denied.\n");
                LCD_write("Access Denied");
                packet_msglog_uart(UART_BBG, "Access Denied");
                buzzer_onoff(1);
            }

            break;
        }

        case GUI_ID:
        {
            printf("Request from GUI received.\n");

            if(datap_rcv.payload[0] == GUI_BUZZER_OFF)
            {
                buzzer_onoff(0);
                printf("Buzzer off.\n");
                LCD_write("Buzzer off");
                packet_msglog_uart(UART_BBG, "Buzzer off");
            }
            else if(datap_rcv.payload[0] == GUI_ADD_FINGERPRINT)
            {
                add_fingerprint(UART_FP);
                delay_ms(3000);
                LCD_write("Press Finger on Scanner to Unlock");
            }
            else if(datap_rcv.payload[0] == GUI_BUZZER_ON)
            {
                buzzer_onoff(1);
                printf("Buzzer on.\n");
                LCD_write("Buzzer on");
                packet_msglog_uart(UART_BBG, "Buzzer on");
            }
            else if(datap_rcv.payload[0] == GUI_DELETE_FINGERPRINT_ALL)
            {
                fp_deleteall(UART_FP);
                printf("Fingerprint Database Erased.\n");
                LCD_write("Fingerprint Database Erased.");
                packet_msglog_uart(UART_BBG, "Fingerprint Database Erased");
            }
            else if(datap_rcv.payload[0] == GUI_RESET_SYSTEM)
            {
                send_failcount = 0;
                otp_count = 0;
                otp_flag = 0;
                memset(otp_arr, 0, 4);
                disable_timer(timer_otp);
                disable_timer(timer_retry);
                reset_timer(timer_otp, OTP_INPUT_TIME);
                reset_timer(timer_retry, PACKET_RETRY_TIME);
                keypad_interrupt_enable();
                fp_interrupt_enable();
                IntMasterEnable();
                printf("Resetting System.\n");
                LCD_write("Resetting System.");
                packet_msglog_uart(UART_BBG, "Resetting System");
            }
            else if(datap_rcv.payload[0] == GUI_ALLOW_ACCESS)
            {
                printf("Access Granted.\n");
                LCD_write("Access Granted");
                packet_msglog_uart(UART_BBG, "Access Granted");
            }

            break;
        }

        case ACK_ID:
        {
            printf("Acknowledgement Received.\n");
            disable_timer(timer_retry);
            reset_timer(timer_retry, PACKET_RETRY_TIME);

            //Reset fail count to zero
            send_failcount = 0;

            break;
        }

        }
    }
}



void uart_commhandler(void)
{

    uint32_t int_status;

    IntMasterDisable();

    // Get the interrrupt status.
    int_status = UARTIntStatus(UART_BBG, false);


    if (int_status & UART_INT_TX)
    {
        printf("Packet Sent.\n");
        UARTIntClear(UART_BBG, UART_INT_TX);
    }
    else if (int_status & UART_INT_RT || int_status & UART_INT_RX)
    {
        printf("Packet Received due to RT/RX.\n");

        if(UARTCharsAvail(UART_BBG))
        {
            uart_packet_handler(packet_rcv_uart(UART_BBG));
        }

        // Clear the asserted interrupts.
        UARTIntClear(UART_BBG, UART_INT_RX | UART_INT_RT);

    }

    IntMasterEnable();
}



void uart_bbg_test(void)
{
    configureUART_bbg();
    uart_send(UART_BBG, 4);

    int32_t x = UARTCharGet(UART4_BASE);

    printf("Displaying received Characters : %d\n", x);

}

