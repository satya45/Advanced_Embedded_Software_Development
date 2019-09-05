/**
 * @file spi.h
 * @brief Header file for spi.c.
 *
 * The file consists of all the function declarations, macros and datatype declarations related to SPI functionality.
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/16/2019
 *
 * @copyright Copyright (c) 2019
 *
 */


#ifndef INC_SPI_H_
#define INC_SPI_H_

#include "inc/main.h"
#include "driverlib/ssi.h"


//******************************************************************************************/
//                          VALUES FOR SPI_T
//******************************************************************************************/
//#define SPI0   (SSI0_BASE)
//#define SPI1   (SSI1_BASE)
//#define SPI2   (SSI2_BASE)

typedef enum
{
   SPI0 = SSI0_BASE,
   SPI1 = SSI1_BASE,
   SPI2 = SSI2_BASE

}spi_t;
//******************************************************************************************/
//                          TYPEDEF FOR UINT32_T (SPI)
//******************************************************************************************/
//typedef uint32_t spi_t;


//******************************************************************************************/
//                          FUNCTION DECLARATIONS
//******************************************************************************************/

/**
 * @brief This function configures the SPI line as per the parameters.
 * @param ssi_x The SPI line used.
 * @return void
 */
void spi_config(spi_t ssi_x);


/**
 * @brief This function transfers a value on the MOSI line and reads the data on the MISO line.
 *
 * @param spi The SPI line used.
 * @param value The value transfered on the SPI line. This can be a register address or the value to
 *  be written to the register value.
 * @return uint8_t The value read from the MISO line on the specified SPI.
 */
uint8_t spi_transfer(spi_t spi, uint8_t value);


/**
 * @brief This function transfers a NO OPERATION Command and reads data from the MISO line.
 *
 * @param spi The SPI line used.
 * @return uint8_t The value read from the MISO line on the specified SPI.
 */
uint8_t spi_dummy_transfer(spi_t spi);


/**
 * @brief This function checks the SPI line if it is busy or not.
 * @param spi The SPI line used.
 * @return void.
 */
inline void spi_busy(spi_t spi);


#endif /* INC_SPI_H_ */
