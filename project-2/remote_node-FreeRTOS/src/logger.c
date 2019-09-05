/**
 * @file logger.c
 * @brief This file consists of all the functions required for logging data to the host console.
 *
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/13/2019
 *
 * @copyright Copyright (c) 2019
 *
 */


#include "inc/logger.h"
#include "inc/sync_objects.h"
#include "inc/msg_queue.h"



uint8_t nrf_loggertask_init(void)
{
    if(xTaskCreate(nrf_logger, (const portCHAR *)"nrf_logger", LEDTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY, NULL) != pdTRUE)
    {
        UARTprintf("Failed to create Logger Task.\n");
        return(1);
    }

    return 0;
}


void nrf_log(void)
{
    uint32_t temp_var;
    event info;

    if(xQueueReceive(nrf_log_mq, (void *)&info, portMAX_DELAY) == pdFALSE)
    {
        xSemaphoreTake(g_uartsem,portMAX_DELAY);
        UARTprintf("Receive from Queue failed.\n");
        xSemaphoreGive(g_uartsem);
    }

    switch(info.id)
    {

    case RANDOM_TEST_ID:
    {
//        xSemaphoreTake(g_uartsem,portMAX_DELAY);
//        UARTprintf("/**RANDOM TEST DATA**/\n");
//        UARTprintf("Timestamp: %d millisocnds\n", info.data.temp_data.current_time);
//        UARTprintf("Name: %s.\n", info.data.temp_data.str);
//        temp_var = (uint32_t)(info.data.temp_data.temp * 10000) - (uint8_t)info.data.temp_data.temp * 10000;
//        UARTprintf("Temperature Recorded: %d.%d Celsius\n", (uint8_t)info.data.temp_data.temp, temp_var);
//        UARTprintf("Random Number: %d.\n", info.data.temp_data.num);
//        xSemaphoreGive(g_uartsem);
        break;
    }

    case TEMP_RCV_ID:
    {
        xSemaphoreTake(g_uartsem,portMAX_DELAY);
        UARTprintf("/**TEMPERATURE DATA**/\n");
        UARTprintf("Timestamp: %d milliseconds\n", info.data.temp_data.current_time);
        temp_var = (uint32_t)(info.data.temp_data.temp * 10000) - (uint8_t)info.data.temp_data.temp * 10000;
        UARTprintf("Temperature Recorded: %d.%d Celsius\n", (uint8_t)info.data.temp_data.temp, temp_var);
        UARTprintf("\n*****************************************************\n\n");
        xSemaphoreGive(g_uartsem);
        break;
    }

    case LED_RCV_ID:
    {
//        xSemaphoreTake(g_uartsem,portMAX_DELAY);
//        UARTprintf("/**LED DATA**/\n");
//        UARTprintf("Timestamp: %d milliseconds\n", info.data.led_data.current_time);
//        UARTprintf("Name: %s\n", info.data.led_data.name);
//        UARTprintf("LED 1 Toggle Count: %d\n", info.data.led_data.toggle_cnt);
//        UARTprintf("LED 2 Toggle Count: %d\n", info.data.led_data.toggle_cnt - 1);
//        UARTprintf("\n*****************************************************\n\n");
//        xSemaphoreGive(g_uartsem);
        break;
    }
    }
}



static void nrf_logger(void *pvParameters)
{
    while(1)
    {
        //Logs all the messages it receives
        nrf_log();

    }


}

