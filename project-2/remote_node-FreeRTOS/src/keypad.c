/**
 * @file keypad.c
 * @brief This file consists of all the functions required for the Keypad.
 *
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/19/2019
 *
 * @copyright Copyright (c) 2019
 *
 */


#include "inc/keypad.h"
#include "inc/delay.h"
#include "inc/myuart.h"
#include "inc/lcd.h"
#include "inc/packet.h"




char keypad_digits[ROWS][COLUMNS] = {{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};

void keypad_config(void)
{

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);

    // Check if the peripheral access is enabled.
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOM) || !SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOQ))
    {
    }


    //Configuring all Rows as Output Type
    GPIOPinTypeGPIOOutput(ROW1_PORT, ROW1_PIN);
    GPIOPinTypeGPIOOutput(ROW2_PORT, ROW2_PIN);
    GPIOPinTypeGPIOOutput(ROW3_PORT, ROW3_PIN);
    GPIOPinTypeGPIOOutput(ROW4_PORT, ROW4_PIN);

    //Configuring all the output rows as Low level.
    GPIOPinWrite(ROW1_PORT, ROW1_PIN, 0);
    GPIOPinWrite(ROW2_PORT, ROW2_PIN, 0);
    GPIOPinWrite(ROW3_PORT, ROW3_PIN, 0);
    GPIOPinWrite(ROW4_PORT, ROW4_PIN, 0);

    //Configuring all Columns as Input Type
    GPIOPinTypeGPIOInput(COLUMN1_PORT, COLUMN1_PIN);      //Column 1 from right
    GPIOPinTypeGPIOInput(COLUMN2_PORT, COLUMN2_PIN);      //Column 2 from right
    GPIOPinTypeGPIOInput(COLUMN3_PORT, COLUMN3_PIN);      //Column 3 from right
    GPIOPinTypeGPIOInput(COLUMN4_PORT, COLUMN4_PIN);      //Column 4 from right

    GPIOPadConfigSet(COLUMN1_PORT, COLUMN1_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(COLUMN2_PORT, COLUMN2_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(COLUMN3_PORT, COLUMN3_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(COLUMN4_PORT, COLUMN4_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);


    GPIOIntTypeSet(COLUMN1_PORT, COLUMN1_PIN, GPIO_FALLING_EDGE);
    GPIOIntTypeSet(COLUMN2_PORT, COLUMN2_PIN, GPIO_FALLING_EDGE);
    GPIOIntTypeSet(COLUMN3_PORT, COLUMN3_PIN, GPIO_FALLING_EDGE);
    GPIOIntTypeSet(COLUMN4_PORT, COLUMN4_PIN, GPIO_FALLING_EDGE);

    GPIOIntRegister(COMMON_COLUMN_PORT, column_handler);

}


void keypad_interrupt_enable(void)
{

    GPIOIntEnable(COLUMN1_PORT, COLUMN1_PIN);
    GPIOIntEnable(COLUMN2_PORT, COLUMN2_PIN);
    GPIOIntEnable(COLUMN3_PORT, COLUMN3_PIN);
    GPIOIntEnable(COLUMN4_PORT, COLUMN4_PIN);
}


void keypad_interrupt_disable(void)
{
//    GPIOIntUnregister(COMMON_COLUMN_PORT);

    GPIOIntDisable(COLUMN1_PORT, COLUMN1_PIN);
    GPIOIntDisable(COLUMN2_PORT, COLUMN2_PIN);
    GPIOIntDisable(COLUMN3_PORT, COLUMN3_PIN);
    GPIOIntDisable(COLUMN4_PORT, COLUMN4_PIN);
}

void column_handler(void)
{
//    GPIOIntUnregister(COMMON_COLUMN_PORT);
    IntMasterDisable();

    uint32_t int_status = 0;
    uint32_t int_pin;

    //Get Pin Interrupt Status.
    int_status = GPIOIntStatus(COMMON_COLUMN_PORT, false);

    printf("Interrupt Status: %d.\n", int_status);

    if (otp_flag)
    {
        if(int_status & COLUMN4_PIN)
        {
            int_pin = COLUMN4_PIN;
            //        GPIOIntClear(COMMON_COLUMN_PORT, int_status & COLUMN4_PIN);
            keypad_button_detect(COLUMN4_PORT, COLUMN4_PIN, keypad_digits, COLUMN4);
        }
        else if(int_status & COLUMN3_PIN)
        {
            int_pin = COLUMN3_PIN;
            //        GPIOIntClear(COMMON_COLUMN_PORT, int_status & COLUMN3_PIN);
            keypad_button_detect(COLUMN3_PORT, COLUMN3_PIN, keypad_digits, COLUMN3);
        }
        else if(int_status & COLUMN2_PIN)
        {
            int_pin = COLUMN2_PIN;
            //        GPIOIntClear(COMMON_COLUMN_PORT, int_status & COLUMN2_PIN);
            keypad_button_detect(COLUMN2_PORT, COLUMN2_PIN, keypad_digits, COLUMN2);
        }
        else if(int_status & COLUMN1_PIN)
        {
            int_pin = COLUMN1_PIN;
            //        GPIOIntClear(COMMON_COLUMN_PORT, int_status & COLUMN1_PIN);
            keypad_button_detect(COLUMN1_PORT, COLUMN1_PIN, keypad_digits, COLUMN1);
        }

        otp_count++;

        if(otp_count >= 4)
        {
            packet_send_uart(UART_BBG, packet_make(OTP_SEND_BBG_ID, otp_arr, 4, TRUE));
            otp_count = 0;
            otp_flag = 0;
            memset(otp_arr, 0, 4);
            goto label;
        }

        //Delay to prevent debouncing
        delay_ms(500);
    }
    else
    {
        printf("Press Finger Against The Scanner first.\n");
        LCD_write("Press Finger on Scanner");
        GPIOIntClear(COMMON_COLUMN_PORT, int_status);
    }


    label:
    //Clear Interrupt.
    GPIOIntClear(COMMON_COLUMN_PORT, int_status & int_pin);


    IntMasterEnable();
//    GPIOIntRegister(COMMON_COLUMN_PORT, column_handler);

}



void keypad_button_detect(uint32_t column_port, uint32_t column_pin, char keypad_digits[ROWS][COLUMNS],uint8_t column)
{
    GPIOPinWrite(ROW1_PORT, ROW1_PIN, ROW1_PIN);
    if(GPIOPinRead(column_port, column_pin) & column_pin)
    {
        otp_arr[otp_count] = keypad_digits[0][column];
        printf("Button Pressed: %c\n", keypad_digits[0][column]);
        lcd_write_data(otp_arr[otp_count]);
        GPIOPinWrite(ROW1_PORT, ROW1_PIN, 0);
        while(!(GPIOPinRead(column_port, column_pin) & column_pin));
    }
    else
    {
        GPIOPinWrite(ROW1_PORT, ROW1_PIN, 0);
        GPIOPinWrite(ROW2_PORT, ROW2_PIN, ROW2_PIN);
        if(GPIOPinRead(column_port, column_pin) & column_pin)
        {
            otp_arr[otp_count] = keypad_digits[1][column];
            printf("Button Pressed: %c\n", keypad_digits[1][column]);
            lcd_write_data(otp_arr[otp_count]);
            GPIOPinWrite(ROW2_PORT, ROW2_PIN, 0);
            while(!(GPIOPinRead(column_port, column_pin) & column_pin));
        }
        else
        {
            GPIOPinWrite(ROW2_PORT, ROW2_PIN, 0);
            GPIOPinWrite(ROW3_PORT, ROW3_PIN, ROW3_PIN);
            if(GPIOPinRead(column_port, column_pin) & column_pin)
            {
                otp_arr[otp_count] = keypad_digits[2][column];
                printf("Button Pressed: %c\n", keypad_digits[2][column]);
                lcd_write_data(otp_arr[otp_count]);
                GPIOPinWrite(ROW3_PORT, ROW3_PIN, 0);
                while(!(GPIOPinRead(column_port, column_pin) & column_pin));
            }
            else
            {
                GPIOPinWrite(ROW3_PORT, ROW3_PIN, 0);
                GPIOPinWrite(ROW4_PORT, ROW4_PIN, ROW4_PIN);
                if(GPIOPinRead(column_port, column_pin) & column_pin)
                {
                    otp_arr[otp_count] = keypad_digits[3][column];
                    printf("Button Pressed: %c\n", keypad_digits[3][column]);
                    lcd_write_data(otp_arr[otp_count]);
                    GPIOPinWrite(ROW4_PORT, ROW4_PIN, 0);
                    while(!(GPIOPinRead(column_port, column_pin) & column_pin));
                }
            }
        }
    }
}
