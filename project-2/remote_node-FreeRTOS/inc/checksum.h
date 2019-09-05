/**
 * @file checksum.h
 * @brief Header file for checksum.c.
 *
 * The file consists of all the function declarations, macros and typedefs related to Checksum.
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/20/2019
 * @see https://barrgroup.com/Embedded-Systems/How-To/CRC-Calculation-C-Code
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef INC_CHECKSUM_H_
#define INC_CHECKSUM_H_


#include "inc/main.h"


typedef uint16_t crc;
crc  crcTable[256];

#define WIDTH  (8 * sizeof(crc))
#define TOPBIT (1 << (WIDTH - 1))
#define POLYNOMIAL (0xD8)


/**
 * @brief This function initializes the Checksum functionality and stores all the possible combinations in a table.
 * @return void.
 */
void checksum_init(void);


/**
 * @brief This function calculates the Checksum for the specified data.
 * @param message The array containing the Message Data.
 * @param nBytes The Number of bytes in the Array.
 * @return crc.
 */
crc checksum_calc(uint8_t const message[], int nBytes);


#endif /* INC_CHECKSUM_H_ */
