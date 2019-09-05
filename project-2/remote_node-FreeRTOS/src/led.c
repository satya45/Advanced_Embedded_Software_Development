/**
 * @file led.c
 * @brief This file consists of all the functions required for LEDS.
 *
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/13/2019
 *
 * @copyright Copyright (c) 2019
 *
 */

#include "inc/led.h"



void led_init(void)
{
    // Enable the GPIO port that is used for the on-board LED.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Check if the peripheral access is enabled.
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION) || !SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }

    // Enable the GPIO pin for the LED (PN0).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_4);

}


void led_on(uint8_t led)
{
    switch(led)
    {

    case LED1:
    {
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);
        break;
    }

    case LED2:
    {
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0);
        break;
    }

    case LED3:
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_PIN_4);
        break;
    }

    case LED4:
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0);
        break;
    }

    }
}


void led_off(uint8_t led)
{
    switch(led)
    {

    case LED1:
    {
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0);
        break;
    }

    case LED2:
    {
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);
        break;
    }

    case LED3:
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, 0);
        break;
    }

    case LED4:
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);
        break;
    }

    }
}
