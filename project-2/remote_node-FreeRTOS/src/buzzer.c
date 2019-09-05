/**
 * @file buzzer.c
 * @brief This file consists of all the functions required for the Buzzer.
 *
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/19/2019
 *
 * @copyright Copyright (c) 2019
 *
 */

#include "inc/buzzer.h"
#include "driverlib/pwm.h"

void buzzer_config(void)
{
    //Setting PWM Clock.
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);

    // Configure the PWM0 to count up/down without synchronization.
    PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE_UP_DOWN |
                    PWM_GEN_MODE_NO_SYNC);


    //Enabling Port G for PWM functionality.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);

    //Waits till peripheral is enabled.
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0) || !SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOG))
    {
    }

    GPIOPinConfigure(GPIO_PG0_M0PWM4);

    GPIOPinTypePWM(BUZZER_PORT, BUZZER_PIN);
}




void buzzer_dutycycle(uint32_t freq)
{

    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, g_ui32SysClock/freq);

    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4,
                     PWMGenPeriodGet(PWM0_BASE, PWM_GEN_2) / 6);

    // Enable the PWM0 Bit0 (PD0) output signal.
    PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT, true);

}


void buzzer_onoff(bool state)
{
    if(state)
    {
        // Enable the PWM generator block.
        PWMGenEnable(PWM0_BASE, PWM_GEN_2);
    }
    else
    {
        //Disable the buzzer.
        PWMGenDisable(PWM0_BASE, PWM_GEN_2);
    }
}


void buzzer_test(void)
{
    buzzer_config();
    buzzer_dutycycle(MAX_FREQ);
//    buzzer_onoff(1);
//    //    SysCtlDelay(100000);
//    buzzer_onoff(0);

}

