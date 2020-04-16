#include <string.h>
#include <math.h>
#include "24cxx.h"
#include "delay.h"

//////////////////////////////////////////////////////////////////////////////////	 
//¹«Ë¾ÍøÕ¾£ºhttp://www.zytdwl.cn/
//ÌÔ±¦µêÆÌ£ºhttp://www.zytdwl.cn/  
//ÎäººÊÐÖÐÒ×ÌìµØÎïÁª¿Æ¼¼ÓÐÏÞ¹«Ë¾
//×÷Õß£ºÍõÕñ wangzhen@zytdwl.cn
//´´½¨ÈÕÆÚ:2020/3/28
//°æ±¾£ºV1.1
//°æÈ¨ËùÓÐ£¬µÁ°æ±Ø¾¿¡£
//Copyright(C) ÎäººÊÐÖÐÒ×ÌìµØÎïÁª¿Æ¼¼ÓÐÏÞ¹«Ë¾ 2014-2024
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

//³õÊ¼»¯IIC
void IIC_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//Ê¹ÄÜGPIOBÊ±ÖÓ

  //GPIOB8,B9³õÊ¼»¯ÉèÖÃ
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//ÆÕÍ¨Êä³öÄ£Ê½
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//ÍÆÍìÊä³ö
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//ÉÏÀ­
  GPIO_Init(GPIOA, &GPIO_InitStructure);//³õÊ¼»¯
	IIC_SCL=1;
	IIC_SDA=1;
}
//²úÉúIICÆðÊ¼ÐÅºÅ
void IIC_Start(void)
{
	SDA_OUT();     //sdaÏßÊä³ö
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//Ç¯×¡I2C×ÜÏß£¬×¼±¸·¢ËÍ»ò½ÓÊÕÊý¾Ý 
}	  
//²úÉúIICÍ£Ö¹ÐÅºÅ
void IIC_Stop(void)
{
	SDA_OUT();//sdaÏßÊä³ö
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//·¢ËÍI2C×ÜÏß½áÊøÐÅºÅ
	delay_us(4);							   	
}
//µÈ´ýÓ¦´ðÐÅºÅµ½À´
//·µ»ØÖµ£º1£¬½ÓÊÕÓ¦´ðÊ§°Ü
//        0£¬½ÓÊÕÓ¦´ð³É¹¦
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDAÉèÖÃÎªÊäÈë  
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
	IIC_SCL=0;//Ê±ÖÓÊä³ö0 	   
	return 0;  
} 
//²úÉúACKÓ¦´ð
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
//²»²úÉúACKÓ¦´ð		    
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
//IIC·¢ËÍÒ»¸ö×Ö½Ú
//·µ»Ø´Ó»úÓÐÎÞÓ¦´ð
//1£¬ÓÐÓ¦´ð
//0£¬ÎÞÓ¦´ð			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//À­µÍÊ±ÖÓ¿ªÊ¼Êý¾Ý´«Êä
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //¶ÔTEA5767ÕâÈý¸öÑÓÊ±¶¼ÊÇ±ØÐëµÄ
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//¶Á1¸ö×Ö½Ú£¬ack=1Ê±£¬·¢ËÍACK£¬ack=0£¬·¢ËÍnACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDAÉèÖÃÎªÊäÈë
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
        IIC_NAck();//·¢ËÍnACK
    else
        IIC_Ack(); //·¢ËÍACK   
    return receive;
}

u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    IIC_Start();  
	
		IIC_Send_Byte(0XA0);	   //·¢ËÍÐ´ÃüÁî
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//·¢ËÍ¸ßµØÖ·	    
	 
	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //·¢ËÍµÍµØÖ·
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA1);           //½øÈë½ÓÊÕÄ£Ê½			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//²úÉúÒ»¸öÍ£Ö¹Ìõ¼þ	    
	return temp;
}
//ÔÚAT24CXXÖ¸¶¨µØÖ·Ð´ÈëÒ»¸öÊý¾Ý
//WriteAddr  :Ð´ÈëÊý¾ÝµÄÄ¿µÄµØÖ·    
//DataToWrite:ÒªÐ´ÈëµÄÊý¾Ý
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
	
		IIC_Send_Byte(0XA0);	    //·¢ËÍÐ´ÃüÁî
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//·¢ËÍ¸ßµØÖ·	  
	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //·¢ËÍµÍµØÖ·
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //·¢ËÍ×Ö½Ú							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//²úÉúÒ»¸öÍ£Ö¹Ìõ¼þ 
	delay_ms(10);	 
}
uchar read_random(u8 Iaddr,u8 Dataaddr,uint random_addr) 
// ÔÚÖ¸¶¨µØÖ·¶ÁÈ¡ 
{ 
		u8 temp=0;		  	    																 
    IIC_Start();  
	
		//IIC_Send_Byte(0XA0);	   //·¢ËÍÐ´ÃüÁ
	   if(Iaddr){
     if(Dataaddr){IIC_Send_Byte(HP_WRITE1); }	 else  IIC_Send_Byte(HP_WRITE);
	           }
   else {
     if(Dataaddr){IIC_Send_Byte(OP_WRITE1); }	 else  IIC_Send_Byte(OP_WRITE);
	           }
		IIC_Wait_Ack();
		IIC_Send_Byte(random_addr>>8);//·¢ËÍ¸ßµØÖ·	    
	 
	IIC_Wait_Ack(); 
    IIC_Send_Byte(random_addr%256);   //·¢ËÍµÍµØÖ·
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	//IIC_Send_Byte(0XA1);           //½øÈë½ÓÊÕÄ£Ê½		
 if(Iaddr){
     if(Dataaddr){IIC_Send_Byte(HP_READ1); }	 else  IIC_Send_Byte(HP_READ);
	           }
   else {
     if(Dataaddr){IIC_Send_Byte(OP_READ1); }	 else  IIC_Send_Byte(OP_READ);
	           }							 
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//²úÉúÒ»¸öÍ£Ö¹Ìõ¼þ	    
	return temp;
		}	

uchar read_current(u8 Iaddr,u8 Dataaddr) 
// ÔÚµ±Ç°µØÖ·¶ÁÈ¡ 
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
//ÔÚAT24CXXÖ¸¶¨µØÖ·Ð´ÈëÒ»¸öÊý¾Ý
//WriteAddr  :Ð´ÈëÊý¾ÝµÄÄ¿µÄµØÖ·    
//DataToWrite:ÒªÐ´ÈëµÄÊý¾Ý
void write_byte(u8 Iaddr,u8 Dataaddr,uint addr, uchar write_data) 
// ÔÚÖ¸¶¨µØÖ·addr´¦Ð´ÈëÊý¾Ýwrite_data 
{ 
	    IIC_Start();  
	
		//IIC_Send_Byte(0XA0);	    //·¢ËÍÐ´ÃüÁî
	   if(Iaddr){
      if(Dataaddr){IIC_Send_Byte(HP_WRITE1); }	else  IIC_Send_Byte(HP_WRITE);
            }
   else {
	  if(Dataaddr){IIC_Send_Byte(OP_WRITE1); }	else  IIC_Send_Byte(OP_WRITE);
        }
		IIC_Wait_Ack();
		IIC_Send_Byte(addr>>8);//·¢ËÍ¸ßµØÖ·	  
	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(addr%256);   //·¢ËÍµÍµØÖ·
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(write_data);     //·¢ËÍ×Ö½Ú							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//²úÉúÒ»¸öÍ£Ö¹Ìõ¼þ 
	delay_ms(10);	
}
