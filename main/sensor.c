#include "sensor.h"

void sendData(char * data);

uart_config_t uart_config = {
        .baud_rate  = BAUDRATE,
        .data_bits  = UART_DATA_8_BITS,
        .parity     = UART_PARITY_DISABLE,
        .stop_bits  = UART_STOP_BITS_1,
        .flow_ctrl  = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
};

/**
 * @brief UART Initialization
*/
void init_UART(){
    ESP_ERROR_CHECK(uart_param_config(UART_PORT_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_PORT_NUM, UART_TX, UART_RX, GPIO_NUM_NC, GPIO_NUM_NC));
    ESP_ERROR_CHECK(uart_driver_install(UART_PORT_NUM, 512, 512, 2048, NULL, 0));
}

void sensor_task(){

    init_UART();

    int length  = 0;
    char res_decode[2];

    char dataReceived[64];

    for(;;){

        /**  
         * @brief Read the RX buffer and return the size of this one
         * @note With a fast vTaskDelay (1µs), the length is always 1 
         */
        uart_get_buffered_data_len(UART_PORT_NUM, (size_t *) &length);
        
        
        if(length){

            /**
             *  Read the RX buffer and add the value in the preinitiate buffer (dataReceived)
            */
            uart_read_bytes(UART_PORT_NUM, dataReceived, length,50);
            
            /*
            for(uint8_t i = 0; i<length; i++){
                printf("%02X ", (uint8_t) dataReceived[i]);
            }
            */

            decode_frame(dataReceived, length, res_decode);

            sendData(dataReceived);
        }
        
        vTaskDelay(1);
        //printf("\e[1;1H\e[2J");
    }
}


/**
 * @brief add the new data in different FreeRTOS Queues
*/
void sendData(char * dataReceived){

    if(dataReceived[2] == HUMAN_PSE){
        char data[5] = {0};
        switch (dataReceived[3])
        {
        case PRESENCE_INF:
            data[0] = dataReceived[3];
            data[1] = dataReceived[6];
            xQueueSend(QueueHumanPSE,(void *) data, portMAX_DELAY);
            break;
        case MOVE_INF:
            data[0] = dataReceived[3];
            data[1] = dataReceived[6];
            xQueueSend(QueueHumanPSE,(void *) data, portMAX_DELAY);
            break;
        case BODY_DYN:
            data[0] = dataReceived[3];
            data[1] = dataReceived[6];
            xQueueSend(QueueHumanPSE,(void *) data, portMAX_DELAY);
            break;
        case BODY_DIS:
            data[0] = dataReceived[3];
            data[1] = dataReceived[6];
            data[2] = dataReceived[7];
            xQueueSend(QueueHumanPSE,(void *) data, portMAX_DELAY);
            break;
        case BODY_ORI:
            data[0] = dataReceived[3];
            data[1] = dataReceived[6];
            data[2] = dataReceived[7];
            data[3] = dataReceived[8];
            data[4] = dataReceived[9];
            xQueueSend(QueueHumanPSE,(void *) data, portMAX_DELAY);
            break;
        
        default:
            break;
        }
    }

    else if (dataReceived[2] == HEART){
        char data[2];
        data[0] = dataReceived[3];
        data[1] = dataReceived[6];
        xQueueSend(QueueHeart,(void *) data, portMAX_DELAY);
    }

    else if (dataReceived[2] == BREATH)
    {
        char data[2];
        data[0] = dataReceived[3];
        data[1] = dataReceived[6];
        xQueueSend(QueueBreath,(void *) data, portMAX_DELAY);

    }
    
    
}