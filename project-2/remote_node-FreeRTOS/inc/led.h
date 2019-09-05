/**
 * @file led.h
 * @brief Header file for led.c.
 *
 * The file consists of all the function declarations, macros and typedefs related to LED functionality.
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/13/2019
 *
 * @copyright Copyright (c) 2019
 *
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include "inc/main.h"


//******************************************************************************************/
//                                  LEDS MACROS
//******************************************************************************************/
#define LED1    (1)
#define LED2    (2)
#define LED3    (3)
#define LED4    (4)



//******************************************************************************************/
//                              FUNCTION DECLARATIONS
//******************************************************************************************/

/**
 * @brief This function initializes all the GPIOs required for LED functionality.
 * @return void
 */
void led_init(void);


/**
 * @brief This function turns on the specified LED.
 * @param led The parameter is a macro and can be : LED1, LED2, LED3, LED4.
 * @return void.
 */
void led_on(uint8_t led);


/**
 * @brief This function turns off the specified LED.
 * @param led The parameter is a macro and can be : LED1, LED2, LED3, LED4.
 * @return void.
 */
void led_off(uint8_t led);

#endif /* INC_LED_H_ */
