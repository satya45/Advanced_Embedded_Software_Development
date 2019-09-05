/**
 * @file checksum.c
 * @brief This file consists of all the functions required for Checksum.
 *
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/20/2019
 * @see https://barrgroup.com/Embedded-Systems/How-To/CRC-Calculation-C-Code
 *
 * @copyright Copyright (c) 2019
 *
 */



#include "inc/checksum.h"


void checksum_init(void)
{
    crc  remainder;
    uint8_t bit;
    int dividend;

    /*
     * Compute the remainder of each possible dividend.
     */
    for (dividend = 0; dividend < 256; ++dividend)
    {
        /*
         * Start with the dividend followed by zeros.
         */
        remainder = dividend << (WIDTH - 8);

        /*
         * Perform modulo-2 division, a bit at a time.
         */
        for (bit = 8; bit > 0; --bit)
        {
            /*
             * Try to divide the current data bit.
             */
            if (remainder & TOPBIT)
            {
                remainder = (remainder << 1) ^ POLYNOMIAL;
            }
            else
            {
                remainder = (remainder << 1);
            }
        }

        /*
         * Store the result into the table.
         */
        crcTable[dividend] = remainder;
    }

}


crc checksum_calc(uint8_t const message[], int nBytes)
{
    uint8_t data;
    crc remainder = 0;
    int byte;

    /*
     * Divide the message by the polynomial, a byte at a time.
     */
    for (byte = 0; byte < nBytes; ++byte)
    {
        data = message[byte] ^ (remainder >> (WIDTH - 8));
        remainder = crcTable[data] ^ (remainder << 8);
    }

    /*
     * The final remainder is the CRC.
     */
    return (remainder);

}
