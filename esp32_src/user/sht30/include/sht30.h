#ifndef __SHT30_H
#define __SHT30_H 
#include "myiic.h"


#define SHT30_ADDR (u8)(0x44) //sht30 i2c地址，addr引脚接低电平为0x44,接高点平为0x45

//SHT30����
#define SHT30_READ_HUMITURE (u16)0x2c06  //����ʪ��

//CRC����ʽ
#define POLYNOMIAL 0x31 // X^8 + X^5 + X^4 + 1

u8 SHT30_Read_Humiture(double *temp,double *humi,uint8_t addr);

void SHT30_CMD(u16 cmd,uint8_t addr);

void SHT30_Init(void);

#endif


