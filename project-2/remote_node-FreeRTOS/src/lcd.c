/**
 * @file lcd.c
 * @brief This file consists of all the functions required for the LCD.
 *
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/20/2019
 *
 * @copyright Copyright (c) 2019
 *
 */

#include "inc/lcd.h"

uint8_t row_address[4] = {0x00, 0x40, 0x10, 0x50};
void lcd_init()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
    GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, ALL_DATA_PINS);
    GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, ALL_CMD_PINS);
   // GPIOPinWrite(GPIO_PORTK_BASE, ALL_DATA_PINS, 0);
    //GPIOPinWrite(GPIO_PORTK_BASE, ALL_CMD_PINS, 0);
    SysCtlDelay(100000);
    GPIOPinWrite(GPIO_PORTK_BASE, RS, 0); //RS Bit set to low to select command register
    SysCtlDelay(100000);
    GPIOPinWrite(GPIO_PORTK_BASE, ALL_DATA_PINS, SET_4_BIT);
    GPIOPinWrite(GPIO_PORTK_BASE, E, 0);
    GPIOPinWrite(GPIO_PORTK_BASE, E, E);
    GPIOPinWrite(GPIO_PORTK_BASE, E, 0);
  //  GPIOPinWrite(GPIO_PORTK_BASE, ALL_DATA_PINS, SET_4_BIT);
    GPIOPinWrite(GPIO_PORTK_BASE, E, 0);
    GPIOPinWrite(GPIO_PORTK_BASE, E, E);
    GPIOPinWrite(GPIO_PORTK_BASE, E, 0);
    lcd_write_cmd(DISPLAY_ON);
    lcd_write_cmd(LCD_SET_2_LINES);
    lcd_write_cmd(LCD_SET_2_LINES);
    lcd_write_cmd(LCD_SET_2_LINES);
    lcd_write_cmd(CLEAR_DISPLAY);
    lcd_write_cmd(LCD_INCREMENT);
}

void lcd_write_cmd(uint8_t command)
{
    GPIOPinWrite(GPIO_PORTK_BASE, RW, 0);
    GPIOPinWrite(GPIO_PORTK_BASE, RS, 0); //RS Bit set to low to select command register
    SysCtlDelay(100000);
    GPIOPinWrite(GPIO_PORTK_BASE, ALL_DATA_PINS, (command >> 4));
    GPIOPinWrite(GPIO_PORTK_BASE, E, 0);
    GPIOPinWrite(GPIO_PORTK_BASE, E, E);
    GPIOPinWrite(GPIO_PORTK_BASE, E, 0);
    GPIOPinWrite(GPIO_PORTK_BASE, ALL_DATA_PINS, command);
    GPIOPinWrite(GPIO_PORTK_BASE, E, 0);
    GPIOPinWrite(GPIO_PORTK_BASE, E, E);
    GPIOPinWrite(GPIO_PORTK_BASE, E, 0);
}

void lcd_write_data(uint8_t data)
{
    GPIOPinWrite(GPIO_PORTK_BASE, RW, 0);
    GPIOPinWrite(GPIO_PORTK_BASE, RS, RS); //RS Bit set to high to select data register
    SysCtlDelay(100000);
    GPIOPinWrite(GPIO_PORTK_BASE, ALL_DATA_PINS, (data >> 4));
    GPIOPinWrite(GPIO_PORTK_BASE, E, 0);
    GPIOPinWrite(GPIO_PORTK_BASE, E, E);
    GPIOPinWrite(GPIO_PORTK_BASE, E, 0);
    GPIOPinWrite(GPIO_PORTK_BASE, ALL_DATA_PINS, data);
    GPIOPinWrite(GPIO_PORTK_BASE, E, 0);
    GPIOPinWrite(GPIO_PORTK_BASE, E, E);
    GPIOPinWrite(GPIO_PORTK_BASE, E, 0);
}

void LCD_write(char* string)
{
    int i;
    lcd_write_cmd(CLEAR_DISPLAY);
    for(i = 0; string[i]!= '\0'; i++)
    {
        if(i == 16)
        {
           lcd_write_cmd(0xC0);
        }
        if(i == 32)
        {
            lcd_write_cmd(0x90);
        }
        if(i == 48)
        {
            lcd_write_cmd(0xD0);
        }
        lcd_write_data(string[i]);
    }
}

void lcd_goto(uint8_t addr)
{
    lcd_write_cmd(addr & 0x7f);
}

void lcdgotoxy(uint8_t row, uint8_t column)
{
    uint8_t addr;
    addr = row_address[row] + column;
    lcd_goto(addr);
}
 uint8_t lcd_read_address(void)
 {
     int32_t read;
     GPIOPinWrite(GPIO_PORTK_BASE, RW, 1);
     GPIOPinWrite(GPIO_PORTK_BASE, RS, 1); //RS Bit set to low to select command register
     SysCtlDelay(10000);
     GPIOPinWrite(GPIO_PORTK_BASE, E, 0);
     GPIOPinWrite(GPIO_PORTK_BASE, E, E);
     GPIOPinWrite(GPIO_PORTK_BASE, E, 0);
     GPIOPinTypeGPIOInput(GPIO_PORTK_BASE, ALL_DATA_PINS);
     read = GPIOPinRead(GPIO_PORTK_BASE, ALL_DATA_PINS);
     read = read << 4;
     GPIOPinWrite(GPIO_PORTK_BASE, E, 0);
     GPIOPinWrite(GPIO_PORTK_BASE, E, E);
     GPIOPinWrite(GPIO_PORTK_BASE, E, 0);
     read = read | GPIOPinRead(GPIO_PORTK_BASE, ALL_DATA_PINS);
     GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, ALL_DATA_PINS);
     return read;
 }



 void lcd_test(void)
 {
     //LCD TESTING
//     lcd_init();

     LCD_write("Testing Fingerprint");

 }
