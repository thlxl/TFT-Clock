#include "myiic.h"
//#include "delay.h"
#include "esp_rom_sys.h"

 
//��ʼ��IIC
//�õ�//PG2,PG8
/*************xxxxx�޸ĳ����õĶ˿�PC4(SDA)��PC5(SCL)***************/
void IIC_Init(void)
{					     
	/*GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOC, ENABLE );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_4|GPIO_Pin_5); 	//PB10,PB11 �����*/

    gpio_config_t  GPIO_InitStructure;
    GPIO_InitStructure.pin_bit_mask = (1ull << 5)|(1ull << 18);
    GPIO_InitStructure.mode = GPIO_MODE_OUTPUT;
    GPIO_InitStructure.pull_down_en = 0;
    GPIO_InitStructure.pull_up_en = 1;
    GPIO_InitStructure.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&GPIO_InitStructure);
}



//IO��������(SDA)
/*********xxxxxxxxxxxxxx*************/
/*void SDA_IN()
{ 
	*//*GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);*//*

    gpio_config_t  GPIO_InitStructure;
    GPIO_InitStructure.pin_bit_mask = (1ull << 18);
    GPIO_InitStructure.mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.pull_down_en = 0;
    GPIO_InitStructure.pull_up_en = 1;
    GPIO_InitStructure.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&GPIO_InitStructure);
}
void SDA_OUT()
{ 
	*//*GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);*//*

    gpio_config_t  GPIO_InitStructure;
    GPIO_InitStructure.pin_bit_mask = (1ull << 18);
    GPIO_InitStructure.mode = GPIO_MODE_OUTPUT;
    GPIO_InitStructure.pull_down_en = 0;
    GPIO_InitStructure.pull_up_en = 1;
    GPIO_InitStructure.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&GPIO_InitStructure);
	IIC_SDA_Set();           //=1;
}*/


//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA_Set();    //=1;
	IIC_SCL_Set();    //=1;
    esp_rom_delay_us(4);        //delay_us(4);
 	IIC_SDA_Clr();        //=0;//START:when CLK is high,DATA change form high to low
    esp_rom_delay_us(4);        //delay_us(4);
	IIC_SCL_Clr();           //=0;//ǯסI2C���ߣ�׼�����ͻ��������
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL_Clr();//=0;
	IIC_SDA_Clr();//=0;//STOP:when CLK is high DATA change form low to high
    esp_rom_delay_us(4);            //delay_us(4);
	IIC_SCL_Set();//=1;
	IIC_SDA_Set();//=1;//����I2C���߽����ź�
    esp_rom_delay_us(4);          //delay_us(4);
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
/*********xxxx�޸ĳ�ʱʱ��************/
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	/*IIC_SDA=1;
    esp_rom_delay_us(3); */      //delay_us(3);
	IIC_SCL_Set();//=1;
    esp_rom_delay_us(3);          //delay_us(3);
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			printf("超时\n");
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_Clr();//=0;//ʱ�����0
	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL_Clr();//=0;
	SDA_OUT();
	IIC_SDA_Clr();//=0;
    esp_rom_delay_us(2);         //delay_us(2);
	IIC_SCL_Set();//=1;
    esp_rom_delay_us(2);          //delay_us(2);
	IIC_SCL_Clr();//=0;
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL_Clr();//=0;
	SDA_OUT();
	IIC_SDA_Set();//=1;
    esp_rom_delay_us(2);            //delay_us(2);
	IIC_SCL_Set();//=1;
    esp_rom_delay_us(2);            //delay_us(2);
	IIC_SCL_Clr();//=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
		SDA_OUT(); 	    
    IIC_SCL_Clr();//=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
		if((txd&0x80)>>7)
			IIC_SDA_Set();//=1;
		else
			IIC_SDA_Clr();//=0;
		txd<<=1;
        esp_rom_delay_us(2);            //delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL_Set();//=1;
        esp_rom_delay_us(2);            //delay_us(2);
		IIC_SCL_Clr();//=0;
        esp_rom_delay_us(2);             //delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
  for(i=0;i<8;i++ )
	{
        IIC_SCL_Clr();//=0;
        esp_rom_delay_us(100);         //delay_us(100);
		IIC_SCL_Set();//=1;
        receive<<=1;
        if(READ_SDA)receive++;
        esp_rom_delay_us(100);           //delay_us(100);
    }					 
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}

