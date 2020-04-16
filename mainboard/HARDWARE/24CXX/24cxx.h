#ifndef __24CXX_H
#define __24CXX_H

//#include "myiic.h"
#include "sys.h"

#define   uchar unsigned char
#define   uint unsigned int
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//24CXX ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//void iicinit(void);
//uchar read_random(u8 Iaddr,u8 Dataaddr,uint random_addr);
//uchar read_current(u8 Iaddr,u8 Dataaddr);
//void write_byte(u8 Iaddr,u8 Dataaddr,uint addr, uchar write_data) ;

//IO��������
#define SDA_IN()  {GPIOA->MODER&=~(3<<(11*2));GPIOA->MODER|=0<<11*2;}	//PB9����ģʽ
#define SDA_OUT() {GPIOA->MODER&=~(3<<(11*2));GPIOA->MODER|=1<<11*2;} //PB9���ģʽ
//IO��������	 
#define IIC_SCL    PAout(12) //SCL
#define IIC_SDA    PAout(11) //SDA	 
#define READ_SDA   PAin(11)  //����SDA 

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	
u8 AT24CXX_ReadOneByte(u16 ReadAddr);							//ָ����ַ��ȡһ���ֽ�
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite);		//ָ����ַд��һ���ֽ�

uchar read_random(u8 Iaddr,u8 Dataaddr,uint random_addr);
uchar read_current(u8 Iaddr,u8 Dataaddr);
void write_byte(u8 Iaddr,u8 Dataaddr,uint addr, uchar write_data) ;


#endif
















