/**
 * @file mygpio.h
 * @brief Header file for all GPIOs used.
 *
 * The file consists of all the function declarations, macros and typedefs related to Keypad functionality.
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/15/2019
 *
 * @copyright Copyright (c) 2019
 *
 */

#ifndef INC_MYGPIO_H_
#define INC_MYGPIO_H_


#include "inc/main.h"



//******************************************************************************************/
//                                  GPIO PERIPHERALS
//******************************************************************************************/
#define GPIO_PERIPHERAL_A                   (SYSCTL_PERIPH_GPIOA)       /**< Macro passed to SysCtlPeripheralEnable*/
#define GPIO_PERIPHERAL_B                   (SYSCTL_PERIPH_GPIOB)       /**< Macro passed to SysCtlPeripheralEnable*/
#define GPIO_PERIPHERAL_C                   (SYSCTL_PERIPH_GPIOC)       /**< Macro passed to SysCtlPeripheralEnable*/
#define GPIO_PERIPHERAL_D                   (SYSCTL_PERIPH_GPIOD)       /**< Macro passed to SysCtlPeripheralEnable*/
#define GPIO_PERIPHERAL_E                   (SYSCTL_PERIPH_GPIOE)       /**< Macro passed to SysCtlPeripheralEnable*/
#define GPIO_PERIPHERAL_F                   (SYSCTL_PERIPH_GPIOF)       /**< Macro passed to SysCtlPeripheralEnable*/
#define GPIO_PERIPHERAL_G                   (SYSCTL_PERIPH_GPIOG)       /**< Macro passed to SysCtlPeripheralEnable*/
#define GPIO_PERIPHERAL_H                   (SYSCTL_PERIPH_GPIOH)       /**< Macro passed to SysCtlPeripheralEnable*/
#define GPIO_PERIPHERAL_J                   (SYSCTL_PERIPH_GPIOJ)       /**< Macro passed to SysCtlPeripheralEnable*/
#define GPIO_PERIPHERAL_K                   (SYSCTL_PERIPH_GPIOK)       /**< Macro passed to SysCtlPeripheralEnable*/
#define GPIO_PERIPHERAL_L                   (SYSCTL_PERIPH_GPIOL)       /**< Macro passed to SysCtlPeripheralEnable*/
#define GPIO_PERIPHERAL_M                   (SYSCTL_PERIPH_GPIOM)       /**< Macro passed to SysCtlPeripheralEnable*/
#define GPIO_PERIPHERAL_N                   (SYSCTL_PERIPH_GPION)       /**< Macro passed to SysCtlPeripheralEnable*/
#define GPIO_PERIPHERAL_P                   (SYSCTL_PERIPH_GPIOP)       /**< Macro passed to SysCtlPeripheralEnable*/
#define GPIO_PERIPHERAL_Q                   (SYSCTL_PERIPH_GPIOQ)       /**< Macro passed to SysCtlPeripheralEnable*/
#define GPIO_PERIPHERAL_R                   (SYSCTL_PERIPH_GPIOR)       /**< Macro passed to SysCtlPeripheralEnable*/
#define GPIO_PERIPHERAL_S                   (SYSCTL_PERIPH_GPIOS)       /**< Macro passed to SysCtlPeripheralEnable*/
#define GPIO_PERIPHERAL_T                   (SYSCTL_PERIPH_GPIOT)       /**< Macro passed to SysCtlPeripheralEnable*/



//******************************************************************************************/
//                             BASE ADDRESS OF MEMORIES AND PERIPHERALS
//******************************************************************************************/



#endif /* INC_MYGPIO_H_ */
