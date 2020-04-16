#include "timer.h"
#include "init.h"
#include "sys.h"
#include "GPRS.h"
#include "delay.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////////////////////////	 
//公司网站：http://www.zytdwl.cn/
//淘宝店铺：http://www.zytdwl.cn/  
//武汉市中易天地物联科技有限公司
//作者：王振 wangzhen@zytdwl.cn
//创建日期:2017/3/28
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 武汉市中易天地物联科技有限公司 2014-2024
//All rights reserved										  
////////////////////////////////////////////////////////////////////////////////// 	 
extern u8 online;
extern u8 USART2RxFlag;
extern u8 USART3RxFlag;
extern u8 USART1RxFlag;
extern struct uart_data_buff uart4_buff,uart1_buff,uart3_buff;

static u16 Timout4 = 0;
static u16 Timout3 = 0;
static u16 Timout2 = 0;

extern struct data_relay_result relay_result_data[4];
extern struct trans_pump  transpump_En[4];

extern u8 ComFlag;	
//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	//TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

void TIM4_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///使能TIM4时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//初始化TIM4
	
	//TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //允许定时器4更新中断
	TIM_Cmd(TIM4,ENABLE); //使能定时器4
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;			//TIM4更新中断源
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 	//抢占优先级是1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//响应优先级是3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
}



void TIM5_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  ///使能TIM5时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE); //允许定时器5更新中断
	TIM_Cmd(TIM5,ENABLE); //使能定时器2
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; //定时器5中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,DISABLE);  //先关闭
	
}

void TIM2_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///使能TIM2时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//初始化TIM2
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //允许定时器2更新中断
	TIM_Cmd(TIM2,ENABLE); //使能定时器4
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;			//TIM4更新中断源
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 	//抢占优先级是1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//响应优先级是3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
}

void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
 
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource5);//PC5 连接到中断线5
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource7);//PE7 连接到中断线7
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource8);//PE8 连接到中断线8
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource9);//PE9 连接到中断线9
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource10);//PE10 连接到中断线10
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource11);//PE11 连接到中断线11
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource12);//PE12 连接到中断线12
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource13);//PE13 连接到中断线13
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0);//PB0 连接到中断线0
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);//PB1 连接到中断线1
	/* 配置EXTI_Line0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
  EXTI_Init(&EXTI_InitStructure);//配置
	
		/* 配置EXTI_Line1 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
  EXTI_Init(&EXTI_InitStructure);//配置
		/* 配置EXTI_Line5 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
  EXTI_Init(&EXTI_InitStructure);//配置
		/* 配置EXTI_Line7 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line7;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
  EXTI_Init(&EXTI_InitStructure);//配置
		/* 配置EXTI_Line8 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line8;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
  EXTI_Init(&EXTI_InitStructure);//配置
		/* 配置EXTI_Line9 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
  EXTI_Init(&EXTI_InitStructure);//配置
		/* 配置EXTI_Line10 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line10;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
  EXTI_Init(&EXTI_InitStructure);//配置
		/* 配置EXTI_Line11 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
  EXTI_Init(&EXTI_InitStructure);//配置
		/* 配置EXTI_Line12 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
  EXTI_Init(&EXTI_InitStructure);//配置
		/* 配置EXTI_Line13 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
  EXTI_Init(&EXTI_InitStructure);//配置
	
	/*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);*/
	
		/* EXTI line gpio config(PB0) */	
  /*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);*/
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//外部中断0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//外部中断1
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;//抢占优先级4
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//外部中断9-5
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//外部中断15-10
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
	   
}

void usart1_init(u32 bound){
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
  USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif
	
}

void usart2_init(u32 bound){
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2复用为USART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3复用为USART2
	
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2与GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA2，PA3

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART2, &USART_InitStructure); //初始化串口1
	
  USART_Cmd(USART2, ENABLE);  //使能串口1 
	
	//USART_ClearFlag(USART2, USART_FLAG_TC);
	
#if EN_USART2_RX	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif
	
}
void usart3_init(u32 bound){
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART3时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOB10复用为USART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11复用为USART3
		
	//PE15推挽输出，485模式控制  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; //GPIOE15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_SetBits(GPIOE,GPIO_Pin_15);
	GPIO_Init(GPIOE,&GPIO_InitStructure); //初始化PE15
	
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOB10与GPIOB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化PB10，PB11

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART3, &USART_InitStructure); //初始化串口1
	
  USART_Cmd(USART3, ENABLE);  //使能串口1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART3_RX	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif
	
}

u8 USART1_SendTxData1(u8 *SendData, u8 DataLen)
{	
	u8 i;
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  	{
  	}
//	RS485_1Se();
	for(i = 0; i < DataLen; i++)
	{
		USART_SendData(USART1, SendData[i]);
		
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  		{
  		}
	}
	//RS485_1Re();
	return i;
}
u8 USART2_SendTxData1(u8 *SendData, u8 DataLen)
{	
	u8 i;
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  	{
  	}
//	RS485_1Se();
	for(i = 0; i < DataLen; i++)
	{
		USART_SendData(USART2, SendData[i]);
		
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  		{
  		}
	}
	//RS485_1Re();
	return i;
}
u8 USART3_SendTxData1(u8 *SendData, u8 DataLen)
{	
	u8 i;
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
  	{
  	}
	RS485_3Se();
	for(i = 0; i < DataLen; i++)
	{
		USART_SendData(USART3, SendData[i]);
		
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
  		{
  		}
	delay_ms(2);
	}
	RS485_3Re();
	return i;
}

void EXTI0_IRQHandler(void)     //通道8
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET) //确保是否产生了EXTI Line中断
  {
	  // LED1 取反
	  if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0) == 1){ 
		PBout(5)=1;
	  relay_result_data[0].data[4] |=(1<<8);                                  }
    EXTI_ClearITPendingBit(EXTI_Line0);     //清除中断标志位
  }  
} 

void EXTI1_IRQHandler(void)       //通道7
{
  if(EXTI_GetITStatus(EXTI_Line1) != RESET) //确保是否产生了EXTI Line中断
  {
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) == 1){ 
		PDout(7)=1;
		relay_result_data[0].data[4]  |=(1<<9);                                  }
    EXTI_ClearITPendingBit(EXTI_Line1);     //清除中断标志位
  }  
}

void EXTI9_5_IRQHandler(void)        
{
	OSIntEnter();
  if(EXTI_GetITStatus(EXTI_Line5) != RESET) //确保是否产生了EXTI Line中断
  {
	  // 
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == 1){ 
		online=0; 
    ComFlag = 2;
	}
	EXTI_ClearITPendingBit(EXTI_Line5);     //清除中断标志位
  } 
	if(EXTI_GetITStatus(EXTI_Line7) != RESET) //确保是否产生了EXTI Line中断   
  {         
	  // //通道6
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7) == 1){ 
		PDout(6)=1;
		relay_result_data[0].data[4] |=(1<<10);                                    }
	EXTI_ClearITPendingBit(EXTI_Line7);     //清除中断标志位
  }
 if(EXTI_GetITStatus(EXTI_Line8) != RESET) //确保是否产生了EXTI Line中断
  {
	  // //通道5
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_8) == 1){ 
		PDout(5)=1;
		relay_result_data[0].data[4] |=(1<<11);                                   }
	EXTI_ClearITPendingBit(EXTI_Line8);     //清除中断标志位
  }
 if(EXTI_GetITStatus(EXTI_Line9) != RESET) //确保是否产生了EXTI Line中断
  {
	  // //通道4
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_9) == 1){ 
		PDout(4)=1;
		relay_result_data[0].data[4] |=(1<<12);                                   }
	EXTI_ClearITPendingBit(EXTI_Line9);     //清除中断标志位
  }
OSIntExit();
}

void EXTI15_10_IRQHandler(void)
{
	OSIntEnter(); 
	if(EXTI_GetITStatus(EXTI_Line10) != RESET) //确保是否产生了EXTI Line中断
  {
	  // //通道3
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10) == 1){ 
		PDout(3)=1;
		relay_result_data[0].data[4] |=(1<<13);                                    }
	EXTI_ClearITPendingBit(EXTI_Line10);     //清除中断标志位
  }
 if(EXTI_GetITStatus(EXTI_Line11) != RESET) //确保是否产生了EXTI Line中断
  {
	  // //通道2
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11) == 1){ 
		PDout(1)=1;
		relay_result_data[0].data[4] |=(1<<14);                                   }
	EXTI_ClearITPendingBit(EXTI_Line11);     //清除中断标志位
  }
 if(EXTI_GetITStatus(EXTI_Line12) != RESET) //确保是否产生了EXTI Line中断
  {
	  // //通道1
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12) == 1){ 
		PDout(0)=1;
		relay_result_data[0].data[4] |=(1<<15);                                   }
	EXTI_ClearITPendingBit(EXTI_Line12);     //清除中断标志位
  }
	 if(EXTI_GetITStatus(EXTI_Line13) != RESET) //确保是否产生了EXTI Line中断
  {
	  //
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13) == 1){      //水泵
		PBout(6)=1;
	transpump_En[0].dataflag[3]|=(1<<14);                                   }
	EXTI_ClearITPendingBit(EXTI_Line13);     //清除中断标志位
  }
OSIntExit();
}

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
	OSIntEnter(); 
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
			if(Timout3 >= 10)				//超时0.1s表示一帧传输完成
  		{
  			Timout3 = 0;	
  			USART3RxFlag = 1;			//置标志
  			TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);
  		}
  		else Timout3 ++;
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
	OSIntExit();
}
//定时器4中断服务函数
void TIM4_IRQHandler(void)
{
	OSIntEnter();
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //溢出中断
	{
		if(Timout4 >= 10)				//超时0.01s表示一帧传输完成
  		{
  			Timout4 = 0;	
  			USART1RxFlag = 1;			//置标志
  			TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE);
  		}
  		else Timout4 ++;
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //清除中断标志位
	OSIntExit();
}

void TIM2_IRQHandler(void)
{
	OSIntEnter();
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //溢出中断
	{
		Timout2++;
		if(Timout2>40){			//1m
      Timout2=0;
	if(!online){                         
      heartbeat(); 
	            }    
						      }
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
	OSIntExit();
}

void TIM5_IRQHandler(void)
{
	OSIntEnter();
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET) //溢出中断
	{
  			USART2RxFlag = 1;			//置标志
  			TIM_ITConfig(TIM5,TIM_IT_Update,DISABLE);
	}
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);  //清除中断标志位
	OSIntExit();
}
							 
#if EN_USART1_RX   		//如果使能了接收   	  
//接收缓存区 	
#define USART1_RX_BUFFER_SIZE 128
#define USART1_RX_BUFFER_MASK (USART1_RX_BUFFER_SIZE - 1) 	//接收缓冲,最大128个字节.
//接收到的数据长度
static u8 USART1_RxBuf[USART1_RX_BUFFER_SIZE];
static u8 USART1_RxHead = 0;
static u8 USART1_RxTail = 0;

void USART1_IRQHandler(void)//GPRS接收数据中断
{
	u8 tmpRxData = 0;
	u8 tmpRxHead = 0;
	#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
  #endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)	 	//获取中断标志
	{	
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		tmpRxData = USART_ReceiveData(USART1);
		tmpRxHead = USART1_RxHead & USART1_RX_BUFFER_MASK;	//calculate buffer index
		USART1_RxHead = tmpRxHead + 1;
		USART1_RxBuf[tmpRxHead] = tmpRxData; 			//store received data in buffer
				TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);//在UART4里使能TIM1产生更新事件中断
  				TIM_ClearFlag(TIM4,TIM_FLAG_Update);	//清中断标志
				Timout4 = 0;                            //超时计时清零
		}
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif		
} 

u8 USART1_ReadRxData(u8 *ReadData)
{
	u8 tmpRxTail = 0;
	u8 RxDataLen = 0;

  	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);			//禁止USART中断
  			
  	while(USART1_RxHead != USART1_RxTail)
	{
		tmpRxTail = USART1_RxTail & USART1_RX_BUFFER_MASK;	//calculate buffer index
		USART1_RxTail = tmpRxTail +1; 				//store new index
		
		ReadData[RxDataLen++] = USART1_RxBuf[tmpRxTail]; 	//read data
	}
	USART1_RxHead=0;
	USART1_RxTail=0;
	uart1_buff.rx_len=RxDataLen;		
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);			//使能USART中断
        USART_ClearITPendingBit(USART1,USART_IT_RXNE); 			//清掉RXNE标志
    //RxDataLen=0;    
       return RxDataLen;
}
#endif

#if EN_USART2_RX   		//如果使能了接收   	  
//接收缓存区 	
#define USART2_RX_BUFFER_SIZE 128
#define USART2_RX_BUFFER_MASK (USART2_RX_BUFFER_SIZE - 1) 	//接收缓冲,最大128个字节.
//接收到的数据长度
static u8 USART2_RxBuf[USART2_RX_BUFFER_SIZE];
static u8 USART2_RxHead = 0;
static u8 USART2_RxTail = 0;

void USART2_IRQHandler(void)//GPRS接收数据中断
{
	u8 tmpRxData = 0;
	u8 tmpRxHead = 0;
	#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
  #endif
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)	 	/*获取中断标志*/
	{	
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		tmpRxData = USART_ReceiveData(USART2);
		tmpRxHead = USART2_RxHead & USART2_RX_BUFFER_MASK;	//calculate buffer index
		USART2_RxHead = tmpRxHead + 1;
		USART2_RxBuf[tmpRxHead] = tmpRxData; 			//store received data in buffer
				TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);//在UART4里使能TIM1产生更新事件中断
  				TIM_ClearFlag(TIM5,TIM_FLAG_Update);	//清中断标志
				//Timout4 = 0;                            //超时计时清零
		}
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif		
} 

u8 USART2_ReadRxData(u8 *ReadData)
{
	u8 tmpRxTail = 0;
	u8 RxDataLen = 0;

  	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);			//禁止USART中断
  			
  	while(USART2_RxHead != USART2_RxTail)
	{
		tmpRxTail = USART2_RxTail & USART2_RX_BUFFER_MASK;	//calculate buffer index
		USART2_RxTail = tmpRxTail +1; 				//store new index
		
		ReadData[RxDataLen++] = USART2_RxBuf[tmpRxTail]; 	//read data
	}
	USART2_RxHead=0;
	USART2_RxTail=0;
	uart4_buff.rx_len=RxDataLen;		
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);			//使能USART中断
        USART_ClearITPendingBit(USART2,USART_IT_RXNE); 			//清掉RXNE标志
    //RxDataLen=0;    
       return RxDataLen;
}
#endif

#if EN_USART3_RX   		//如果使能了接收   	  
//接收缓存区 	
#define USART3_RX_BUFFER_SIZE 128
#define USART3_RX_BUFFER_MASK (USART3_RX_BUFFER_SIZE - 1) 	//接收缓冲,最大128个字节.
//接收到的数据长度
static u8 USART3_RxBuf[USART3_RX_BUFFER_SIZE];
static u8 USART3_RxHead = 0;
static u8 USART3_RxTail = 0;

void USART3_IRQHandler(void)//GPRS接收数据中断
{
	u8 tmpRxData = 0;
	u8 tmpRxHead = 0;
	#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
  #endif
	if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)	 	/*获取中断标志*/
	{	
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		tmpRxData = USART_ReceiveData(USART3);
		tmpRxHead = USART3_RxHead & USART3_RX_BUFFER_MASK;	//calculate buffer index
		USART3_RxHead = tmpRxHead + 1;
		USART3_RxBuf[tmpRxHead] = tmpRxData; 			//store received data in buffer
				TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//在UART4里使能TIM1产生更新事件中断
  				TIM_ClearFlag(TIM3,TIM_FLAG_Update);	//清中断标志
				Timout3 = 0;                            //超时计时清零
		}
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif	
} 

u8 USART3_ReadRxData(u8 *ReadData)
{
	u8 tmpRxTail = 0;
	u8 RxDataLen = 0;
	
  	USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);			//禁止USART中断
  			
  	while(USART3_RxHead != USART3_RxTail)
	{
		tmpRxTail = USART3_RxTail & USART3_RX_BUFFER_MASK;	//calculate buffer index
		USART3_RxTail = tmpRxTail +1; 				//store new index
		
		ReadData[RxDataLen++] = USART3_RxBuf[tmpRxTail]; 	//read data
	}
	USART3_RxHead=0;
	USART3_RxTail=0;
	uart3_buff.rx_len=RxDataLen;		
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);			//使能USART中断
        USART_ClearITPendingBit(USART3,USART_IT_RXNE); 			//清掉RXNE标志
    //RxDataLen=0;    
       return RxDataLen;
}
#endif	
