/**
 * @file keypad.h
 * @brief Header file for keypad.c.
 *
 * The file consists of all the function declarations, macros and typedefs related to Keypad functionality.
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/20/2019
 *
 * @copyright Copyright (c) 2019
 *
 */

#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

#include "inc/main.h"


//******************************************************************************************/
//                              GPIO PORTS AND PINS
//******************************************************************************************/
#define COMMON_COLUMN_PORT          (GPIO_PORTM_BASE)           /**< Common Port for all Interrupts*/
#define COLUMN1_PORT                (GPIO_PORTM_BASE)           /**< Column 1 from right*/
#define COLUMN1_PIN                 (GPIO_PIN_0)                /**< GPIO pin for Column 1*/
#define COLUMN2_PORT                (GPIO_PORTM_BASE)           /**< Column 2 from right*/
#define COLUMN2_PIN                 (GPIO_PIN_1)                /**< GPIO pin for Column 2*/
#define COLUMN3_PORT                (GPIO_PORTM_BASE)           /**< Column 3 from right*/
#define COLUMN3_PIN                 (GPIO_PIN_2)                /**< GPIO pin for Column 3*/
#define COLUMN4_PORT                (GPIO_PORTM_BASE)           /**< Column 4 from right*/
#define COLUMN4_PIN                 (GPIO_PIN_6)                /**< GPIO pin for Column 4*/


#define ROW1_PORT                   (GPIO_PORTQ_BASE)           /**< Row 1 from Top*/
#define ROW1_PIN                    (GPIO_PIN_0)                /**< GPIO pin for Row 1*/
#define ROW2_PORT                   (GPIO_PORTQ_BASE)           /**< Row 2 from Top*/
#define ROW2_PIN                    (GPIO_PIN_2)                /**< GPIO pin for Row 2*/
#define ROW3_PORT                   (GPIO_PORTQ_BASE)           /**< Row 3 from Top*/
#define ROW3_PIN                    (GPIO_PIN_3)                /**< GPIO pin for Row 3*/
#define ROW4_PORT                   (GPIO_PORTQ_BASE)           /**< Row 4 from Top*/
#define ROW4_PIN                    (GPIO_PIN_1)                /**< GPIO pin for Row 4*/


#define COLUMN4                     (0)
#define COLUMN3                     (1)
#define COLUMN2                     (2)
#define COLUMN1                     (3)




//******************************************************************************************/
//                             KEYPAD CHARACTERS AND INFO
//******************************************************************************************/
#define CHAR_0                      (0)
#define CHAR_1                      (1)
#define CHAR_2                      (2)
#define CHAR_3                      (3)
#define CHAR_4                      (4)
#define CHAR_5                      (5)
#define CHAR_6                      (6)
#define CHAR_7                      (7)
#define CHAR_8                      (8)
#define CHAR_9                      (9)
#define CHAR_A                      (A)
#define CHAR_B                      (B)
#define CHAR_C                      (C)
#define CHAR_D                      (D)
//#define CHAR_*                      (*)


#define ROWS                        (4)                 /**< Number of Rows in Keypad*/
#define COLUMNS                     (4)                 /**< Number of Columns in Keypad*/


//******************************************************************************************/
//                          VARIABLE DECLARATIONS
//******************************************************************************************/

//char keypad_digits[ROWS][COLUMNS];

//******************************************************************************************/
//                          FUNCTION DECLARATIONS
//******************************************************************************************/

/**
 * @brief This function initializes GPIOs and the required interrupts.
 * @return void.
 */
void keypad_config(void);


/**
 * @brief Enable Interrupts for Keypad Functionality.
 * @return void.
 */
void keypad_interrupt_enable(void);


/**
 * @brief Disable Interrupts for Keypad Functionality.
 * @return void.
 */
void keypad_interrupt_disable(void);

/**
 * @brief Callback function of Column 1 Interrupt.
 * @return void.
 */
void column_handler(void);


/**
 * @brief This function detects which key is pressed.
 * @param column_port The Column Port being checked.
 * @param column_pin The Column Pin being checked.
 * @param keypad_digits The array storing character data.
 * @param column The column being examined.
 * @return void.
 */
void keypad_button_detect(uint32_t column_port, uint32_t column_pin, char keypad_digits[ROWS][COLUMNS], uint8_t column);

#endif /* INC_KEYPAD_H_ */
