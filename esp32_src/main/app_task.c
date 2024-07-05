//
// Created by xl on 2024/5/13.
//
#include <time.h>
#include "lcd_init.h"
#include "lcd.h"
#include "lcd_number.h"
#include "lcd_number2.h"
#include "sz_wifi.h"
#include "spi_ws2812.h"
#include "sht30.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "my_mqtt_client.h"

const unsigned char *Image1[10]= {gImage_0,gImage_1,gImage_2,gImage_3,gImage_4,gImage_5,
                                         gImage_6,gImage_7,gImage_8,gImage_9};

const unsigned char *Image2[10]= {gImage2_0,gImage2_1,gImage2_2,gImage2_3,gImage2_4,gImage2_5,
                                        gImage2_6,gImage2_7,gImage2_8,gImage2_9};
const unsigned char **Number_Image = Image1;

extern esp_mqtt_client_handle_t client_esp32;
extern uint32_t rgb24bit;
int ima_flag = 0;

void switch_Image()
{
    ima_flag = 1;
    if(Number_Image == Image1){
        Number_Image = Image2;
        esp_mqtt_client_publish(client_esp32, "/time/style","image2", 0, 1, 0);
    }
    else{
        Number_Image = Image1;
        esp_mqtt_client_publish(client_esp32, "/time/style","image1", 0, 1, 0);
    }
}

void task_gettime(void *pvParameters)
{
    sz_get_sntptime();
    vTaskSuspend( NULL );

}
void task_display(void *pvParameters)
{
    time_t now;
    struct tm timeinfo;
    char payload[50];

    //时间是否设置成功
    while(timeinfo.tm_year < (2024 - 1900))
    {
        // 获取当前时间并解析成结构体
        time(&now);
        localtime_r(&now, &timeinfo);
    }

    // 显示完整的时分秒时间
    int sec_ones = timeinfo.tm_sec % 10;
    int sec_tens = timeinfo.tm_sec / 10;
    int min_ones = timeinfo.tm_min % 10;
    int min_tens = timeinfo.tm_min / 10;
    int hour_ones = timeinfo.tm_hour % 10;
    int hour_tens = timeinfo.tm_hour / 10;

    //printf("hour:%d\n",timeinfo.tm_hour);
    LCD_CS_ShowPicture(0, 0, 80, 160, Number_Image[hour_tens], 1);
    LCD_CS_ShowPicture(0, 0, 80, 160, Number_Image[hour_ones], 2);
    LCD_CS_ShowPicture(0, 0, 80, 160, Number_Image[min_tens], 3);
    LCD_CS_ShowPicture(0, 0, 80, 160, Number_Image[min_ones], 4);
    LCD_CS_ShowPicture(0, 0, 80, 160, Number_Image[sec_tens], 5);
    LCD_CS_ShowPicture(0, 0, 80, 160, Number_Image[sec_ones], 6);

    int last_sec = timeinfo.tm_sec; // 用于存储上一次的秒数
    int last_min = timeinfo.tm_min;
    int last_hour = timeinfo.tm_hour;

    while(true)
    {
        if(ima_flag == 1){
            ima_flag = 0;
            LCD_CS_ShowPicture(0, 0, 80, 160, Number_Image[hour_tens], 1);
            LCD_CS_ShowPicture(0, 0, 80, 160, Number_Image[hour_ones], 2);
            LCD_CS_ShowPicture(0, 0, 80, 160, Number_Image[min_tens], 3);
            LCD_CS_ShowPicture(0, 0, 80, 160, Number_Image[min_ones], 4);
            LCD_CS_ShowPicture(0, 0, 80, 160, Number_Image[sec_tens], 5);
            LCD_CS_ShowPicture(0, 0, 80, 160, Number_Image[sec_ones], 6);
        }
        // 获取当前时间并解析成结构体
        time(&now);
        localtime_r(&now, &timeinfo);

        sec_ones = timeinfo.tm_sec % 10;
        sec_tens = timeinfo.tm_sec / 10;
        min_ones = timeinfo.tm_min % 10;
        min_tens = timeinfo.tm_min / 10;
        hour_ones = timeinfo.tm_hour % 10;
        hour_tens = timeinfo.tm_hour / 10;

        snprintf(payload,sizeof(payload),"%1d,%1d,%1d,%1d,%1d,%1d",hour_tens,hour_ones,min_tens,min_ones,sec_tens,sec_ones);
        esp_mqtt_client_publish(client_esp32, "/time", payload, strlen(payload), 0, 0);

        if(timeinfo.tm_sec != last_sec) {
            last_sec = timeinfo.tm_sec;
            LCD_CS_ShowPicture(0, 0, 80, 160, Number_Image[sec_ones], 6);
            if (sec_ones == 0) {
                LCD_CS_ShowPicture(0, 0, 80, 160, Number_Image[sec_tens], 5);
            }
        }

        if(timeinfo.tm_min != last_min){
            last_min = timeinfo.tm_min;
            LCD_CS_ShowPicture(0, 0, 80, 160, Number_Image[min_ones], 4);
            if (min_ones == 0) {
                LCD_CS_ShowPicture(0, 0, 80, 160, Number_Image[min_tens], 3);
            }
        }

        if(timeinfo.tm_hour != last_hour){
            last_hour = timeinfo.tm_hour;
            LCD_CS_ShowPicture(0, 0, 80, 160, Number_Image[hour_ones], 2);
            LCD_CS_ShowPicture(0, 0, 80, 160, Number_Image[hour_tens], 1);
        }

        // 任务延迟调整为500ms，确保屏幕刷新更加平滑
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void task_tem_hum(void *pvParameters)
{
    double sht_buf[2];
    char tem_payload[10];
    char hum_payload[10];

    while(true)
    {
        SHT30_Read_Humiture(&sht_buf[0],&sht_buf[1],0);
        //printf("温度:%f   湿度:%f\n",sht_buf[0],sht_buf[1]);
        // 发布温度数据
        snprintf(tem_payload, sizeof(tem_payload), "%.0f", sht_buf[0]/10);
        esp_mqtt_client_publish(client_esp32, "/sensor/tem", tem_payload, strlen(tem_payload), 0, 0);

        // 发布湿度数据
        snprintf(hum_payload, sizeof(hum_payload), "%.0f", sht_buf[1]/10);
        esp_mqtt_client_publish(client_esp32, "/sensor/hum", hum_payload, strlen(hum_payload), 0, 0);
        vTaskDelay(1200 / portTICK_PERIOD_MS);
    }
}

void task_led(void *pvParameters)
{
    while(true)
    {
        uint32_t tmp[6] = {rgb24bit,rgb24bit,rgb24bit,rgb24bit,rgb24bit,rgb24bit};
        setLedColorByArr( 6, tmp);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        printf("rgb24bit:%lx\n", rgb24bit);
//        setLedColorByArr( 6, tmp[1]);
//        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}
