/**
 * @file main.h
 * @brief Header file for main.c.
 *
 * The file consists of all the function declarations, macros and datatype declarations.
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/16/2019
 *
 * @copyright Copyright (c) 2019
 *
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/uart.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "sensorlib/i2cm_drv.h"
#include "driverlib/i2c.h"
#include "utils/uartstdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "drivers/pinout.h"
#include "drivers/buttons.h"



#define LEDTASKSTACKSIZE        (128)
#define printf                  (UARTprintf)
#define CLOCK                   (16000000)
#define TRUE                    (1)
#define FALSE                   (0)
#define FINGERPRINT_MATCHED     (1)
#define FINGERPRINT_NOTMATCHED  (0)
#define DONT_CARE               (0)

//******************************************************************************************/
//                                MACROS FOR DIFFERENT EVENTS
//******************************************************************************************/
#define RANDOM_TEST_ID              (0)
#define FINGERPRINT_ID              (1)
#define OTP_SENT_USER_ID            (2)
#define OTP_SEND_BBG_ID             (3)
#define ACCESS_STATUS_RCV_ID        (4)
#define MSG_LOG_ID                  (5)
#define ACK_ID                      (6)
#define GUI_ID                      (7)
#define TEMP_RCV_ID                 (10)
#define LED_RCV_ID                  (11)


//******************************************************************************************/
//                                 GUI PAYLOAD RECEIVE MACROS
//******************************************************************************************/
#define GUI_BUZZER_OFF              (1)             /**< Buzzer OFF Request from BBG GUI*/
#define GUI_ADD_FINGERPRINT         (2)             /**< Add Fingerprint Request from BBG GUI*/
#define GUI_BUZZER_ON               (3)             /**< Buzzer ON Request from BBG GUI*/
#define GUI_DELETE_FINGERPRINT_ALL  (4)             /**< Delete Fingerprint Request from BBG GUI*/
#define GUI_RESET_SYSTEM            (5)             /**< Reset System Request from GUI*/
#define GUI_ALLOW_ACCESS            (6)             /**< Allow Access Request from GUI*/

//******************************************************************************************/
//                                  GLOBAL VARIABLES
//******************************************************************************************/
uint32_t g_ui32SysClock;                        /**< This variable holds the system clock*/
volatile uint8_t otp_flag;                      /**< This flag allows data to be stored in the array otp_arr only when OTP has been sent to user*/
volatile uint8_t otp_count;                     /**< Keeps a Tab on how many digits have been entered*/
uint8_t otp_arr[4];                             /**< Global array where OTP input by user is stored*/


//******************************************************************************************/
//                              STRUCTURES CONTAINING GLOBAL DATA
//******************************************************************************************/
struct temp
{
    TickType_t current_time;
    float temp;
    char str[50];
    int num;
};


typedef struct
{
    uint8_t id;
    union
    {
        struct temp temp_data;
    }data;
}event;



//******************************************************************************************/
//                              FUNCTION DECLARATIONS
//******************************************************************************************/

/**
 * @brief Configures the UART and its pins.This must be called before UARTprintf().
 * @return void
 */
void ConfigureUART(void);



#endif /* INC_MAIN_H_ */
