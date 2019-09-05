/**
 * @file nrf_driver.h
 * @brief Header file for nrf_driver.c.
 *
 * The file consists of all the function declarations, macros and datatype declarations required for NRF24L01+.
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/16/2019
 *
 * @copyright Copyright (c) 2019
 *
 */


#ifndef INC_NRF_DRIVER_H_
#define INC_NRF_DRIVER_H_


#include "inc/main.h"
#include "inc/spi.h"


#define NRF_SEND

#define LOW     (0)
#define HIGH    (1)
#define SHIFT(y)   (1<<y)

//******************************************************************************************/
//       GPIO PORT and PINS assigned for Chip Enable, Chip Select and Interrupt with NRF24L01+
//******************************************************************************************/
#define NRF_CHIP_ENABLE_PORT    (GPIO_PORTC_BASE)
#define NRF_CHIP_ENABLE_PIN     (GPIO_PIN_7)
#define NRF_CHIP_SELECT_PORT    (GPIO_PORTC_BASE)
#define NRF_CHIP_SELECT_PIN     (GPIO_PIN_6)
#define NRF_IRQ_PORT            (GPIO_PORTL_BASE)
#define NRF_IRQ_PIN             (GPIO_PIN_2)


//******************************************************************************************/
//             GPIO PORT and PINS assigned for SPI interface with NRF24L01+
//******************************************************************************************/
#define NRF_PORT        (GPIO_PORTD_BASE)
#define NRF_CLK_PORT    (GPIO_PORTD_BASE)
#define NRF_CLK_PIN     (GPIO_PIN_3)
#define NRF_FSS_PORT    (GPIO_PORTD_BASE)   /**< This port is used for slave functionality of TIVA*/
#define NRF_FSS_PIN     (GPIO_PIN_2)        /**< This pin is used for slave functionality of TIVA*/
#define NRF_MOSI_PORT   (GPIO_PORTD_BASE)
#define NRF_MOSI_PIN    (GPIO_PIN_0)
#define NRF_MISO_PORT   (GPIO_PORTD_BASE)
#define NRF_MISO_PIN    (GPIO_PIN_1)


//******************************************************************************************/
//                              MASKS
//******************************************************************************************/
#define READ_MASK   (0xFF)
#define WRITE MASK  (0x20)


//******************************************************************************************/
//                          REGISTER MAP
//******************************************************************************************/
#define NRF_CONFIG_REG                      (0x00)      /**< Configuration Register address*/
#define NRF_EN_AA_REG                       (0x01)      /**< AA Register address*/
#define NRF_ENABLE_RXPIPE_ADDRESS_REG       (0x02)      /**< Receiver Pipe Enable Register address*/
#define NRF_SETUP_AW_REG                    (0x03)      /**< Address Width Register address*/
#define NRF_SETUP_RETR_REG                  (0x04)      /**< Retransmission Register address*/
#define NRF_SETUP_RFCHANNEL_REG             (0x05)      /**< RF Channel Setup Register address*/
#define NRF_RFSETUP_REG                     (0x06)      /**< RF Setup Register address*/
#define NRF_STATUS_REG                      (0x07)      /**< Status Register address*/
#define NRF_RX_ADDR_P0_REG                  (0x0A)      /**< Register address containing Receiver Pipe0 Address*/
#define NRF_RX_ADDR_P1_REG                  (0x0B)      /**< Register address containing Receiver Pipe1 Address*/
#define NRF_RX_ADDR_P2_REG                  (0x0C)      /**< Register address containing Receiver Pipe2 Address*/
#define NRF_RX_ADDR_P3_REG                  (0x0D)      /**< Register address containing Receiver Pipe3 Address*/
#define NRF_RX_ADDR_P4_REG                  (0x0E)      /**< Register address containing Receiver Pipe4 Address*/
#define NRF_RX_ADDR_P5_REG                  (0x0F)      /**< Register address containing Receiver Pipe5 Address*/
#define NRF_TX_ADDR_REG                     (0x10)      /**< Register address containing Transmitter Pipe Address*/
#define NRF_RX_BYTES_P0_REG                 (0x11)      /**< Register address containing Number of Data Bytes in Receiver Pipe0*/
#define NRF_RX_BYTES_P1_REG                 (0x12)      /**< Register address containing Number of Data Bytes in Receiver Pipe1*/
#define NRF_RX_BYTES_P2_REG                 (0x13)      /**< Register address containing Number of Data Bytes in Receiver Pipe2*/
#define NRF_RX_BYTES_P3_REG                 (0x14)      /**< Register address containing Number of Data Bytes in Receiver Pipe3*/
#define NRF_RX_BYTES_P4_REG                 (0x15)      /**< Register address containing Number of Data Bytes in Receiver Pipe4*/
#define NRF_RX_BYTES_P5_REG                 (0x16)      /**< Register address containing Number of Data Bytes in Receiver Pipe5*/
#define NRF_FIFO_STATUS_REG                 (0x17)      /**< FIFO Status Register Address*/


//******************************************************************************************/
//                          REGISTER MAP BIT MASKS
//******************************************************************************************/
#define RF_DR_LOW250   (5)     /**< Bit to set data rate to 250kbps*/
#define MASK_RX_DR                          (0b01000000)    /**< Mask Interrupt caused by RX_DR*/

#define RF_DR_LOW                           (0b00100000)

#define NRF_RX_DR_MASK                      (0x40)          /**< Mask for RX data receive*/
#define NRF_TX_DS_MASK                      (0x20)          /**< Mask for TX data send*/
#define NRF_MAX_RT_MASK                     (0x10)          /**< Mask for Maximum Retries*/



//******************************************************************************************/
//                          REGISTER COMMANDS
//******************************************************************************************/
#define R_REGISTER_CMD(y)                   (0x1F & y)  /**< Register Read Command with Register address as Parameter*/
#define W_REGISTER_CMD(y)                   (0x20 | y)  /**< Register Write Command with Register address as Parameter*/
#define NOP_CMD                             (0xFF)      /**< No Operation Command which can be used to get status Register*/
#define FLUSH_TX_CMD                        (0xE1)      /**< Flush TX FIFO Command, used in Tx Mode*/
#define FLUSH_RX_CMD                        (0xE2)      /**< Flush RX FIFO Command, used in Rx Mode*/
#define R_RX_PAYLOD_CMD                     (0x61)      /**< Read RX Payload, Payload deleted after reading, used in RX Mode*/
#define W_TX_PAYLOD_CMD                     (0xA0)      /**< Read RX Payload, used in TX Mode*/



//******************************************************************************************/
//                          VARIABLE DECLARATIONS
//******************************************************************************************/
typedef struct
{
    uint8_t tx_pipe_address[5];                            /**< Array containing TX Pipe address, First byte is the LSB. */
}TX;

typedef struct
{
        uint8_t rx_pipe_address[5];                        /**< Array containing RX Pipe0 address, first byte is the LSB */
}RX;


typedef enum
{
    mode_tx = 1,
    mode_rx = 2
}modes_t;



//******************************************************************************************/
//                          FUNCTION DECLARATIONS
//******************************************************************************************/
/**
 * @brief This function initializes all the GPIOs required by the NRF25L01+.
 *
 * The Pins are: Chip Enable, Chip Select and IRQ pin.
 * @return void
 */
void nrf_gpio_init(void);


/**
 * @brief This function configures and enables the GPIO interrupt for NRF24L01+.
 * @return void.
 */
void nrf_interrupt_enable(void);

/**
 * @brief This function writes to a register in the NRF24L01+.
 *
 * @param spi The SPI line used.
 * @param reg The Register value to write to.
 * @param value The value to write to the Register.
 * @return void.
 */
void nrf_write_reg(spi_t spi, uint8_t reg, uint8_t value);


/**
 * @brief This function reads the value from the register specified by the parameter @b reg.
 * @param spi The SPI line used.
 * @param reg The Register value to read data from.
 * @return uint8_t The value read from the register @b reg.
 */
uint8_t nrf_read_reg(spi_t spi, uint8_t reg);


/**
 * @brief This function configures the register value for NRF24L01+.
 * @return void.
 */
void nrf_config(void);


/**
 * @brief This function configures the NRF24L01+ in TX or RX Mode.
 * @param spi The SPI line used.
 * @param mode The mode specified by the user.
 */
void nrf_config_mode(spi_t spi, modes_t mode);



/**
 * @brief Callback Function for NRF24L01+ Interrupt.
 *
 * This function is called whenever there is an interrupt on the IRQ pin of NRF24L01+.
 * The interrupt is due to data received and transmitted.
 * @return void.
 */
void nrf_irqhandler(void);


/**
 * @brief This function sends the packet data.
 * @param spi The SPI line used.
 */
void nrf_packet_send(spi_t spi);


/**
 * @brief This function receives the packet data.
 * @param spi The SPI line used.
 */
void nrf_packet_rcv(spi_t spi);


/**
 * @brief This function assigns the specified TX pipe address.
 * @param spi The SPI line used
 * @param addr The object of structure TX.
 */
void nrf_write_TXaddr(spi_t spi, TX addr);


/**
 * @brief This function reads the specified TX Pipe address.
 * @param spi The SPI line used.
 */
void nrf_read_TXaddr(spi_t spi);

/**
 * @brief This function assigns the specified RX pipe address.
 * @param spi The SPI line used.
 * @param reg The Register address of the PIPE.
 * @param addr The object of structure RX.
 */
void nrf_write_RXaddr(spi_t spi, uint8_t reg, RX addr);


/**
 * @brief This function reads the specified RX pipe address
 * @param spi The SPI line used.
 * @param reg The Register address of the PIPE.
 */
void nrf_read_RXaddr(spi_t spi, uint8_t reg);




/**
 * @brief This function changes the status of chip enable(CE) GPIO pin.
 * @param status This value is a macro HIGH or LOW which specifies the pin to go high or low.
 * @return void
 */
inline void ce_status(uint8_t status)
{
    if (status)
    {
        GPIOPinWrite(NRF_CHIP_ENABLE_PORT, NRF_CHIP_ENABLE_PIN, NRF_CHIP_ENABLE_PIN);
    }
    else
    {
        GPIOPinWrite(NRF_CHIP_ENABLE_PORT, NRF_CHIP_ENABLE_PIN, 0);
    }
}

/**
 * @brief This function changes the status of Chip Select(CS) GPIO pin.
 * @param status This value is a macro HIGH or LOW which specifies the pin to go high or low.
 * @return void.
 */
inline void csn_status(uint8_t status)
{
    if (status)
    {
        GPIOPinWrite(NRF_CHIP_SELECT_PORT, NRF_CHIP_SELECT_PIN, NRF_CHIP_SELECT_PIN);
    }
    else
    {
        GPIOPinWrite(NRF_CHIP_SELECT_PORT, NRF_CHIP_SELECT_PIN, 0);
    }
}


/**
 * @brief Thuis function returns the status register contents.
 * @param spi The SPI line used.
 * @return uint8_t The contents of Status register.
 */
inline uint8_t nrf_get_statusreg(spi_t spi)
{
    return spi_dummy_transfer(spi);
}

/**
 * @brief This function writes a command to the NRF24L01+.
 * @param spi The SPI line used.
 * @param cmd The Command MACRO.
 */
inline uint8_t nrf_write_cmd(spi_t spi, uint8_t cmd)
{
        spi_transfer(spi, cmd);
        return (spi_dummy_transfer(spi));
}


//inline bool nrf_set_rfsetupreg(spi_t, uint8_t value)
//{
//    nrf_write_reg(SPI2, W_REGISTER_CMD(NRF_RFSETUP_REG), value);
//}

#endif /* INC_NRF_DRIVER_H_ */
