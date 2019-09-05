/**
 * @file temp.h
 * @brief Header file for temp.c.
 *
 * The file consists of all the function declarations, macros and typedefs related to Temperature functionality.
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Satya Mehta
 * @date 04/14/2019
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef INC_TEMP_H_
#define INC_TEMP_H_

#include "inc/main.h"


#define CONFIG_TEMP     (0x00)
#define SLAVE_ADDR      (0x48)          //Slave Address of I2C sensor.
#define THRESHOLD       (25)


//Function Declarations
uint8_t temptask_init(void);
static void temp(void *pvParameters);
void temp_send(void);
float temp_read(void);

#endif /* INC_TEMP_H_ */
