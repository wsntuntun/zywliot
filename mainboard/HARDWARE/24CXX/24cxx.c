#include <string.h>
#include <math.h>
#include "24cxx.h"
#include "delay.h"

//////////////////////////////////////////////////////////////////////////////////	 
//公司网站：http://www.zytdwl.cn/
//淘宝店铺：http://www.zytdwl.cn/  
//武汉市中易天地物联科技有限公司
//作者：王振 wangzhen@zytdwl.cn
//创建日期:2020/3/28
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 武汉市中易天地物联科技有限公司 2014-2024
//All rights reserved											  
//********************************************************************************
#define uchar unsigned char
#define uint unsigned int
 
/**********************************************************/ 

 #define OP_WRITE 0xa0           
 #define OP_READ  0xa1           

 #define OP_WRITE1 0xa2           
 #define OP_READ1  0xa3           
 
 #define HP_WRITE 0xa4          
 #define HP_READ  0xa5           

 #define HP_WRITE1 0xa6          
 #define HP_READ1  0xa7            

//初始化IIC
void IIC_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOB时钟

  //GPIOB8,B9初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
	IIC_SCL=1;
	IIC_SDA=1;
}
//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}

u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    IIC_Start();  
	
		IIC_Send_Byte(0XA0);	   //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//发送高地址	    
	 
	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //发送低地址
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA1);           //进入接收模式			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//产生一个停止条件	    
	return temp;
}
//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
	
		IIC_Send_Byte(0XA0);	    //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//发送高地址	  
	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //发送低地址
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //发送字节							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//产生一个停止条件 
	delay_ms(10);	 
}
uchar read_random(u8 Iaddr,u8 Dataaddr,uint random_addr) 
// 在指定地址读取 
{ 
		u8 temp=0;		  	    																 
    IIC_Start();  
	
		//IIC_Send_Byte(0XA0);	   //发送写命�
	   if(Iaddr){
     if(Dataaddr){IIC_Send_Byte(HP_WRITE1); }	 else  IIC_Send_Byte(HP_WRITE);
	           }
   else {
     if(Dataaddr){IIC_Send_Byte(OP_WRITE1); }	 else  IIC_Send_Byte(OP_WRITE);
	           }
		IIC_Wait_Ack();
		IIC_Send_Byte(random_addr>>8);//发送高地址	    
	 
	IIC_Wait_Ack(); 
    IIC_Send_Byte(random_addr%256);   //发送低地址
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	//IIC_Send_Byte(0XA1);           //进入接收模式		
 if(Iaddr){
     if(Dataaddr){IIC_Send_Byte(HP_READ1); }	 else  IIC_Send_Byte(HP_READ);
	           }
   else {
     if(Dataaddr){IIC_Send_Byte(OP_READ1); }	 else  IIC_Send_Byte(OP_READ);
	           }							 
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//产生一个停止条件	    
	return temp;
		}	

uchar read_current(u8 Iaddr,u8 Dataaddr) 
// 在当前地址读取 
{ 
	   uchar read_data; 
   IIC_Start(); 
   if(Iaddr){
     if(Dataaddr){IIC_Send_Byte(HP_READ1); }	 else  IIC_Send_Byte(HP_READ);
	           }
   else {
     if(Dataaddr){IIC_Send_Byte(OP_READ1); }	 else  IIC_Send_Byte(OP_READ);
	           }			    
   //delayNOP(); 
   read_data = IIC_Read_Byte(0); 
   IIC_Stop(); 
   return read_data; 
}
//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void write_byte(u8 Iaddr,u8 Dataaddr,uint addr, uchar write_data) 
// 在指定地址addr处写入数据write_data 
{ 
	    IIC_Start();  
	
		//IIC_Send_Byte(0XA0);	    //发送写命令
	   if(Iaddr){
      if(Dataaddr){IIC_Send_Byte(HP_WRITE1); }	else  IIC_Send_Byte(HP_WRITE);
            }
   else {
	  if(Dataaddr){IIC_Send_Byte(OP_WRITE1); }	else  IIC_Send_Byte(OP_WRITE);
        }
		IIC_Wait_Ack();
		IIC_Send_Byte(addr>>8);//发送高地址	  
	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(addr%256);   //发送低地址
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(write_data);     //发送字节							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//产生一个停止条件 
	delay_ms(10);	
}
