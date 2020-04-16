#ifndef __24CXX_H
#define __24CXX_H

//#include "myiic.h"
#include "sys.h"

#define   uchar unsigned char
#define   uint unsigned int
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//24CXX 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/6
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//void iicinit(void);
//uchar read_random(u8 Iaddr,u8 Dataaddr,uint random_addr);
//uchar read_current(u8 Iaddr,u8 Dataaddr);
//void write_byte(u8 Iaddr,u8 Dataaddr,uint addr, uchar write_data) ;

//IO方向设置
#define SDA_IN()  {GPIOA->MODER&=~(3<<(11*2));GPIOA->MODER|=0<<11*2;}	//PB9输入模式
#define SDA_OUT() {GPIOA->MODER&=~(3<<(11*2));GPIOA->MODER|=1<<11*2;} //PB9输出模式
//IO操作函数	 
#define IIC_SCL    PAout(12) //SCL
#define IIC_SDA    PAout(11) //SDA	 
#define READ_SDA   PAin(11)  //输入SDA 

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	
u8 AT24CXX_ReadOneByte(u16 ReadAddr);							//指定地址读取一个字节
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite);		//指定地址写入一个字节

uchar read_random(u8 Iaddr,u8 Dataaddr,uint random_addr);
uchar read_current(u8 Iaddr,u8 Dataaddr);
void write_byte(u8 Iaddr,u8 Dataaddr,uint addr, uchar write_data) ;


#endif
















