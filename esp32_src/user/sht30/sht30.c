#include "myiic.h"
#include "sht30.h"
//#include "delay.h"
#include "stdio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define POLYNOMIAL_CXDZ 0x31 // X^8 + X^5 + X^4 + 1
//SHT3X CRCУ校验
unsigned char SHT3X_CRC(u8 *data, u8 len)
{
	unsigned char bit;        // bit mask
	unsigned char crc = 0xFF; // calculated checksum
	unsigned char byteCtr;    // byte counter

	// calculates 8-Bit checksum with given polynomial @GZCXDZ
	for(byteCtr = 0; byteCtr < len; byteCtr++) {
			crc ^= (data[byteCtr]);
			for(bit = 8; bit > 0; --bit) {
					if(crc & 0x80) {
							crc = (crc << 1) ^ POLYNOMIAL_CXDZ;
					}  else {
							crc = (crc << 1);
					}
			}
	}
  return crc;
}

//SHT30命令函数
//addr:表示产品的序号
void SHT30_CMD(u16 cmd,uint8_t addr)
{
	IIC_Start();
  IIC_Send_Byte(((SHT30_ADDR+addr)<<1)|0);  //发送设备地址，写寄存器
	IIC_Wait_Ack();
	IIC_Send_Byte((cmd>>8)&0xff); //MSB
	IIC_Wait_Ack();
	IIC_Send_Byte(cmd&0xff); //LSB
	IIC_Wait_Ack();
	IIC_Stop();
    vTaskDelay(50/portTICK_PERIOD_MS);//命令发送完后需要等待20ms以上才能读写
}



//SHT30读取温度
//temp:温度，-400～1250,实际温度=temp/10,分辨率0.1摄氏度，精度正负0.3摄氏度
//humi:湿度，0～1000,实际湿度=humi/10,分辨率0.1%rh，精度正负3
//返回0成功，1失败
u8 SHT30_Read_Humiture(double *temp,double *humi,uint8_t addr)
{
	u8 buff[6];

	SHT30_CMD(SHT30_READ_HUMITURE,addr);//读温湿度命令
	
	IIC_Start();
	IIC_Send_Byte(((SHT30_ADDR+addr)<<1)|1); //发送设备地址，写寄存器
	IIC_Wait_Ack();
	buff[0]=IIC_Read_Byte(1);//继续读，给应答
	buff[1]=IIC_Read_Byte(1);//继续读，给应答
	buff[2]=IIC_Read_Byte(1);//继续读，给应答
	buff[3]=IIC_Read_Byte(1);//继续读，给应答
	buff[4]=IIC_Read_Byte(1);//继续读，给应答*
	buff[5]=IIC_Read_Byte(0);//不继续读，停止应答
	IIC_Stop();

	
	//printf("buff=%d,%d,%d,%d,%d,%d\r\n",buff[0],buff[1],buff[2],buff[3],buff[4],buff[5]);
	//CRC校验
	if(SHT3X_CRC(&buff[0],2)==buff[2] && SHT3X_CRC(&buff[3],2)==buff[5])
	{

		*temp = (-45+(175.0*((buff[0]<<8)+buff[1])/65535.0))*10;
		*humi = 10*100*((buff[3]<<8)+buff[4])/65535.0;
		if(*temp>1250) *temp = 1250;
		else if(*temp<-400) *temp = -400;
		return 0;
	}
	else return 1;	
	
}

//SHT30初始化
void SHT30_Init()
{
	IIC_Init();
}


