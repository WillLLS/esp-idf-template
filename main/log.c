#include "log.h"
#include "string.h"

void timeSinceStart(uint64_t µTime);
void receivedValue();

uint8_t flag_init   = 0;
uint16_t days       = 0;
uint8_t hours       = 0;
uint8_t min         = 0;
uint8_t sec         = 0;

uint8_t heartValue      = 0;

uint8_t breath_info     = 0;
uint8_t breathValue     = 0;

uint8_t pres_info       = 0;
uint8_t movement_info   = 0;
uint8_t body_dynamic    = 0;
uint16_t body_distance  = 0;
uint16_t body_orient[2] = {0};


uint8_t bodyDynamic     = 0;
uint16_t bodyDistance   = 0;

char value1Byte[2]  = {0};
char value2Bytes[3] = {0};
char value4Bytes[5] = {0};

/**
 * heartValue   = HV
 * BreathInfo   = BI
 * BreathValue  = BV
 * PresInfo     = PI
 * movementInfo = MI
 * BodyDynamic  = DYN
 * BodyDistance = DIS
 * BodyOrient   = BO
*/

/**
 * @brief Monitoring Task
*/
void monitoring(){

    ESP_ERROR_CHECK(esp_timer_early_init());

    for(;;){
        
        uint64_t time = esp_timer_get_time();

        timeSinceStart(time);
        receivedValue();

        printf("{ HV: %2d}, ", heartValue);
        printf("{ BI: %2d, BV: %2d }, ", breath_info, breathValue);
        printf("{ PI: %2d, MI: %2d, DYN: %3d, DIS: %3d, BO: (%3d, %3d) }\n",
                pres_info, movement_info, body_dynamic, body_distance, body_orient[0], body_orient[1]);
        
        vTaskDelay(100);

    }
}


/**
 * @brief Transform the time in µseconde into a char* for the monitoring.
*/
void timeSinceStart(uint64_t µTime){

    sec = (µTime / 1000000) % 60;

    if(sec == 0 && flag_init == 1){
        //sec = 0;
        min = min + 1;
    }

    if(min == 60){
        min = 0;
        hours = hours + 1;
    }

    if(hours == 24){
        hours = 0;
        days = days + 1;
    }

    flag_init = 1;

    printf("{ %04d:%02d:%02d:%02d }, ", days, hours, min, sec);
}


/**
 * @brief Read the value from the differents Queues.
*/
void receivedValue(){

    if(xQueueReceive(QueueHeart, (void *) value1Byte, 10)){
        if(value1Byte[0] == 0x02){ // Heart Value
            heartValue = (uint8_t) value1Byte[1];
        }
    }

    if(xQueueReceive(QueueBreath, (void *) value1Byte, 10)){
        switch ((uint8_t) value1Byte[0])
        {
        case BREATH_INF: // Respiratory Information
            breath_info = (uint8_t) value1Byte[1];
            /*
            switch ((uint8_t) value1Byte[1])
            {
            case NORMAL:
                strcpy(breath_info, "NORMAL");
                break;
            case HYPERVENTILATION:
                strcpy(breath_info, "HYPERVENTIL");
                break;
            case HYPOPNEA:
                strcpy(breath_info, "HYPOPNEA");
                break;
            case DETECTION_PROGRESS:
                strcpy(breath_info, "DETECTION..");
                break;
            
            default:
                break;
            }
            */
            break;
        
        case BREATH_VAL: // Respiratory Value
            breathValue = (uint8_t) value1Byte[1];
            break;

        default:
            break;
        }
    }

    if(xQueueReceive(QueueHumanPSE, (void *) value4Bytes, 10)){
        switch (value4Bytes[0])
        {
        case PRESENCE_INF:
            pres_info = value4Bytes[1];
            break;
        case MOVE_INF:
            movement_info = value4Bytes[1];
            break;
        case BODY_DYN:
            body_dynamic = value4Bytes[1];
            break;
        case BODY_DIS:
            body_distance = value4Bytes[1] << 8 | value4Bytes[2];
            break;
        case BODY_ORI:
            body_orient[0] = value4Bytes[1] << 8 | value4Bytes[2];
            body_orient[1] = value4Bytes[3] << 8 | value4Bytes[4];
            break;
        
        default:
            break;
        }
    }

}
