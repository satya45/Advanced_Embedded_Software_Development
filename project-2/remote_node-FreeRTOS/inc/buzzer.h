/**
 * @file buzzer.h
 * @brief Header file for buzzer.c.
 *
 * The file consists of all the function declarations, macros and typedefs related to Buzzer.
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/19/2019
 *
 * @copyright Copyright (c) 2019
 *
 */

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_


#include "inc/main.h"


//******************************************************************************************/
//                              GPIO PORT AND PINS
//******************************************************************************************/
#define BUZZER_PORT         (GPIO_PORTG_BASE)       /**< GPIO port for buzzer*/
#define BUZZER_PIN          (GPIO_PIN_0)            /**< GPIO pin for buzzer*/



//******************************************************************************************/
//                              FREQUENCY VALUES FOR BUZZER
//******************************************************************************************/
#define MAX_FREQ            (2048)                  /**< This is the maximum frequency value for the buffer*/
#define FREQ_1              ()



//******************************************************************************************/
//                              FUNCTION DECLARATIONS
//******************************************************************************************/

/**
 * @brief This function configures the GPIOs required for the buzzer.
 * @return void.
 */
void buzzer_config(void);


/**
 * @brief This function sets the duty cycle of the buzzer.
 * @param freq The frequency value provided for the buzzer to vary sound.
 * @return void.
 */
void buzzer_dutycycle(uint32_t freq);


/**
 * @brief This function switches the buzzer on/off.
 * @param state The state parameter represents on or off. Non-zero value represents ON, Zero represents OFF.
 * @return void.
 */
void buzzer_onoff(bool state);


/**
 * @brief This function is used for testing.
 * @return void.
 */
void buzzer_test(void);

#endif /* INC_BUZZER_H_ */
