#ifndef __MYIIC_H
#define __MYIIC_H
#include <stdint.h>
#include "driver/gpio.h"

typedef uint8_t  u8;
typedef uint16_t  u16;

//IO操作函数
#define IIC_SCL_Clr()    gpio_set_level(GPIO_NUM_5, 0) //SCL
#define IIC_SCL_Set()    gpio_set_level(GPIO_NUM_5, 1)

#define IIC_SDA_Clr()    gpio_set_level(GPIO_NUM_18, 0) //SDA
#define IIC_SDA_Set()    gpio_set_level(GPIO_NUM_18, 1)

#define SDA_IN()         gpio_set_direction(GPIO_NUM_18, GPIO_MODE_INPUT)
#define SDA_OUT()        gpio_set_direction(GPIO_NUM_18, GPIO_MODE_OUTPUT)

#define READ_SDA   gpio_get_level(GPIO_NUM_18)  //输入SDA

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待Ack信号
void IIC_Ack(void);					//IIC发送Ack信号
void IIC_NAck(void);				//IIC不发送Ack信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);
#endif
















