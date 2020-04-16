#include "timer.h"
#include "init.h"
#include "sys.h"
#include "GPRS.h"
#include "delay.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////////////////////////	 
//��˾��վ��http://www.zytdwl.cn/
//�Ա����̣�http://www.zytdwl.cn/  
//�人��������������Ƽ����޹�˾
//���ߣ����� wangzhen@zytdwl.cn
//��������:2017/3/28
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �人��������������Ƽ����޹�˾ 2014-2024
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
//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	//TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

void TIM4_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///ʹ��TIM4ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//��ʼ��TIM4
	
	//TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //����ʱ��4�����ж�
	TIM_Cmd(TIM4,ENABLE); //ʹ�ܶ�ʱ��4
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;			//TIM4�����ж�Դ
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 	//��ռ���ȼ���1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//��Ӧ���ȼ���3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
}



void TIM5_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  ///ʹ��TIM5ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE); //����ʱ��5�����ж�
	TIM_Cmd(TIM5,ENABLE); //ʹ�ܶ�ʱ��2
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; //��ʱ��5�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,DISABLE);  //�ȹر�
	
}

void TIM2_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///ʹ��TIM2ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//��ʼ��TIM2
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //����ʱ��2�����ж�
	TIM_Cmd(TIM2,ENABLE); //ʹ�ܶ�ʱ��4
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;			//TIM4�����ж�Դ
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 	//��ռ���ȼ���1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//��Ӧ���ȼ���3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
}

void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	
 
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource5);//PC5 ���ӵ��ж���5
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource7);//PE7 ���ӵ��ж���7
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource8);//PE8 ���ӵ��ж���8
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource9);//PE9 ���ӵ��ж���9
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource10);//PE10 ���ӵ��ж���10
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource11);//PE11 ���ӵ��ж���11
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource12);//PE12 ���ӵ��ж���12
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource13);//PE13 ���ӵ��ж���13
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0);//PB0 ���ӵ��ж���0
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);//PB1 ���ӵ��ж���1
	/* ����EXTI_Line0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
  EXTI_Init(&EXTI_InitStructure);//����
	
		/* ����EXTI_Line1 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
  EXTI_Init(&EXTI_InitStructure);//����
		/* ����EXTI_Line5 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
  EXTI_Init(&EXTI_InitStructure);//����
		/* ����EXTI_Line7 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line7;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
  EXTI_Init(&EXTI_InitStructure);//����
		/* ����EXTI_Line8 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line8;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
  EXTI_Init(&EXTI_InitStructure);//����
		/* ����EXTI_Line9 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
  EXTI_Init(&EXTI_InitStructure);//����
		/* ����EXTI_Line10 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line10;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
  EXTI_Init(&EXTI_InitStructure);//����
		/* ����EXTI_Line11 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
  EXTI_Init(&EXTI_InitStructure);//����
		/* ����EXTI_Line12 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
  EXTI_Init(&EXTI_InitStructure);//����
		/* ����EXTI_Line13 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
  EXTI_Init(&EXTI_InitStructure);//����
	
	/*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOC, &GPIO_InitStructure);*/
	
		/* EXTI line gpio config(PB0) */	
  /*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);*/
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//�ⲿ�ж�0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//��ռ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//�ⲿ�ж�1
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;//��ռ���ȼ�4
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//�ⲿ�ж�9-5
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//��ռ���ȼ�1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//�ⲿ�ж�15-10
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
	   
}

void usart1_init(u32 bound){
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
  USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif
	
}

void usart2_init(u32 bound){
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2����ΪUSART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3����ΪUSART2
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2��GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA2��PA3

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART2, &USART_InitStructure); //��ʼ������1
	
  USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���1 
	
	//USART_ClearFlag(USART2, USART_FLAG_TC);
	
#if EN_USART2_RX	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif
	
}
void usart3_init(u32 bound){
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART3ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOB10����ΪUSART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11����ΪUSART3
		
	//PE15���������485ģʽ����  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; //GPIOE15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_SetBits(GPIOE,GPIO_Pin_15);
	GPIO_Init(GPIOE,&GPIO_InitStructure); //��ʼ��PE15
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOB10��GPIOB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��PB10��PB11

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART3, &USART_InitStructure); //��ʼ������1
	
  USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART3_RX	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

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

void EXTI0_IRQHandler(void)     //ͨ��8
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET) //ȷ���Ƿ������EXTI Line�ж�
  {
	  // LED1 ȡ��
	  if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0) == 1){ 
		PBout(5)=1;
	  relay_result_data[0].data[4] |=(1<<8);                                  }
    EXTI_ClearITPendingBit(EXTI_Line0);     //����жϱ�־λ
  }  
} 

void EXTI1_IRQHandler(void)       //ͨ��7
{
  if(EXTI_GetITStatus(EXTI_Line1) != RESET) //ȷ���Ƿ������EXTI Line�ж�
  {
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) == 1){ 
		PDout(7)=1;
		relay_result_data[0].data[4]  |=(1<<9);                                  }
    EXTI_ClearITPendingBit(EXTI_Line1);     //����жϱ�־λ
  }  
}

void EXTI9_5_IRQHandler(void)        
{
	OSIntEnter();
  if(EXTI_GetITStatus(EXTI_Line5) != RESET) //ȷ���Ƿ������EXTI Line�ж�
  {
	  // 
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == 1){ 
		online=0; 
    ComFlag = 2;
	}
	EXTI_ClearITPendingBit(EXTI_Line5);     //����жϱ�־λ
  } 
	if(EXTI_GetITStatus(EXTI_Line7) != RESET) //ȷ���Ƿ������EXTI Line�ж�   
  {         
	  // //ͨ��6
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7) == 1){ 
		PDout(6)=1;
		relay_result_data[0].data[4] |=(1<<10);                                    }
	EXTI_ClearITPendingBit(EXTI_Line7);     //����жϱ�־λ
  }
 if(EXTI_GetITStatus(EXTI_Line8) != RESET) //ȷ���Ƿ������EXTI Line�ж�
  {
	  // //ͨ��5
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_8) == 1){ 
		PDout(5)=1;
		relay_result_data[0].data[4] |=(1<<11);                                   }
	EXTI_ClearITPendingBit(EXTI_Line8);     //����жϱ�־λ
  }
 if(EXTI_GetITStatus(EXTI_Line9) != RESET) //ȷ���Ƿ������EXTI Line�ж�
  {
	  // //ͨ��4
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_9) == 1){ 
		PDout(4)=1;
		relay_result_data[0].data[4] |=(1<<12);                                   }
	EXTI_ClearITPendingBit(EXTI_Line9);     //����жϱ�־λ
  }
OSIntExit();
}

void EXTI15_10_IRQHandler(void)
{
	OSIntEnter(); 
	if(EXTI_GetITStatus(EXTI_Line10) != RESET) //ȷ���Ƿ������EXTI Line�ж�
  {
	  // //ͨ��3
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10) == 1){ 
		PDout(3)=1;
		relay_result_data[0].data[4] |=(1<<13);                                    }
	EXTI_ClearITPendingBit(EXTI_Line10);     //����жϱ�־λ
  }
 if(EXTI_GetITStatus(EXTI_Line11) != RESET) //ȷ���Ƿ������EXTI Line�ж�
  {
	  // //ͨ��2
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11) == 1){ 
		PDout(1)=1;
		relay_result_data[0].data[4] |=(1<<14);                                   }
	EXTI_ClearITPendingBit(EXTI_Line11);     //����жϱ�־λ
  }
 if(EXTI_GetITStatus(EXTI_Line12) != RESET) //ȷ���Ƿ������EXTI Line�ж�
  {
	  // //ͨ��1
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12) == 1){ 
		PDout(0)=1;
		relay_result_data[0].data[4] |=(1<<15);                                   }
	EXTI_ClearITPendingBit(EXTI_Line12);     //����жϱ�־λ
  }
	 if(EXTI_GetITStatus(EXTI_Line13) != RESET) //ȷ���Ƿ������EXTI Line�ж�
  {
	  //
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13) == 1){      //ˮ��
		PBout(6)=1;
	transpump_En[0].dataflag[3]|=(1<<14);                                   }
	EXTI_ClearITPendingBit(EXTI_Line13);     //����жϱ�־λ
  }
OSIntExit();
}

//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
	OSIntEnter(); 
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{
			if(Timout3 >= 10)				//��ʱ0.1s��ʾһ֡�������
  		{
  			Timout3 = 0;	
  			USART3RxFlag = 1;			//�ñ�־
  			TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);
  		}
  		else Timout3 ++;
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
	OSIntExit();
}
//��ʱ��4�жϷ�����
void TIM4_IRQHandler(void)
{
	OSIntEnter();
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //����ж�
	{
		if(Timout4 >= 10)				//��ʱ0.01s��ʾһ֡�������
  		{
  			Timout4 = 0;	
  			USART1RxFlag = 1;			//�ñ�־
  			TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE);
  		}
  		else Timout4 ++;
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //����жϱ�־λ
	OSIntExit();
}

void TIM2_IRQHandler(void)
{
	OSIntEnter();
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //����ж�
	{
		Timout2++;
		if(Timout2>40){			//1m
      Timout2=0;
	if(!online){                         
      heartbeat(); 
	            }    
						      }
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //����жϱ�־λ
	OSIntExit();
}

void TIM5_IRQHandler(void)
{
	OSIntEnter();
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET) //����ж�
	{
  			USART2RxFlag = 1;			//�ñ�־
  			TIM_ITConfig(TIM5,TIM_IT_Update,DISABLE);
	}
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);  //����жϱ�־λ
	OSIntExit();
}
							 
#if EN_USART1_RX   		//���ʹ���˽���   	  
//���ջ����� 	
#define USART1_RX_BUFFER_SIZE 128
#define USART1_RX_BUFFER_MASK (USART1_RX_BUFFER_SIZE - 1) 	//���ջ���,���128���ֽ�.
//���յ������ݳ���
static u8 USART1_RxBuf[USART1_RX_BUFFER_SIZE];
static u8 USART1_RxHead = 0;
static u8 USART1_RxTail = 0;

void USART1_IRQHandler(void)//GPRS���������ж�
{
	u8 tmpRxData = 0;
	u8 tmpRxHead = 0;
	#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
  #endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)	 	//��ȡ�жϱ�־
	{	
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		tmpRxData = USART_ReceiveData(USART1);
		tmpRxHead = USART1_RxHead & USART1_RX_BUFFER_MASK;	//calculate buffer index
		USART1_RxHead = tmpRxHead + 1;
		USART1_RxBuf[tmpRxHead] = tmpRxData; 			//store received data in buffer
				TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);//��UART4��ʹ��TIM1���������¼��ж�
  				TIM_ClearFlag(TIM4,TIM_FLAG_Update);	//���жϱ�־
				Timout4 = 0;                            //��ʱ��ʱ����
		}
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif		
} 

u8 USART1_ReadRxData(u8 *ReadData)
{
	u8 tmpRxTail = 0;
	u8 RxDataLen = 0;

  	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);			//��ֹUSART�ж�
  			
  	while(USART1_RxHead != USART1_RxTail)
	{
		tmpRxTail = USART1_RxTail & USART1_RX_BUFFER_MASK;	//calculate buffer index
		USART1_RxTail = tmpRxTail +1; 				//store new index
		
		ReadData[RxDataLen++] = USART1_RxBuf[tmpRxTail]; 	//read data
	}
	USART1_RxHead=0;
	USART1_RxTail=0;
	uart1_buff.rx_len=RxDataLen;		
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);			//ʹ��USART�ж�
        USART_ClearITPendingBit(USART1,USART_IT_RXNE); 			//���RXNE��־
    //RxDataLen=0;    
       return RxDataLen;
}
#endif

#if EN_USART2_RX   		//���ʹ���˽���   	  
//���ջ����� 	
#define USART2_RX_BUFFER_SIZE 128
#define USART2_RX_BUFFER_MASK (USART2_RX_BUFFER_SIZE - 1) 	//���ջ���,���128���ֽ�.
//���յ������ݳ���
static u8 USART2_RxBuf[USART2_RX_BUFFER_SIZE];
static u8 USART2_RxHead = 0;
static u8 USART2_RxTail = 0;

void USART2_IRQHandler(void)//GPRS���������ж�
{
	u8 tmpRxData = 0;
	u8 tmpRxHead = 0;
	#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
  #endif
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)	 	/*��ȡ�жϱ�־*/
	{	
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		tmpRxData = USART_ReceiveData(USART2);
		tmpRxHead = USART2_RxHead & USART2_RX_BUFFER_MASK;	//calculate buffer index
		USART2_RxHead = tmpRxHead + 1;
		USART2_RxBuf[tmpRxHead] = tmpRxData; 			//store received data in buffer
				TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);//��UART4��ʹ��TIM1���������¼��ж�
  				TIM_ClearFlag(TIM5,TIM_FLAG_Update);	//���жϱ�־
				//Timout4 = 0;                            //��ʱ��ʱ����
		}
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif		
} 

u8 USART2_ReadRxData(u8 *ReadData)
{
	u8 tmpRxTail = 0;
	u8 RxDataLen = 0;

  	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);			//��ֹUSART�ж�
  			
  	while(USART2_RxHead != USART2_RxTail)
	{
		tmpRxTail = USART2_RxTail & USART2_RX_BUFFER_MASK;	//calculate buffer index
		USART2_RxTail = tmpRxTail +1; 				//store new index
		
		ReadData[RxDataLen++] = USART2_RxBuf[tmpRxTail]; 	//read data
	}
	USART2_RxHead=0;
	USART2_RxTail=0;
	uart4_buff.rx_len=RxDataLen;		
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);			//ʹ��USART�ж�
        USART_ClearITPendingBit(USART2,USART_IT_RXNE); 			//���RXNE��־
    //RxDataLen=0;    
       return RxDataLen;
}
#endif

#if EN_USART3_RX   		//���ʹ���˽���   	  
//���ջ����� 	
#define USART3_RX_BUFFER_SIZE 128
#define USART3_RX_BUFFER_MASK (USART3_RX_BUFFER_SIZE - 1) 	//���ջ���,���128���ֽ�.
//���յ������ݳ���
static u8 USART3_RxBuf[USART3_RX_BUFFER_SIZE];
static u8 USART3_RxHead = 0;
static u8 USART3_RxTail = 0;

void USART3_IRQHandler(void)//GPRS���������ж�
{
	u8 tmpRxData = 0;
	u8 tmpRxHead = 0;
	#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
  #endif
	if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)	 	/*��ȡ�жϱ�־*/
	{	
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		tmpRxData = USART_ReceiveData(USART3);
		tmpRxHead = USART3_RxHead & USART3_RX_BUFFER_MASK;	//calculate buffer index
		USART3_RxHead = tmpRxHead + 1;
		USART3_RxBuf[tmpRxHead] = tmpRxData; 			//store received data in buffer
				TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//��UART4��ʹ��TIM1���������¼��ж�
  				TIM_ClearFlag(TIM3,TIM_FLAG_Update);	//���жϱ�־
				Timout3 = 0;                            //��ʱ��ʱ����
		}
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif	
} 

u8 USART3_ReadRxData(u8 *ReadData)
{
	u8 tmpRxTail = 0;
	u8 RxDataLen = 0;
	
  	USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);			//��ֹUSART�ж�
  			
  	while(USART3_RxHead != USART3_RxTail)
	{
		tmpRxTail = USART3_RxTail & USART3_RX_BUFFER_MASK;	//calculate buffer index
		USART3_RxTail = tmpRxTail +1; 				//store new index
		
		ReadData[RxDataLen++] = USART3_RxBuf[tmpRxTail]; 	//read data
	}
	USART3_RxHead=0;
	USART3_RxTail=0;
	uart3_buff.rx_len=RxDataLen;		
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);			//ʹ��USART�ж�
        USART_ClearITPendingBit(USART3,USART_IT_RXNE); 			//���RXNE��־
    //RxDataLen=0;    
       return RxDataLen;
}
#endif	
