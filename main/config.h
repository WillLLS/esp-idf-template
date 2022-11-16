#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

extern QueueHandle_t QueueLog;
extern QueueHandle_t QueueHeart;
extern QueueHandle_t QueueBreath;
extern QueueHandle_t QueueHumanPSE;


//QueueHandle_t xQueueMqtt;

#define HEAD_0  0x53
#define HEAD_1  0x59
#define TAIL_0  0x54
#define TAIL_1  0x43
 
/**
 * Human presence 
*/
#define HUMAN_PSE   0x80        //Human presence data

#define PRESENCE_INF    0x01        //Presence Information
#define SOMEONE_HERE        0x02    //Someone here
#define NOONE_HERE          0x00    //Noone here
 
#define MOVE_INF        0x02        //Campaign Information
#define NONE                0x00    //None
#define CA_CLOSE            0x01    //Someone approaches
#define CA_AWAY             0x02    //Some people stay away
#define DISORDER            0x03    //Disorderly movement

#define BODY_DYN        0x03        //Body movement information

#define BODY_DIS        0x04        //Body distance

#define BODY_ORI        0x05        //Body orientation

/**
 * Breath
*/
#define BREATH      0x81

#define BREATH_INF      0X01        // Breath information
#define NORMAL              0x01    // Normal respiration
#define HYPERVENTILATION    0x02    // Hypperventilation
#define HYPOPNEA            0x03    // Hypopnea ?
#define DETECTION_PROGRESS  0X04    // Detection in progress

#define BREATH_VAL      0x02        // Respiratory value

/**
 * Heart
*/
#define HEART       0x85

#define HEART_VAL       0x02

 
 
///////////////////////////////////////////////////////////////////////////////////

#define HEART_RATE_RADAR 0x81   //Respiratory heart rate data
 
#define RATE_DATA 0x01          //Heartbeat Pack
#define RATE_NORMAL 0x01        //Normal heart rate
#define RATE_RAPID 0x02         //Rapid heart rate
#define RATE_SLOW 0x03          //Slow heart rate
 
#define HEART_RATE 0x02         //Breathing rate
 
#define HEART_RATE_WAVE 0x03    //Heart rate waveform (No analysis for now)
 
#define BREATH_DATA 0x04        //Breathing data
#define BREATH_NORMAL 0x01      //Normal breathing
#define BREATH_RAPID 0x02       //Acute respiratory abnormalities
#define BREATH_SLOW 0x03        //Slow heartbeat
 
 
#define BREATH_WAVE 0x06        //Respiratory waveform (No analysis for now)
 
#define LOCA_DET_ANOMAL 0x07    //Location detection anomaly
#define OUT_OF_RANGE 0x00       //Detection of targets out of range
#define WITHIN_RANGE 0x01       //Within the scope
 
#define DISTANCE 0x08           //Distance at rest
#define ANGLE 0x09              //Angle at rest