/**
 * @file nrf_driver.c
 * @brief This file consists of all the functions required for NRF24L01+ driver.
 *
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/12/2019
 *
 * @copyright Copyright (c) 2019
 *
 */


#include "inc/nrf_driver.h"


void nrf_gpio_init(void)
{

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    // Check if the peripheral access is enabled.
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC))
    {
    }

    GPIOPinTypeGPIOOutput(NRF_CHIP_ENABLE_PORT, NRF_CHIP_ENABLE_PIN);
    GPIOPinTypeGPIOOutput(NRF_CHIP_SELECT_PORT, NRF_CHIP_SELECT_PIN);

//    ce_status(LOW);

    nrf_interrupt_enable();

}

void nrf_interrupt_enable(void)
{

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOL))
    {
    }

    GPIOPinTypeGPIOInput(NRF_IRQ_PORT, NRF_IRQ_PIN);
    GPIOPadConfigSet(NRF_IRQ_PORT, NRF_IRQ_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);

    GPIOIntRegister(NRF_IRQ_PORT, nrf_irqhandler);
    GPIOIntTypeSet(NRF_IRQ_PORT, NRF_IRQ_PIN, GPIO_FALLING_EDGE);

    GPIOIntEnable(NRF_IRQ_PORT, NRF_IRQ_PIN);

    //GPIOIntRegisterPin(NRF_IRQ_PORT, NRF_IRQ_PIN, nrf_irqhandler);


}


void nrf_write_reg(spi_t spi, uint8_t reg, uint8_t value)
{
    csn_status(HIGH);
    csn_status(LOW);

    spi_transfer(spi, reg);
    spi_transfer(spi, value);

    csn_status(HIGH);
}



uint8_t nrf_read_reg(spi_t spi, uint8_t reg)
{
    uint8_t reg_value;
    csn_status(HIGH);
    csn_status(LOW);

    spi_transfer(spi, reg);
    reg_value = spi_dummy_transfer(spi);

    csn_status(HIGH);

    return (reg_value);
}


void nrf_config(void)
{
    uint8_t res;

    //Enable all interrupts
//    nrf_write_reg(SPI2, W_REGISTER_CMD(NRF_CONFIG_REG), 0x0A);
//    res = nrf_read_reg(SPI2, R_REGISTER_CMD(NRF_CONFIG_REG));
//    printf("CONFIG REGISTER = 0x%x\n", res);

    //Enable Auto acknowledgments for Pipe 0
//    nrf_write_reg(SPI2, W_REGISTER_CMD(NRF_EN_AA_REG), 0x01);
//    res = nrf_read_reg(SPI2, R_REGISTER_CMD(NRF_EN_AA_REG));
//    printf("EN_AA REGISTER = 0x%x\n", res);

    //Enable RX Pipe 0
    nrf_write_reg(SPI2, W_REGISTER_CMD(NRF_ENABLE_RXPIPE_ADDRESS_REG), 0x01);
    res = nrf_read_reg(SPI2, R_REGISTER_CMD(NRF_ENABLE_RXPIPE_ADDRESS_REG));
    printf("PO_RX REGISTER = 0x%x\n", res);

    //Setup Address Width
//    nrf_write_reg(SPI2, W_REGISTER_CMD(NRF_SETUP_AW_REG), 0x03);
//    res = nrf_read_reg(SPI2, R_REGISTER_CMD(NRF_SETUP_AW_REG));
//    printf("ADDRESS WIDTH REGISTER = 0x%x\n", res);

    //Setup Retransmission count
//    nrf_write_reg(SPI2, W_REGISTER_CMD(NRF_SETUP_RETR_REG), 0x08);
//    res = nrf_read_reg(SPI2, R_REGISTER_CMD(NRF_SETUP_RETR_REG));
//    printf("RETRANSMISSION REGISTER = 0x%x\n", res);

    //Speed and power setup
//    nrf_write_reg(SPI2, W_REGISTER_CMD(NRF_RFSETUP_REG), 0x26);
//    res = nrf_read_reg(SPI2, R_REGISTER_CMD(NRF_RFSETUP_REG));
//    printf("SPEED AND POWER = %x\n", res);
////    printf("Value read: %x.\n", nrf_read_reg(SPI2, R_REGISTER_CMD(NRF_RFSETUP_REG)));

    //Frequency setup channel
//    nrf_write_reg(SPI2, W_REGISTER_CMD(NRF_SETUP_RFCHANNEL_REG), 0x22);
//    res = nrf_read_reg(SPI2, R_REGISTER_CMD(NRF_SETUP_RFCHANNEL_REG));
//    printf("RF Channel = %x\n", res);

    //Clear Status Register
    //nrf_write_reg(SPI2, W_REGISTER_CMD(NRF_STATUS_REG), 0x00);

#ifdef NRF_SEND
    //Write tx pipe address
    TX pipe;
    pipe.tx_pipe_address[0] = 0x06;    //LSB
    pipe.tx_pipe_address[1] = 0x00;
    pipe.tx_pipe_address[2] = 0x00;
    pipe.tx_pipe_address[3] = 0x00;
    pipe.tx_pipe_address[4] = 0x00;

    nrf_write_TXaddr(SPI2, pipe);
    nrf_read_TXaddr(SPI2);

    //Write rx pipe address
    RX pipe1;
    pipe1.rx_pipe_address[0] = 0x06;    //LSB
    pipe1.rx_pipe_address[1] = 0x00;
    pipe1.rx_pipe_address[2] = 0x00;
    pipe1.rx_pipe_address[3] = 0x00;
    pipe1.rx_pipe_address[4] = 0x00;

    nrf_write_RXaddr(SPI2, NRF_RX_ADDR_P0_REG, pipe1);
    nrf_read_RXaddr(SPI2, NRF_RX_ADDR_P0_REG);

    nrf_write_reg(SPI2, W_REGISTER_CMD(NRF_RX_BYTES_P0_REG), 0x04);
    res = nrf_read_reg(SPI2, R_REGISTER_CMD(NRF_RX_BYTES_P0_REG));
    printf("RX Bytes = %x\n", res);


    nrf_config_mode(SPI2, mode_tx);

#else

    //Write tx pipe address
    TX pipe;
    pipe.tx_pipe_address[0] = 2;    //LSB
    pipe.tx_pipe_address[1] = 0;
    pipe.tx_pipe_address[2] = 0;
    pipe.tx_pipe_address[3] = 0;
    pipe.tx_pipe_address[4] = 0;

    nrf_write_TXaddr(SPI2, pipe);
    nrf_read_TXaddr(SPI2);

    //Write rx pipe address
    RX pipe1;
    pipe1.rx_pipe_address[0] = 1;    //LSB
    pipe1.rx_pipe_address[1] = 0;
    pipe1.rx_pipe_address[2] = 0;
    pipe1.rx_pipe_address[3] = 0;
    pipe1.rx_pipe_address[4] = 0;

    nrf_write_RXaddr(SPI2, NRF_RX_ADDR_P0_REG, pipe1);
    nrf_read_RXaddr(SPI2, NRF_RX_ADDR_P0_REG);


#endif
}


void nrf_config_mode(spi_t spi, modes_t mode)
{
    if (mode == mode_tx)
    {
        uint8_t res;

        //Flush TX FIFO Data
        nrf_write_cmd(SPI2, FLUSH_TX_CMD);

        //Changing config register to enable TX mode
//        nrf_write_reg(spi, W_REGISTER_CMD(NRF_CONFIG_REG), 0x0A);
//        res = nrf_read_reg(SPI2, R_REGISTER_CMD(NRF_CONFIG_REG));
//        printf("config tx = %x\n", res);


//        Enabling Acknowledgements
        nrf_write_reg(spi, W_REGISTER_CMD(NRF_EN_AA_REG), 0x01);
        res = nrf_read_reg(SPI2, R_REGISTER_CMD(NRF_EN_AA_REG));
        printf("AA = %x\n", res);

        nrf_write_reg(SPI2, W_REGISTER_CMD(NRF_CONFIG_REG), 0x0A);
        res = nrf_read_reg(SPI2, R_REGISTER_CMD(NRF_CONFIG_REG));
        printf("CONFIG REGISTER = 0x%x\n", res);

        //Loading data to transmit in the FIFO
        nrf_packet_send(spi);

        //SysCtlDelay(32000);

        //Sending data on high pulse.
        ce_status(HIGH);

        SysCtlDelay(32000);
//        for(k = 0; k < 20; k++);
//        ce_status(LOW);

    }
    else if (mode == mode_rx)
    {
//        uint8_t res;

        //Flush TX FIFO Data
        nrf_write_cmd(SPI2, FLUSH_RX_CMD);

//        nrf_write_reg(SPI2, W_REGISTER_CMD(NRF_RX_BYTES_P0_REG), 0x04);
//        res = nrf_read_reg(SPI2, R_REGISTER_CMD(NRF_RX_BYTES_P0_REG));
//        printf("RX Bytes = %x\n", res);

        //Changing config register to enable RX mode
        nrf_write_reg(spi, W_REGISTER_CMD(NRF_CONFIG_REG), 0x0B);


        ce_status(HIGH);
        SysCtlDelay(32000);

    }
}

void nrf_packet_send(spi_t spi)
{
//    nrf_write_reg(spi, W_REGISTER_CMD(NRF_CONFIG_REG), 0x0A);

    csn_status(HIGH);
    csn_status(LOW);

    //    SSIDataPut(spi, W_TX_PAYLOD_CMD);
    //    while(SSIBusy(spi)){}

    uint8_t x = spi_transfer(spi, W_TX_PAYLOD_CMD);
    //    printf("packet send1 = %x\n", x);

    spi_transfer(spi, 0x0A);
    spi_transfer(spi, 0x0A);
    spi_transfer(spi, 0xF0);
    spi_transfer(spi, 0xF0);
    //    printf("packet send2 = %x\n", x);


//    uint32_t buffer;
//    SSIDataGet(spi, &buffer);
//    printf("packet send2 = %x\n", buffer);


    //    nrf_write_cmd(SPI2, W_TX_PAYLOD_CMD);

    csn_status(HIGH);
//    ce_status(HIGH);
    SysCtlDelay(10000);
}


void nrf_packet_rcv(spi_t spi)
{
    uint8_t result;
    csn_status(HIGH);
    csn_status(LOW);

    spi_transfer(SPI2, R_RX_PAYLOD_CMD);
    result = spi_transfer(SPI2, 0xFF);

//    result = nrf_write_cmd(SPI2, R_RX_PAYLOD_CMD);

    printf("\n\n\n Received value over nrf is : %x", result);

    csn_status(HIGH);
}



void nrf_write_TXaddr(spi_t spi, TX addr)
{
    uint8_t i;

    csn_status(HIGH);
    csn_status(LOW);

    spi_transfer(spi, W_REGISTER_CMD(NRF_TX_ADDR_REG));
    for (i = 0; i < 5; i++)
    {
    spi_transfer(spi, addr.tx_pipe_address[i]);
    }

    csn_status(HIGH);
}

void nrf_read_TXaddr(spi_t spi)
{
    uint8_t i;
    uint8_t reg_value[5];
    csn_status(HIGH);
    csn_status(LOW);

    spi_transfer(spi, R_REGISTER_CMD(NRF_TX_ADDR_REG));

    for (i = 0; i < 5; i++)
    {
        reg_value[i] = spi_dummy_transfer(spi);
        printf("\n Address = %x", reg_value[i]);
    }
    csn_status(HIGH);
}


void nrf_write_RXaddr(spi_t spi, uint8_t reg, RX addr)
{
    uint8_t i;

    csn_status(HIGH);
    csn_status(LOW);

    spi_transfer(spi, W_REGISTER_CMD(reg));
    for (i = 0; i < 5; i++)
    {
        spi_transfer(spi, addr.rx_pipe_address[i]);
    }

    csn_status(HIGH);
}


void nrf_read_RXaddr(spi_t spi, uint8_t reg)
{

    uint8_t i;
    uint8_t reg_value[5];
    csn_status(HIGH);
    csn_status(LOW);

    spi_transfer(spi, R_REGISTER_CMD(reg));

    for (i = 0; i < 5; i++)
    {
        reg_value[i] = spi_dummy_transfer(spi);
        printf("\n Address = %x", reg_value[i]);
    }
    csn_status(HIGH);
}


void nrf_irqhandler(void)
{
    uint32_t int_status;
    uint8_t status;

    IntMasterDisable();
    //Disable the particular pin interrupt.
//    GPIOIntDisable(NRF_IRQ_PORT, NRF_IRQ_PIN);

    //Get Pin Interrupt Status.
    int_status = GPIOIntStatus(NRF_IRQ_PORT, false);

    //Clear Interrupt.
    //
    //DOES THIS CLEAR THE ENTIRE PORT INTERRUPT MIGHT CONFLICT WITH FINGERPRINT INTERRUPT.

    GPIOIntClear(NRF_IRQ_PORT, int_status);

    //*************Insert Delay here if interrupt is triggered continuously or twice sequentially***************/


    status = nrf_read_reg(SPI2, R_REGISTER_CMD(NRF_STATUS_REG));
    printf("status = %x\n", status);

    if (status & NRF_TX_DS_MASK)
    {
        //Clear the interrupt bit due to TX in NRF24L01+
        nrf_write_reg(SPI2, W_REGISTER_CMD(NRF_STATUS_REG), NRF_TX_DS_MASK);
        printf("Transmission Successful.\n");
    }
    else if (status & NRF_RX_DR_MASK)
    {
        //Clear the interrupt bit due to RX in NRF24L01+
        nrf_write_reg(SPI2, W_REGISTER_CMD(NRF_STATUS_REG), NRF_RX_DR_MASK);
        printf("Data Received Successfully.\n");

        uint8_t x = nrf_read_reg(SPI2, R_REGISTER_CMD(NRF_STATUS_REG)) & 0x0E;
        printf("Received in PIPE : %d\n", x);

        spi_transfer(SPI2, R_RX_PAYLOD_CMD);
        //    printf("packet send1 = %x\n", x);


        x = spi_dummy_transfer(SPI2);
        printf("Received Value: %x\n", x);
        x = spi_dummy_transfer(SPI2);
        printf("Received Value: %x\n", x);
        x = spi_dummy_transfer(SPI2);
        printf("Received Value: %x\n", x);
        x = spi_dummy_transfer(SPI2);
        printf("Received Value: %x\n", x);

    }
    else if (status & NRF_MAX_RT_MASK)
    {
        nrf_write_reg(SPI2, W_REGISTER_CMD(NRF_STATUS_REG), NRF_MAX_RT_MASK);
        printf("Maximum Retry Count Reached.\n");
    }

#ifdef NRF_SEND

//    printf("\nPacket sent.\n");
#else
    nrf_packet_rcv(SPI2);
#endif

    //Enable Interrupt.
//    GPIOIntEnable(NRF_IRQ_PORT, NRF_IRQ_PIN);
    IntMasterEnable();
}


