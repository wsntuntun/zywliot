#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/6/16
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口2接收
#define EN_USART3_RX 			1		//使能（1）/禁止（0）串口3接收

#define START_TIME2  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);TIM_Cmd(TIM2, ENABLE)
#define STOP_TIME2  TIM_Cmd(TIM2, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE)

void TIM2_Int_Init(u16 arr,u16 psc);
void TIM3_Int_Init(u16 arr,u16 psc);
void TIM4_Int_Init(u16 arr,u16 psc);
void usart1_init(u32 bound);
void usart2_init(u32 bound);
void usart3_init(u32 bound);
u8 USART1_ReadRxData(u8 *ReadData);
u8 USART2_ReadRxData(u8 *ReadData);
u8 USART3_ReadRxData(u8 *ReadData);
void EXTIX_Init(void);	//外部中断初始化
u8 USART1_SendTxData1(u8 *SendData, u8 DataLen);
u8 USART2_SendTxData1(u8 *SendData, u8 DataLen);
u8 USART3_SendTxData1(u8 *SendData, u8 DataLen);
#endif
