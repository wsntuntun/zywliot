#include <string.h>
#include <math.h>
#include "24cxx.h"
#include "delay.h"

//////////////////////////////////////////////////////////////////////////////////	 
//��˾��վ��http://www.zytdwl.cn/
//�Ա����̣�http://www.zytdwl.cn/  
//�人��������������Ƽ����޹�˾
//���ߣ����� wangzhen@zytdwl.cn
//��������:2020/3/28
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) �人��������������Ƽ����޹�˾ 2014-2024
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

//��ʼ��IIC
void IIC_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOBʱ��

  //GPIOB8,B9��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	IIC_SCL=1;
	IIC_SDA=1;
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
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
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
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
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}

u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    IIC_Start();  
	
		IIC_Send_Byte(0XA0);	   //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//���͸ߵ�ַ	    
	 
	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA1);           //�������ģʽ			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//����һ��ֹͣ����	    
	return temp;
}
//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
	
		IIC_Send_Byte(0XA0);	    //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ	  
	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//����һ��ֹͣ���� 
	delay_ms(10);	 
}
uchar read_random(u8 Iaddr,u8 Dataaddr,uint random_addr) 
// ��ָ����ַ��ȡ 
{ 
		u8 temp=0;		  	    																 
    IIC_Start();  
	
		//IIC_Send_Byte(0XA0);	   //����д���
	   if(Iaddr){
     if(Dataaddr){IIC_Send_Byte(HP_WRITE1); }	 else  IIC_Send_Byte(HP_WRITE);
	           }
   else {
     if(Dataaddr){IIC_Send_Byte(OP_WRITE1); }	 else  IIC_Send_Byte(OP_WRITE);
	           }
		IIC_Wait_Ack();
		IIC_Send_Byte(random_addr>>8);//���͸ߵ�ַ	    
	 
	IIC_Wait_Ack(); 
    IIC_Send_Byte(random_addr%256);   //���͵͵�ַ
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	//IIC_Send_Byte(0XA1);           //�������ģʽ		
 if(Iaddr){
     if(Dataaddr){IIC_Send_Byte(HP_READ1); }	 else  IIC_Send_Byte(HP_READ);
	           }
   else {
     if(Dataaddr){IIC_Send_Byte(OP_READ1); }	 else  IIC_Send_Byte(OP_READ);
	           }							 
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//����һ��ֹͣ����	    
	return temp;
		}	

uchar read_current(u8 Iaddr,u8 Dataaddr) 
// �ڵ�ǰ��ַ��ȡ 
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
//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void write_byte(u8 Iaddr,u8 Dataaddr,uint addr, uchar write_data) 
// ��ָ����ַaddr��д������write_data 
{ 
	    IIC_Start();  
	
		//IIC_Send_Byte(0XA0);	    //����д����
	   if(Iaddr){
      if(Dataaddr){IIC_Send_Byte(HP_WRITE1); }	else  IIC_Send_Byte(HP_WRITE);
            }
   else {
	  if(Dataaddr){IIC_Send_Byte(OP_WRITE1); }	else  IIC_Send_Byte(OP_WRITE);
        }
		IIC_Wait_Ack();
		IIC_Send_Byte(addr>>8);//���͸ߵ�ַ	  
	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(addr%256);   //���͵͵�ַ
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(write_data);     //�����ֽ�							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//����һ��ֹͣ���� 
	delay_ms(10);	
}
