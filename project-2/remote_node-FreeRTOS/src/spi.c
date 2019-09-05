/**
 * @file spi.c
 * @brief This file consists of all the functions required for SPI interfacing.
 *
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/16/2019
 *
 * @copyright Copyright (c) 2019
 *
 */


#include "inc/spi.h"



//*********************************************************************************
//Check if spi is same as i2c config. Compare the two spi and i2c config functions
//*********************************************************************************


void spi_config(spi_t ssi_x)
{
    if (ssi_x == SPI0)
    {
        //Enabling GPIO Peripheral
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

        // Enbaling SSI0 Peripheral
        SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
        //    SysCtlPeripheralReset(SYSCTL_PERIPH_SSI0);

        while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA) || !SysCtlPeripheralReady(SYSCTL_PERIPH_SSI0))
        {
        }

        // Configure the pin muxing for SSI0 functions on port A2, A3, A4, and A5.
        // This step is not necessary if your part does not support pin muxing.
        //Corresponds to the pins for I2C 7 and I2C 8 SCL and SDA pins.
        //Booster pack 2
        GPIOPinConfigure(GPIO_PA2_SSI0CLK);
        GPIOPinConfigure(GPIO_PA3_SSI0FSS);
        GPIOPinConfigure(GPIO_PA4_SSI0XDAT0);    //RX (MISO)
        GPIOPinConfigure(GPIO_PA5_SSI0XDAT1);      //TX (MOSI)

        //Configures th GPIO pins as clock, fss, MOSI, MISO
        GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 |
                       GPIO_PIN_2);

        SSIConfigSetExpClk(SSI0_BASE, g_ui32SysClock, SSI_FRF_MOTO_MODE_0,
                           SSI_MODE_MASTER, 1000000, 8);

        // Enable the SSI0 module.
        SSIEnable(SSI0_BASE);
    }

    if (ssi_x == SPI1)
    {
        //Enabling GPIO Peripheral
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

        // Enbaling SSI0 Peripheral
        SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);
        //    SysCtlPeripheralReset(SYSCTL_PERIPH_SSI0);

        while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB) || !SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE) || !SysCtlPeripheralReady(SYSCTL_PERIPH_SSI1))
        {
        }


        // Configure the pin muxing for SSI0 functions on port A2, A3, A4, and A5.
        // This step is not necessary if your part does not support pin muxing.
        //Corresponds to the pins for I2C 7 and I2C 8 SCL and SDA pins.
        //Booster pack 2
        GPIOPinConfigure(GPIO_PB5_SSI1CLK);
        GPIOPinConfigure(GPIO_PB4_SSI1FSS);
        GPIOPinConfigure(GPIO_PE4_SSI1XDAT0);    //RX (MISO)
        GPIOPinConfigure(GPIO_PE5_SSI1XDAT1);      //TX (MOSI)

        //Configures th GPIO pins as clock, fss, MOSI, MISO
        GPIOPinTypeSSI(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_4);
        GPIOPinTypeSSI(GPIO_PORTE_BASE, GPIO_PIN_5 | GPIO_PIN_4);

        SSIConfigSetExpClk(SSI1_BASE, g_ui32SysClock, SSI_FRF_MOTO_MODE_0,
                           SSI_MODE_MASTER, 1000000, 8);

        // Enable the SSI0 module.
        SSIEnable(SSI1_BASE);
    }

    else if (ssi_x == SPI2)
    {
        //Enabling GPIO Peripheral
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

        // Enbaling SSI0 Peripheral
        SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI2);
        //SysCtlPeripheralReset(SYSCTL_PERIPH_SSI0);


        while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD) || !SysCtlPeripheralReady(SYSCTL_PERIPH_SSI2))
        {
        }

        // Configure the pin muxing for SSI0 functions on port A2, A3, A4, and A5.
        // This step is not necessary if your part does not support pin muxing.
        // TODO: change this to select the port/pin you are using.
        GPIOPinConfigure(GPIO_PD3_SSI2CLK);
        GPIOPinConfigure(GPIO_PD2_SSI2FSS);
        GPIOPinConfigure(GPIO_PD1_SSI2XDAT0);   //RX (MISO)
        GPIOPinConfigure(GPIO_PD0_SSI2XDAT1);   //TX (MOSI)

        //Configures the GPIO pins as clock, fss, MOSI, MISO
        GPIOPinTypeSSI(GPIO_PORTD_BASE, GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1 |
                       GPIO_PIN_0);

        SSIConfigSetExpClk(SSI2_BASE, g_ui32SysClock, SSI_FRF_MOTO_MODE_0,
                           SSI_MODE_MASTER, 1000000, 8);

        // Enable the SSI0 module.
        SSIEnable(SSI2_BASE);

    }
}


uint8_t spi_transfer(spi_t spi, uint8_t value)
{
    uint32_t buffer;
    SSIDataPut(spi, (uint32_t)value);
    spi_busy(SPI2);
    SSIDataGet(spi, &buffer);
    //SysCtlDelay(100000);
    return ((uint8_t)(buffer & 0xFF));
}

uint8_t spi_dummy_transfer(spi_t spi)
{
    return spi_transfer(spi, 0xFF);
}


inline void spi_busy(spi_t spi)
{
    while(SSIBusy(spi)){}
}
