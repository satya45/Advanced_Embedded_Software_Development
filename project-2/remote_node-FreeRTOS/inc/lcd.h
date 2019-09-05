/**
 * @file lcd.h
 * @brief Header file for lcd.c.
 *
 * The file consists of all the function declarations, macros and typedefs related to LCD functionality.
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Satya Mehta
 * @date 04/20/2019
 *
 * @copyright Copyright (c) 2019
 *
 */

#ifndef LCD_H_
#define LCD_H_


#include "inc/main.h"




#define D4 GPIO_PIN_0
#define D5 GPIO_PIN_1
#define D6 GPIO_PIN_2
#define D7 GPIO_PIN_3

#define CLEAR_DISPLAY (0x01)
#define SET_4_BIT (0x02)
#define DISPLAY_ON (0x0E)
#define LCD_SET_2_LINES (0x28)
#define LCD_INCREMENT (0x06)

#define RS GPIO_PIN_4
#define RW GPIO_PIN_5
#define E  GPIO_PIN_6
#define ALL_CMD_PINS E|RW|RS
#define ALL_DATA_PINS D7|D6|D5|D4



//******************************************************************************************/
//                          FUNCTION DECLARATIONS
//******************************************************************************************/

void lcd_init();
void lcd_write_data(uint8_t);
void lcd_write_cmd(uint8_t);
void LCD_write(char *);
void lcd_goto(uint8_t addr);
void lcdgotoxy(uint8_t row, uint8_t column);
uint8_t lcd_read_address(void);
void lcd_test(void);
inline void lcd_clear(void)
{
    lcd_write_cmd(CLEAR_DISPLAY);
}

#endif /* LCD_H_ */
