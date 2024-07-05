#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include <stdint.h>
#include "driver/gpio.h"


#define USE_HORIZONTAL 1  //设置横屏或竖屏显示  0或1为竖屏 2或3为横屏


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 80
#define LCD_H 160

#else
#define LCD_W 160
#define LCD_H 80
#endif



//-----------------LCD端口定义----------------

#define LCD_SCLK_Clr() gpio_set_level(GPIO_NUM_33, 0)//SCL=SCLK
#define LCD_SCLK_Set() gpio_set_level(GPIO_NUM_33, 1)

#define LCD_MOSI_Clr() gpio_set_level(GPIO_NUM_25, 0) //SDA=MOSI
#define LCD_MOSI_Set() gpio_set_level(GPIO_NUM_25, 1)

#define LCD_RES_Clr()  gpio_set_level(GPIO_NUM_21, 0)//RES
#define LCD_RES_Set()  gpio_set_level(GPIO_NUM_21, 1)

#define LCD_DC_Clr()   gpio_set_level(GPIO_NUM_19, 0)//DC
#define LCD_DC_Set()   gpio_set_level(GPIO_NUM_19, 1)
 		     
//#define LCD_CS_Clr()   gpio_set_level(GPIO_NUM_26, 0) //CS
//#define LCD_CS_Set()   gpio_set_level(GPIO_NUM_26, 1)

#define LCD_BLK_Clr()  gpio_set_level(GPIO_NUM_22, 0) //BLK
#define LCD_BLK_Set()  gpio_set_level(GPIO_NUM_22, 1)



void LCD_GPIO_Init(void);//初始化GPIO
void LCD_Writ_Bus(uint8_t dat);//模拟SPI时序
void LCD_WR_DATA8(uint8_t dat);//写入一个字节
void LCD_WR_DATA(uint16_t dat);//写入两个字节
void LCD_WR_REG(uint8_t dat);//写入一个指令
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);//设置坐标函数
void LCD_Init(void);//LCD初始化

void LCD_Sel_CS(int val);//使用LCD显示函数时必须先调用一下这个函数
//void (*LCD_CS_Clr)();
//void (*LCD_CS_Set)();
void LCD_CS_ShowPicture(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const uint8_t pic[],int cs);
void LCD_Full_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);
#endif




