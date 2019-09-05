/*
 * temp.c
 *
 *  Created on: Apr 8, 2019
 *      Author: Siddhant Jajoo
 */


#include <string.h>
#include "inc/temp.h"
#include "inc/msg_queue.h"
#include "inc/sync_objects.h"


/**
 * @brief This function creates Temperature Task
 * @return uint8_t
 */
uint8_t temptask_init(void)
{

    if(xTaskCreate(temp, (const portCHAR *)"temperature", LEDTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY, NULL) != pdTRUE)
    {
        UARTprintf("Failed to create Temperature Task.\n");
        return 1;
    }

    return 0;
}

/**
 * @brief 8 bit write function
 * @return void
 */
void temp_write(void)
{

    I2CMasterSlaveAddrSet(I2C2_BASE, SLAVE_ADDR, false);
    I2CMasterDataPut(I2C2_BASE, CONFIG_TEMP);
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_SEND);

    while(!I2CMasterBusy(I2C2_BASE));
    while(I2CMasterBusy(I2C2_BASE));

}


/**
 * @brief This function reads 2 byte Temperature register using I2C.
 * @return void
 */
float temp_read(void)
{
    uint32_t temp;
    float final_temp;

    temp_write();

    I2CMasterSlaveAddrSet(I2C2_BASE, SLAVE_ADDR, true);
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);

    while(!I2CMasterBusy(I2C2_BASE));
    while(I2CMasterBusy(I2C2_BASE));

    temp = I2CMasterDataGet(I2C2_BASE);
    temp <<= 8;
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);

    while(!I2CMasterBusy(I2C2_BASE));
    while(I2CMasterBusy(I2C2_BASE));

    temp |= I2CMasterDataGet(I2C2_BASE);

    temp >>= 4;
    final_temp = temp * 0.0625;

    return final_temp;
}


/**
 * @brief The function receives data from the temperature sensor and sends it to
 * the logger task via message queue
 * @return void
 */
void temp_send(void)
{
    event info;
    info.id = TEMP_RCV_ID;
    info.data.temp_data.current_time = xTaskGetTickCount();
    info.data.temp_data.temp = temp_read();


    xSemaphoreTake(g_qsem,portMAX_DELAY);
    if(xQueueSend(nrf_log_mq, &info, portMAX_DELAY) != pdTRUE)
    {
        xSemaphoreTake(g_uartsem,portMAX_DELAY);
        UARTprintf("Temperature Data Send failed.\n");
        xSemaphoreGive(g_uartsem);
    }
    xSemaphoreGive(g_qsem);
}


/**
 * @brief Callback function for Temperature Task.
 * @return void
 */
static void temp(void *pvParameters)
{
    event info;
    while(1)
    {
        xSemaphoreTake(g_temp,portMAX_DELAY);


        //Assigning Random values in the structure for testing purposes.
        info.id = RANDOM_TEST_ID;
        info.data.temp_data.current_time = xTaskGetTickCount();
        info.data.temp_data.temp = ((float)rand()/(float)(RAND_MAX)) * 10.0;
        info.data.temp_data.num = rand() % 200;
        strcpy(info.data.temp_data.str, "SIDDHANT JAJOO");


        xSemaphoreTake(g_qsem,portMAX_DELAY);
        if(xQueueSend(nrf_log_mq, &info, portMAX_DELAY) != pdTRUE)
        {
            xSemaphoreTake(g_uartsem,portMAX_DELAY);
            UARTprintf("Temperature Data Send failed.\n");
            xSemaphoreGive(g_uartsem);
        }
        xSemaphoreGive(g_qsem);

        //Reading and sending temperature to the logger task.
        //temp_send();
    }
}
