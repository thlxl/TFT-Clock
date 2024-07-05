//
// Created by xl on 2024/5/5.
//
#include "lcd_init.h"
#include "lcd.h"

void (*LCD_CS_Clr)();
void (*LCD_CS_Set)();

void LCD_CS_Clr_1()
{
    gpio_set_level(GPIO_NUM_26, 0);
}
void LCD_CS_Set_1()
{
    gpio_set_level(GPIO_NUM_26, 1);
}

void LCD_CS_Clr_2()
{
    gpio_set_level(GPIO_NUM_27, 0);
}
void LCD_CS_Set_2()
{
    gpio_set_level(GPIO_NUM_27, 1);
}

void LCD_CS_Clr_3()
{
    gpio_set_level(GPIO_NUM_14, 0);
}
void LCD_CS_Set_3()
{
    gpio_set_level(GPIO_NUM_14, 1);
}

void LCD_CS_Clr_4()
{
    gpio_set_level(GPIO_NUM_12, 0);
}
void LCD_CS_Set_4()
{
    gpio_set_level(GPIO_NUM_12, 1);
}

void LCD_CS_Clr_5()
{
    gpio_set_level(GPIO_NUM_13, 0);
}
void LCD_CS_Set_5()
{
    gpio_set_level(GPIO_NUM_13, 1);
}

void LCD_CS_Clr_6()
{
    gpio_set_level(GPIO_NUM_23, 0);
}
void LCD_CS_Set_6()
{
    gpio_set_level(GPIO_NUM_23, 1);
}

void LCD_Sel_CS(int val)
{
   if(val == 1){
       LCD_CS_Clr = LCD_CS_Clr_1;
       LCD_CS_Set = LCD_CS_Set_1;
   }
   else if(val == 2){
       LCD_CS_Clr = LCD_CS_Clr_2;
       LCD_CS_Set = LCD_CS_Set_2;
   }
   else if(val == 3){
       LCD_CS_Clr = LCD_CS_Clr_3;
       LCD_CS_Set = LCD_CS_Set_3;
   }
   else if(val == 4){
       LCD_CS_Clr = LCD_CS_Clr_4;
       LCD_CS_Set = LCD_CS_Set_4;
   }
   else if(val == 5){
       LCD_CS_Clr = LCD_CS_Clr_5;
       LCD_CS_Set = LCD_CS_Set_5;
   }
   else if(val == 6){
       LCD_CS_Clr = LCD_CS_Clr_6;
       LCD_CS_Set = LCD_CS_Set_6;
   }
}

void LCD_CS_ShowPicture(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const uint8_t pic[],int cs)
{
    LCD_Sel_CS(cs);
    LCD_ShowPicture(x,y,length,width,pic);
}
void LCD_Full_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)
{
    LCD_Sel_CS(1);
    LCD_Fill(xsta,ysta,xend,yend,color);
    LCD_Sel_CS(2);
    LCD_Fill(xsta,ysta,xend,yend,color);
    LCD_Sel_CS(3);
    LCD_Fill(xsta,ysta,xend,yend,color);
    LCD_Sel_CS(4);
    LCD_Fill(xsta,ysta,xend,yend,color);
    LCD_Sel_CS(5);
    LCD_Fill(xsta,ysta,xend,yend,color);
    LCD_Sel_CS(6);
    LCD_Fill(xsta,ysta,xend,yend,color);
}


