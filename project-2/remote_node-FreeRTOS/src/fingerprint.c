/**
 * @file fingerprint.c
 * @brief This file consists of all the functions required for the Fingerprint Scanner GT-521F32.
 *
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/19/2019
 *
 * @copyright Copyright (c) 2019
 *
 */


#include "inc/fingerprint.h"
#include "inc/packet.h"
#include "inc/lcd.h"
#include "inc/timer.h"




void fp_interrupt_config(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOL))
    {
    }

    GPIOPinTypeGPIOInput(FP_IRQ_PORT, FP_IRQ_PIN);
    GPIOPadConfigSet(FP_IRQ_PORT, FP_IRQ_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPD);

    GPIOIntRegister(FP_IRQ_PORT, fp_irqhandler);

    GPIOIntTypeSet(FP_IRQ_PORT, FP_IRQ_PIN, GPIO_RISING_EDGE);

    fp_interrupt_enable();

}


void fp_cmdsend(uart_t uart, uint8_t cmd, uint8_t param)
{
    uint8_t cmd_packet[COMMAND_SIZE] =
    {
     START_CODE1, START_CODE2,
     DEVICEID_LSBYTE, DEVICEID_MSBYTE,
     param, PARAMETER_BYTES, PARAMETER_BYTES, PARAMETER_BYTES,
     cmd, CMD_CODE_MSBYTE,
     CHECKSUM_BYTES, CHECKSUM_BYTES
    };

    uint8_t i;
    uint16_t fp_checksum = fp_checksum_calc(cmd_packet);

    cmd_packet[10] = fp_checksum & 0x00FF;
    cmd_packet[11] = (fp_checksum & 0xFF00)>>8 ;

    for(i = 0; i < COMMAND_SIZE; i++)
    {
        UARTCharPut(uart, cmd_packet[i]);
        uart_busy(uart);
    }
}


uint16_t fp_responsercv(uart_t uart)
{
    uint8_t response_packet[RESPONSE_SIZE];
    uint8_t i;
//    uint16_t fp_checksum;
    uint16_t temp;

    for(i=0; i < RESPONSE_SIZE; i++)
    {
        response_packet[i] = (uint8_t)(UARTCharGet(uart) & 0x000000FF);
    }

//    fp_checksum = fp_checksum_calc(response_packet);

//    if (response_packet[11] == fp_checksum)
//    {
//        printf("Checksum Calculated , Correct Data Received.\n");
//    }
//    else
//    {
//        printf("Checksum Failed.\n");
//    }

    if(response_packet[8] == ACK)
    {
        temp = response_packet[5] << 8;
        temp = temp | response_packet[4];

        return (temp);
    }
    else if(response_packet[8] == NACK)
    {
        temp = response_packet[5] << 8;
        temp = temp | response_packet[4];

        return (temp);
    }
    return -1;
}

uint16_t fp_checksum_calc(uint8_t packet[])
{
    uint8_t i;
    uint16_t fp_checksum = 0;
    for(i = 0; i < 10; i++)
    {
        fp_checksum += packet[i];
    }

    return fp_checksum;
}


uint16_t fp_start_enroll(uart_t uart, uint16_t new_id)
{
    uint8_t id_lsb, id_msb;
    uint8_t i;
    uint16_t fp_checksum;

    id_lsb = (uint8_t)(new_id & 0x00FF);
    id_msb = (uint8_t)(new_id & 0xFF00);


    uint8_t cmd_packet[COMMAND_SIZE] =
    {
     START_CODE1, START_CODE2,
     DEVICEID_LSBYTE, DEVICEID_MSBYTE,
     id_lsb, id_msb, PARAMETER_BYTES, PARAMETER_BYTES,
     FP_ENROLLSTART_CMD, CMD_CODE_MSBYTE,
     CHECKSUM_BYTES, CHECKSUM_BYTES
    };


    fp_checksum = fp_checksum_calc(cmd_packet);

    cmd_packet[10] = fp_checksum & 0x00FF;
    cmd_packet[11] = (fp_checksum & 0xFF00)>>8 ;

    for(i = 0; i < COMMAND_SIZE; i++)
    {
        UARTCharPut(uart, cmd_packet[i]);
        uart_busy(uart);
    }

    return (fp_responsercv(uart));
}



void fp_enroll(uart_t uart, uint8_t enroll_cmd)
{
    printf("Press finger on Scanner.\n");
    packet_msglog_uart(UART_BBG, "Press Finger on Scanner");

    //Stays here till finger is not pressed.
    while(fp_isfingerpress(uart));

    //Capture finger.
    fp_capture(uart, FP_BEST_IMAGE);

    //Enrollment Command.
    fp_cmdsend(uart, enroll_cmd, FP_NOPARAM);
    fp_responsercv(uart);

    printf("Lift finger from Scanner.\n");
    packet_msglog_uart(UART_BBG, "Lift finger from Scanner.");
    LCD_write("Lift finger from Scanner.");

    //Stay here till finger is lifted
    while(!fp_isfingerpress(uart));
}



void add_fingerprint(uart_t uart)
{
    //TODO: Take care of duplicated ID.

    uint16_t enroll_count;
    uint32_t ui32Int;
    uint32_t int_status;

    //Disable GPIO interrupt for Finger print Sensor.
    ui32Int = IntMasterDisable();


    //LCD Display
    //Get ready to Register Fingerprint

    //Get enrollment count
    enroll_count = fp_get_enrollcount(uart);

    printf("Starting Fingerprint Enrollment.\n");
    packet_msglog_uart(UART_BBG, "Starting Fingerprint Enrollment");

    fp_start_enroll(uart, enroll_count);

    //Enroll Finger print 3 times.
    fp_enroll(uart, FP_ENROLL1_CMD);
    fp_enroll(uart, FP_ENROLL2_CMD);
    fp_enroll(uart, FP_ENROLL3_CMD);

    printf("Fingerprint Added.\n");
    LCD_write("Fingerprint Added");
    packet_msglog_uart(UART_BBG, "Fingerprint Added");


    //Clear Interrupts if in case interrupts were triggered.
    //Get Pin Interrupt Status.
    int_status = GPIOIntStatus(FP_IRQ_PORT, false);
    //Clear the particular Interrupt
    GPIOIntClear(FP_IRQ_PORT, int_status & FP_IRQ_PIN);



    // If interrupts were enabled when we turned them off, turn them
    // back on again.
    if(!ui32Int)
    {
        IntMasterEnable();
    }

}


uint16_t fp_baudrate(uart_t uart, uint16_t baudrate)
{
    uint8_t br_lsb, br_msb;
    uint8_t i;
    uint16_t fp_checksum;

    br_lsb = (uint8_t)(baudrate & 0x00FF);
    br_msb = (uint8_t)(baudrate & 0xFF00);

    uint8_t cmd_packet[COMMAND_SIZE] =
    {
     START_CODE1, START_CODE2,
     DEVICEID_LSBYTE, DEVICEID_MSBYTE,
     br_lsb, br_msb, PARAMETER_BYTES, PARAMETER_BYTES,
     FP_BAUDRATE_CMD, CMD_CODE_MSBYTE,
     CHECKSUM_BYTES, CHECKSUM_BYTES
    };

    fp_checksum = fp_checksum_calc(cmd_packet);

    cmd_packet[10] = fp_checksum & 0x00FF;
    cmd_packet[11] = (fp_checksum & 0xFF00)>>8 ;

    for(i = 0; i < COMMAND_SIZE; i++)
    {
        UARTCharPut(uart, cmd_packet[i]);
        uart_busy(uart);
    }

    return (fp_responsercv(uart));
}


uint16_t fp_deleteid(uart_t uart, uint16_t id)
{
    uint8_t id_lsb, id_msb;
    uint8_t i;
    uint16_t fp_checksum;

    id_lsb = (uint8_t)(id & 0x00FF);
    id_msb = (uint8_t)(id & 0xFF00);

    uint8_t cmd_packet[COMMAND_SIZE] =
    {
     START_CODE1, START_CODE2,
     DEVICEID_LSBYTE, DEVICEID_MSBYTE,
     id_lsb, id_msb, PARAMETER_BYTES, PARAMETER_BYTES,
     FP_DELETEID_CMD, CMD_CODE_MSBYTE,
     CHECKSUM_BYTES, CHECKSUM_BYTES
    };

    fp_checksum = fp_checksum_calc(cmd_packet);

    cmd_packet[10] = fp_checksum & 0x00FF;
    cmd_packet[11] = (fp_checksum & 0xFF00)>>8 ;

    for(i = 0; i < COMMAND_SIZE; i++)
    {
        UARTCharPut(uart, cmd_packet[i]);
        uart_busy(uart);
    }

    return (fp_responsercv(uart));
}


uint16_t fp_verify(uart_t uart, uint16_t id)
{
    uint8_t id_lsb, id_msb;
    uint8_t i;
    uint16_t fp_checksum;

    id_lsb = (uint8_t)(id & 0x00FF);
    id_msb = (uint8_t)(id & 0xFF00);

    uint8_t cmd_packet[COMMAND_SIZE] =
    {
     START_CODE1, START_CODE2,
     DEVICEID_LSBYTE, DEVICEID_MSBYTE,
     id_lsb, id_msb, PARAMETER_BYTES, PARAMETER_BYTES,
     FP_VERIFY_CMD, CMD_CODE_MSBYTE,
     CHECKSUM_BYTES, CHECKSUM_BYTES
    };

    fp_checksum = fp_checksum_calc(cmd_packet);

    cmd_packet[10] = fp_checksum & 0x00FF;
    cmd_packet[11] = (fp_checksum & 0xFF00)>>8 ;

    for(i = 0; i < COMMAND_SIZE; i++)
    {
        UARTCharPut(uart, cmd_packet[i]);
        uart_busy(uart);
    }

    return (fp_responsercv(uart));
}


uint16_t fp_identify(uart_t uart)
{
    fp_cmdsend(uart, FP_IDENTIFY_CMD, FP_NOPARAM);
    return (fp_responsercv(uart));
}


void fp_init(void)
{
    fp_open(UART_FP);
    fp_led_status(UART_FP, FP_LEDON);
}



void fp_access_handle(void)
{
    uint16_t identify_status;
    uint8_t payload_arr[3];

    fp_capture(UART_FP, FP_LOW_IMAGE);

    identify_status = fp_identify(UART_FP);

    if(identify_status < 3000)
    {
        printf("Verifying Fingerprint.\n");
        LCD_write("Verifying Fingerprint");
        packet_msglog_uart(UART_BBG, "Verifying Fingerprint");

        payload_arr[0] = FINGERPRINT_MATCHED;
        payload_arr[1] = (identify_status & 0x00FF);
        payload_arr[2] = ((identify_status & 0xFF00)>>8);

        //Send fingerprint status TO BBG over here.
        packet_send_uart(UART_BBG, packet_make(FINGERPRINT_ID, payload_arr, 3, TRUE));

    }
    else if(identify_status == FP_NACK_DB_IS_EMPTY)
    {
        printf("WARNING: DATABASE EMPTY.\n");
        printf("ADD FINGERPRINT.\n");
        LCD_write("WARNING: DATABASE EMPTY");
        packet_msglog_uart(UART_BBG, "WARNING: DATABASE EMPTY");
        packet_msglog_uart(UART_BBG, "ADD FINGERPRINT TO DATABASE");
        //TODO: Send (Add Fingerprint) packet over here.

    }
    else if(identify_status == FP_NACK_IDENTIFY_FAILED)
    {
        printf("Verifying Fingerprint.\n");
        printf("In not matched handler\n");
        LCD_write("Verifying Fingerprint");
        packet_msglog_uart(UART_BBG, "Verifying Fingerprint");

        payload_arr[0] = FINGERPRINT_NOTMATCHED;
        payload_arr[1] = DONT_CARE;
        payload_arr[2] = DONT_CARE;

        //Send fingerprint status TO BBG over here.
        packet_send_uart(UART_BBG, packet_make(FINGERPRINT_ID, payload_arr, 3, TRUE));
    }
}



void fp_irqhandler(void)
{
    uint32_t int_status;

    IntMasterDisable();

    // Get the interrrupt status.
    int_status = GPIOIntStatus(FP_IRQ_PORT, false);

    if (int_status & FP_IRQ_PIN)
    {
        GPIOIntClear(FP_IRQ_PORT, int_status & FP_IRQ_PIN);
        printf("Fingerprint Sensor Touched.\n");
        fp_access_handle();
    }

    IntMasterEnable();
}



void fingerprint_test(void)
{
    //Fingerprint Testing

//    uart_configure(UART_FP, CLOCK, BAUDRATE_FP, 0);
//
//    fp_interrupt_config();

    //Initializing the fingerprint sensor.
    fp_open(UART_FP);

    //Turning the LED on
    fp_led_status(UART_FP, FP_LEDON);

//    fp_deleteid(UART_FP, 0);
//    fp_deleteall(UART_FP);
//    add_fingerprint(UART_FP);

    //Terminating the fingerprint sensor
//    fp_close(UART_FP);

}

