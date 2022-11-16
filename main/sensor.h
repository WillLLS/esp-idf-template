#include "config.h"
#include "decode_frame.h"

#include "driver/uart.h"
#include "driver/gpio.h"

#define UART_PORT_NUM   UART_NUM_2
#define UART_TX         GPIO_NUM_4
#define UART_RX         GPIO_NUM_5      

#define BAUDRATE    115200

#define BUF_SIZE (256)

void sensor_task();