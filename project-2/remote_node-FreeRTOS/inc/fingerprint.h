/**
 * @file fingerprint.h
 * @brief Header file for fingerprint.c.
 *
 * The file consists of all the function declarations, macros and typedefs related to Fingerprint functionality.
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/19/2019
 *
 * @copyright Copyright (c) 2019
 *
 */

#ifndef INC_FINGERPRINT_H_
#define INC_FINGERPRINT_H_

#include "inc/main.h"
#include "inc/myuart.h"




//******************************************************************************************/
//                     Size for Command, Acknowledge and Data Packets
//******************************************************************************************/
#define COMMAND_SIZE            (12)            /**< Size of the Command Packet*/
#define RESPONSE_SIZE           (12)            /**< Size of the Response Packet*/
//#define DATA_SIZE               ()


//******************************************************************************************/
//                     Packets for Command, Acknowledge and Data
//******************************************************************************************/
#define START_CODE1             (0x55)
#define START_CODE2             (0xAA)
#define DEVICEID_LSBYTE         (0x01)
#define DEVICEID_MSBYTE         (0x00)
#define PARAMETER_BYTES         (0x00)
#define CMD_CODE_MSBYTE         (0x00)
#define CHECKSUM_BYTES          (0x00)


//******************************************************************************************/
//                              COMMANDS FOR GT521F32
//******************************************************************************************/
#define FP_OPEN_CMD             (0x01)
#define FP_CLOSE_CMD            (0x02)
#define FP_BAUDRATE_CMD         (0x04)
#define FP_CMOSLED_CMD          (0x12)
#define FP_GETENROLLCOUNT_CMD   (0x20)
#define FP_CHECKENROLLED_CMD    (0x21)
#define FP_ENROLLSTART_CMD      (0x22)
#define FP_ENROLL1_CMD          (0x23)
#define FP_ENROLL2_CMD          (0x24)
#define FP_ENROLL3_CMD          (0x25)
#define FP_ISPRESSFINGER_CMD    (0x26)
#define FP_DELETEID_CMD         (0x40)
#define FP_DELETEALL_CMD        (0x41)
#define FP_VERIFY_CMD           (0x50)
#define FP_IDENTIFY_CMD         (0x51)
#define FP_CAPTUREFINGER_CMD    (0x60)
#define FP_GETIMAGE_CMD         (0x62)
#define FP_GETRAWIMAGE_CMD      (0x63)
#define ACK                     (0x30)
#define NACK                    (0x31)


//******************************************************************************************/
//                              PARAMETERS FOR COMMAND PACKET.
//******************************************************************************************/
#define FP_LEDON                (1)                 /**< Parameter Value to switch on the LED*/
#define FP_LEDOFF               (0)                 /**< Parameter Value to switch off the LED*/
#define FP_NOEXTRA_INFO         (0)                 /**< Parameter Value for OPEN command*/
#define FP_NOPARAM              (0)                 /**< Parameter Value for no parameter*/
#define FP_BEST_IMAGE           (1)                 /**< Parameter Value to get High Resolution Fingerprint Image, Use at the time of enrolling fingerprint*/
#define FP_LOW_IMAGE            (0)                 /**< Parameter Value to get fast but low Resolution Fingerprint Image, Use at the time of verifying fingerprint*/
#define FP_FINGER_PRESSED       (0)                  /**< Parameter Value in response indicating finger is pressed.*/



//******************************************************************************************/
//                                  ERROR CODES
//******************************************************************************************/
#define FP_NACK_DB_IS_EMPTY     (0x100A)
#define FP_NACK_IDENTIFY_FAILED (0x1008)


//******************************************************************************************/
//                     GPIO PORT and PINS assigned for ICPCK with GT-521F32
//******************************************************************************************/
#define FP_IRQ_PORT            (GPIO_PORTL_BASE)        /**< Port for Fingerprint sensor interrupt*/
#define FP_IRQ_PIN             (GPIO_PIN_3)             /**< Pin for Fingerprint sensor interrupt*/



//******************************************************************************************/
//                              FUNCTION DECLARATIONS
//******************************************************************************************/

/**
 * @brief This function initializes and enables the required GPIO pins interrupts for fingerprint sensor GT-521F32.
 * @return void.
 */
void fp_interrupt_config(void);


/**
 * @brief This function sends the command to the Fingerprint Sensor GT-521F32 via UART.
 * @param uart The UART line used.
 * @param cmd The Command macro defined in fingerprint.h
 * @param param The parameter value required while sending the particular command.
 * @return void.
 */
void fp_cmdsend(uart_t uart, uint8_t cmd, uint8_t param);


/**
 * @brief This function receives the response packet from the Fingerprint Sensor GT-521F32 and returns the output parameter/error code.
 * @param uart The UART line being used.
 * @return uint16_t The error code or output parameter in case of NACK/ACK.
 */
uint16_t fp_responsercv(uart_t uart);


/**
 * @brief This function calculates the checksum as specfied in the datasheet of Fingerprint GT-521F32.
 * @param packet The Response/Command packet array as per the datasheet of GT-521F32.
 * @return uint16_t The calculated checksum value.
 */
uint16_t fp_checksum_calc(uint8_t packet[]);


/**
 * @brief This function changes the baudrate. The default baudrate is 9600.
 * @param uart The UART line used.
 * @param baudrate The baudrate MACRO:
 * @return uint16_t The error code or output parameter in case of NACK/ACK respectively.
 */
uint16_t fp_baudrate(uart_t uart, uint16_t baudrate);


/**
 * @brief This function Starts the fingerprint Enrollment process.
 * @param uart The UART lune used.
 * @param new_id The ID number for the fingerprint.
 * @return uint16_t The error code or output parameter in case of NACK/ACK respectively.
 */
uint16_t fp_start_enroll(uart_t uart, uint16_t new_id);


/**
 * @brief This function enrolls the fingerprint.
 * @param uart The UART line used.
 * @param enroll_cmd The Enrollment Command Macro: FP_ENROLL1_CMD, FP_ENROLL2_CMD, FP_ENROLL3_CMD.
 * @return void.
 */
void fp_enroll(uart_t uart, uint8_t enroll_cmd);


/**
 * @brief This function adds a fingerprint.
 * @param uart The UART line used.
 * @return void.
 */
void add_fingerprint(uart_t uart);


/**
 * @brief This function deletes the specified Fingerprint ID.
 * @param uart The UART line used.
 * @param id The fingerprint ID number.
 * @return uint16_t The error code or output parameter in case of NACK/ACK respectively.
 */
uint16_t fp_deleteid(uart_t uart, uint16_t id);


/**
 * @brief This function verifies the latest captured fingerprint.
 * @param uart The UART line used.
 * @param id The ID to be compared with.
 * @return uint16_t The error code or output parameter in case of NACK/ACK respectively.
 */
uint16_t fp_verify(uart_t uart, uint16_t id);



/**
 * @brief This function identifies the captured fingerprint from the stored fingerprints and returns the ID.
 * @param uart The UARt line used.
 * @return uint16_t The error code or output parameter in case of NACK/ACK respectively.
 */
uint16_t fp_identify(uart_t uart);


/**
 * @brief Callback function for Fingerprint Interrupt Handler.
 * @return void.
 */
void fp_irqhandler(void);


/**
 * @brief This function was used for testing purposes.
 * @return void.
 */
void fingerprint_test(void);


/**
 * @brief This function initializes the Scanner and turns LED on.
 * @return void.
 */
void fp_init(void);


/**
 * @brief This function enables the interrupts for fingerprint sensor.
 * @return void.
 */
inline void fp_interrupt_enable(void)
{
    GPIOIntEnable(FP_IRQ_PORT, FP_IRQ_PIN);
}



/**
 * @brief This function disables the interrupts for fingerprint sensor.
 * @return void.
 */
inline void fp_interrupt_disable(void)
{
    GPIOIntDisable(FP_IRQ_PORT, FP_IRQ_PIN);
}



/**
 * @brief This function is used to initialize and get the device's static information.
 * @return uint16_t The error code or output parameter in case of NACK/ACK.
 */
inline uint16_t fp_open(uart_t uart)
{
    fp_cmdsend(uart, FP_OPEN_CMD, FP_NOEXTRA_INFO);
    return (fp_responsercv(uart));
}


/**
 * @brief This function switches the LED on or off.
 * @param uart The UART line used.
 * @param status The FP_LEDON or FP_LEDOFF Macro to switch the LED on or off.
 * @return uint16_t The error code or output parameter in case of NACK/ACK.
 */
inline uint16_t fp_led_status(uart_t uart, uint8_t status)
{
    fp_cmdsend(UART_FP, FP_CMOSLED_CMD, FP_LEDON);
    return (fp_responsercv(uart));
}



/**
 * @brief This function return the Number of Fingerprints enrolled in the Fingerprint Sensor GT-521F32.
 * @param uart The UARt line used.
 * @return uint16_t The error code or output parameter in case of NACK/ACK respectively.
 */
inline uint16_t fp_get_enrollcount(uart_t uart)
{
    uint16_t enroll_count;
    //Get enrollment count
    fp_cmdsend(uart, FP_GETENROLLCOUNT_CMD, FP_NOPARAM);
    enroll_count = fp_responsercv(uart);
    printf("Enrollment Count : %d.\n", enroll_count);
    return (enroll_count);
}


/**
 * @brief This function checks whether the finger is pressed against the scanner.
 * @param uart The UART line used.
 * @return uint16_t The error code or output parameter in case of NACK/ACK respectively.
 */
inline uint16_t fp_isfingerpress(uart_t uart)
{
    fp_cmdsend(uart, FP_ISPRESSFINGER_CMD, FP_NOPARAM);
    return (fp_responsercv(uart));
}


/**
 * @brief This function Captures the fingerprint.
 * @param uart The UART line used.
 * @param resolution The resolution for the fingerprint scan. The macros are: FP_BEST_IMAGE, FP_LOW_IMAGE.
 * @return uint16_t The error code or output parameter in case of NACK/ACK respectively.
 */
inline uint16_t fp_capture(uart_t uart, uint8_t resolution)
{
    fp_cmdsend(uart, FP_CAPTUREFINGER_CMD, resolution);
    return (fp_responsercv(uart));
}



/**
 * @brief This function deletes all the Registered Fingerprints.
 * @param uart The UART line used.
 * @return uint16_t The error code or output parameter in case of NACK/ACK respectively.
 */
inline uint16_t fp_deleteall(uart_t uart)
{
    fp_cmdsend(uart, FP_DELETEALL_CMD, FP_NOPARAM);
    return (fp_responsercv(uart));
}


/**
 * @brief This function terminates. Virtually does nothing.
 * @return uint16_t The error code or output parameter in case of NACK/ACK.
 */
inline uint16_t fp_close(uart_t uart)
{
    fp_cmdsend(uart,  FP_CLOSE_CMD, FP_NOPARAM);
    return (fp_responsercv(uart));
}

#endif /* INC_FINGERPRINT_H_ */
