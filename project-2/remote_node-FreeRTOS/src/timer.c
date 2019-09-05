/**
 * @file timer.c
 * @brief This file consists of all the functions required for Timer initialization, enable and callback functions.
 *
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/13/2019
 *
 * @copyright Copyright (c) 2019
 *
 */

#include "inc/timer.h"
#include "inc/sync_objects.h"
#include "inc/keypad.h"
#include "inc/delay.h"
#include "inc/fingerprint.h"
#include "inc/lcd.h"


void timer_config(timer_t timer, uint32_t ms)
{
    if (timer == timer_retry)
    {

        // Enable the peripherals for Timers.
        ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

        // Configure the 32-bit periodic timer.
        ROM_TimerConfigure(timer, TIMER_CFG_PERIODIC);
        ROM_TimerLoadSet(timer, TIMER_A, (g_ui32SysClock/1000)*ms);

        // Setup the interrupts for the timer timeout.
        ROM_IntEnable(INT_TIMER0A);
        ROM_TimerIntEnable(timer, TIMER_TIMA_TIMEOUT);


    }
    else if (timer == timer_otp)
    {

        // Enable the peripherals for Timers.
        ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

        // Configure the 32-bit periodic timer.
        ROM_TimerConfigure(timer, TIMER_CFG_PERIODIC);
        ROM_TimerLoadSet(timer, TIMER_A, (g_ui32SysClock/1000)*ms);

        // Setup the interrupts for the timer timeout.
        ROM_IntEnable(INT_TIMER1A);
        ROM_TimerIntEnable(timer, TIMER_TIMA_TIMEOUT);

    }
}


void enable_timer_int(timer_t timer)
{
    // Enable the timers.
    ROM_TimerEnable(timer, TIMER_A);

    // Enable processor interrupts.
    ROM_IntMasterEnable();
}

inline void disable_timer(timer_t timer)
{
    //Disable Timer.
    ROM_TimerDisable(timer, TIMER_A);
}


inline void reset_timer(timer_t timer, uint32_t ms)
{
    //Reset Timer
    ROM_TimerLoadSet(timer, TIMER_A, (g_ui32SysClock/1000)*ms);
}

void timer0handler(void)
{
    //Disable all interrupts
    ROM_IntMasterDisable();

    // Clear the timer interrupt.
    ROM_TimerIntClear(timer_retry, TIMER_TIMA_TIMEOUT);

    printf("In Retry Timer Interrupt Handler.\n");
    send_failcount++;

    if(send_failcount <= PACKET_RETRY_COUNT)
    {
        //Send Packet Data Over here.
        printf("Sending Packet data Again.\n");
    }
    else
    {
        //Disable Timers.
        disable_timer(timer_retry);
        reset_timer(timer_retry, PACKET_RETRY_TIME);
        send_failcount = 0;
        printf("Crossed Maximum Retries, Control Node offline.\n");
    }

    //Enable all interrupts
    ROM_IntMasterEnable();
}


void timer1handler(void)
{
    //Disable all interrupts
    ROM_IntMasterDisable();

    otp_count = 0;
    otp_flag = 0;
    memset(otp_arr, 0, 4);

    // Clear the timer interrupt.
    ROM_TimerIntClear(timer_otp, TIMER_TIMA_TIMEOUT);

    //Disable and Reload the Timer for future use.
    disable_timer(timer_otp);
    reset_timer(timer_otp, OTP_INPUT_TIME);

    printf("In Keypad Timer Interrupt Handler.\n");
    printf("Timeout!!");
    LCD_write("TIMEOUT!!");

    delay_ms(3000);

    //Enable Interrupts again to Try again.
    fp_interrupt_enable();

    printf("Start Again!! Press Finger On Scanner.");
    LCD_write("Start Again!! Press Finger On Scanner.");

    //Enable all interrupts
    ROM_IntMasterEnable();

}
