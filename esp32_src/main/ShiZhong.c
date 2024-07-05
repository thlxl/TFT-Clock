#include <stdio.h>
#include "lcd_init.h"
#include "lcd.h"
#include "spi_ws2812.h"
#include "sht30.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sz_wifi.h"
#include "time.h"
#include "app_task.h"
#include "my_mqtt_client.h"

//任务优先级
#define GETTIME_TASK_PRIO		3
//任务堆栈大小
#define GETTIME_STK_SIZE 		4096
//任务句柄
TaskHandle_t GetTimeTask_Handler;

//任务优先级
#define TEMHUM_TASK_PRIO		1
//任务堆栈大小
#define TEMHUM_STK_SIZE 		2048
//任务句柄
TaskHandle_t TemHumTask_Handler;

//任务优先级
#define LED_TASK_PRIO		3
//任务堆栈大小
#define LED_STK_SIZE 		2048
//任务句柄
TaskHandle_t LedTask_Handler;

//任务优先级
#define DISPLAY_TASK_PRIO		2
//任务堆栈大小
#define DISPLAY_STK_SIZE 		4096
//任务句柄
TaskHandle_t DisplayTask_Handler;


void app_main(void)
{

   LCD_Init();
   spi_ws2812_init();
   LCD_Full_Fill(0,0,LCD_W,LCD_H,WHITE);
   SHT30_Init();
   sz_wifi_init();
   sz_wifi_connect();
   mqtt5_app_start();

   xTaskCreate((TaskFunction_t ) task_display,            //任务函数
                   (const char*    )"display_task",          //任务名称
                   (uint16_t       )DISPLAY_STK_SIZE,        //任务堆栈大小
                   (void*          )NULL,                  //传递给任务函数的参数
                   (UBaseType_t    )DISPLAY_TASK_PRIO,       //任务优先级
                   (TaskHandle_t*  )&DisplayTask_Handler);//任务句柄

   xTaskCreate((TaskFunction_t ) task_gettime,            //任务函数
                (const char*    )"gettime_task",          //任务名称
                (uint16_t       )GETTIME_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )GETTIME_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&GetTimeTask_Handler);//任务句柄

   xTaskCreate((TaskFunction_t ) task_tem_hum,            //任务函数
                (const char*    )"tem_hum_task",          //任务名称
                (uint16_t       )TEMHUM_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )TEMHUM_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&TemHumTask_Handler);//任务句柄

   xTaskCreate((TaskFunction_t ) task_led,            //任务函数
                (const char*    )"led_task",          //任务名称
                (uint16_t       )LED_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )LED_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&LedTask_Handler);//任务句柄
}
