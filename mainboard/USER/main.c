#include "sys.h"
#include "delay.h"
//#include "usart.h"
#include "init.h"
#include "includes.h"
#include "timer.h"
#include "rtc.h"
#include "24cxx.h"
#include "GPRS.h"
#include "dataprocess.h"
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
u8 USART2RxFlag = 0;
u8 USART3RxFlag = 0;
u8 USART1RxFlag = 0;
u8 UART5RxFlag = 0;


u32 CpuID[3];  
u32 Lock_Code; 


u8 state[16];
u8 Alarmdata[128];


u8 Time_GetData[32];

//====================ϵͳ״̬====================


u8  IDDataflag,verDataflag,comDataflag,enDataflag=0;
u8  sysDataflag,capDataflag,jzDataflag,sdDataflag=0;
u8  setDataflag[4],inteDataflag[4],yanDataflag[4],jiaoDataflag=0;
u8  alaDataflag[4],gateDataflag[4],transDataflag,relayshouDataflag[4];
u8  relaygateDataflag[4],relayalDataflag[4],relaysetflag[4],relaygateENflag[4];
u8  transalarmENflag[4],transpumpENflag=0;

u8  transflag[4],relayflag[4];


  u16 Transdata[16];


  u8 ComFlag = 0;			//
	u8 enFlag=0;

u32 device_address;			//�豸��ַ��
u8 device_communication;	 //�豸ͨ�ŷ�ʽ
u8 Time_Update; 			//RTCʱ��һ����±�־���������ж�ʱ��1����ʱ�䱻ˢ��֮����0

u8 uart4_rxData;			//uart4���յ�������
u8 uart4_Rx_State = 0;		//uart4��ǰ�����Ľ���״̬
u32 uart4_delay_count = 0;	//uart4�������ݼ�ʱ��1�볬ʱ������״̬���㣬׼����һ֡���ݵĽ���

u8 online=0;
u8 yanflag = 0;
u8 zhunflag = 0;
u8 seVerflag = 0;
u8 trans_address=1;


//*
//ʱ��ṹ��
RTC_TimeTypeDef RTC_TimeStruct;
RTC_DateTypeDef RTC_DateStruct;
struct rtc_time systmtime,settmtime;
struct uart_data_buff uart4_buff,uart1_buff,uart3_buff;
struct id_data device_id;
struct second_device_data second_device;
struct version_data program_ver;
struct yandu_data device_yandu[4];
struct jiaozhen_data device_jiaozhun[4];//*/
//extern struct data_alarm ppm_prealarm_value;
struct data_alarm data_alarm_value[4];
struct Intelligent_control Intelligent_value[4];
struct shou_control shoudong_control[4];

struct trans_query_date transID[4];
struct data_trans_result trans_query_data[4];
struct system_information system_info;
struct relay_query_date relay_date[4];
struct data_relay_result relay_result_data[4];
struct Trans_control control_info[4];
struct capacity_data capacity_info;
//struct jiaozhun_Fixed  jiaozhun_info;
struct trans_alarm_enable  transAlarm_En[4];
struct trans_pump  transpump_En[4];
struct relay_alarm_enable  relayAlarm_En[4];
//struct ungate_Fixedopen relay_ungate_data[4];
struct gate_Fixedopen relay_gate_data[4];
struct relay_fixgate_enable relay_gate_En[4];



u8 tiim;


/////////////////////////UCOSII��������///////////////////////////////////
//START ����
//�����������ȼ�
#define START_TASK_PRIO      			10 //��ʼ��������ȼ�����Ϊ���
//���������ջ��С
#define START_STK_SIZE  				64
//�����ջ	
OS_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *pdata);	
 			   
//INIT����
//�����������ȼ�
#define INIT_TASK_PRIO       			7 
//���������ջ��С
#define INIT_STK_SIZE  		    		512
//�����ջ	
OS_STK INIT_TASK_STK[INIT_STK_SIZE];
//������
void init_task(void *pdata);


//LCD����
//�����������ȼ�
#define LCD_TASK_PRIO       			6 
//���������ջ��С
#define LCD_STK_SIZE  					256
//�����ջ
OS_STK LCD_TASK_STK[LCD_STK_SIZE];
//������
void lcd_task(void *pdata);

//433�ڲ���������
//�����������ȼ�
#define W433_TASK_PRIO       			4 
//���������ջ��С
#define W433_STK_SIZE  					256
//�����ջ
OS_STK W433_TASK_STK[W433_STK_SIZE];
//������
void w433_task(void *pdata);

//GPRS��������
//�����������ȼ�
#define GPRS_TASK_PRIO       			3 
//���������ջ��С
#define GPRS_STK_SIZE  					1000
//�����ջ
OS_STK GPRS_TASK_STK[GPRS_STK_SIZE];
//������
void gprs_task(void *pdata);
/*
//���ܿ�������
//�����������ȼ�
#define AUTO_TASK_PRIO       			3 
//���������ջ��С
#define AUTO_STK_SIZE  					64
//�����ջ
OS_STK AUTO_TASK_STK[AUTO_STK_SIZE];
//������
void auto_task(void *pdata);

//��ʱ��������
//�����������ȼ�
#define TIME_TASK_PRIO       			2 
//���������ջ��С
#define TIME_STK_SIZE  					64
//�����ջ
OS_STK TIME_TASK_STK[TIME_STK_SIZE];
//������
void time_task(void *pdata);
*/
//�ֶ���������
//�����������ȼ�
#define HAND_TASK_PRIO       			5 
//���������ջ��С
#define HAND_STK_SIZE  					512
//�����ջ
OS_STK HAND_TASK_STK[HAND_STK_SIZE];
//������
void hand_task(void *pdata);

int main(void)
{ 
 
	delay_init(168);		  //��ʼ����ʱ����
	LED_Init();		        //��ʼ��LED�˿� 
	IIC_Init();
	My_RTC_Init();
	EepromWriteData_init();
	EepromReadData_init();
	usart1_init(9600);
	usart3_init(9600);
	EXTIX_Init();
	TIM2_Int_Init(5000-1,8400-1);	//��ʱ��ʱ��84M����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�����5000��Ϊ500ms
	TIM3_Int_Init(10-1,8400-1);	//��ʱ��ʱ��84M����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�����10��Ϊ1ms
	TIM4_Int_Init(10-1,8400-1);	//��ʱ��ʱ��84M����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�����10��Ϊ1ms
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//write_byte(0,0,0x0002,0x55);
	//delay_ms(3);
	//tiim=read_random(0,0,0x0002);
	OSInit();   
 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//������ʼ����
	OSStart();	
}

 //��ʼ����
void start_task(void *pdata)
{
    OS_CPU_SR cpu_sr=0;
	pdata = pdata; 
  OS_ENTER_CRITICAL();			//�����ٽ���(�޷����жϴ��)    
 	OSTaskCreate(init_task,(void *)0,(OS_STK*)&INIT_TASK_STK[INIT_STK_SIZE-1],INIT_TASK_PRIO);						   
 	//OSTaskCreate(lcd_task,(void *)0,(OS_STK*)&LCD_TASK_STK[LCD_STK_SIZE-1],LCD_TASK_PRIO);
	OSTaskCreate(w433_task,(void *)0,(OS_STK*)&W433_TASK_STK[W433_STK_SIZE-1],W433_TASK_PRIO);						   
 	OSTaskCreate(gprs_task,(void *)0,(OS_STK*)&GPRS_TASK_STK[GPRS_STK_SIZE-1],GPRS_TASK_PRIO);	
	//OSTaskCreate(auto_task,(void *)0,(OS_STK*)&AUTO_TASK_STK[AUTO_STK_SIZE-1],AUTO_TASK_PRIO);
	//OSTaskCreate(time_task,(void *)0,(OS_STK*)&TIME_TASK_STK[TIME_STK_SIZE-1],TIME_TASK_PRIO);						   
 	OSTaskCreate(hand_task,(void *)0,(OS_STK*)&HAND_TASK_STK[HAND_STK_SIZE-1],HAND_TASK_PRIO);
	OSTaskSuspend(START_TASK_PRIO);	//������ʼ����.
	OS_EXIT_CRITICAL();				//�˳��ٽ���(���Ա��жϴ��)
} 

//��ʼ������
void init_task(void *pdata)
{	 
 u8 i=0;	
 u8 m=0;
 u16 config;
	while(1)
	{
		delay_ms(300);
		EepromReadData_init();
		//AT24CXX_WriteOneByte(0x0001,0x5a);
	  //write_byte(0,0,0x0001,0x5a);
		OSTimeDlyHMSM(0,0,0,5);
		pump();
		relay();
for (m=0;m<second_device.trans_data;m++){
			 if(transID[m].state!=0xee){
				 config=transID[m].config_data;
				 	for(i=1;i<16;i++){
			if((config&0x4000)==0x4000){
					trans_address=i+16*(transID[m].probe_data1[i]*2-4+transID[m].probe_data2[i]);//��ѯ����������
					trans_send(m+1,transID[m].config_data1,transID[m].config_data2);
				  state[i]=state[i]+1;
					delay_ms(50);		       }
					config=config<<1;
					if(state[i]==10){
						 state[i]=0;
						 trans_query_data[m].decimal[i]=0xE2;   //̽ͷͨ�Ŵ���
								          }
											     }
				 OSTimeDlyHMSM(0,0,0,200);
			                            }
		                                      }
		delay_ms(300);
	};
}
/*
//��ʾ����
void lcd_task(void *pdata)
{	  
	while(1)
	{
		
		delay_ms(50);
		if(!online){     //��������
			START_TIME2;
		             }
		uart1_buff.tx_data_buff[0] = tiim;
		uart1_buff.tx_data_buff[1] = tiim;
		uart1_buff.tx_data_buff[2] = tiim;
		USART2_SendTxData1(uart1_buff.tx_data_buff, 3);
		delay_ms(50);
	};
}
*/
void w433_task(void *pdata)
{	  
	while(1)
	{//delay_ms(300);
		OSTimeDlyHMSM(0,0,0,300);
  if(USART3RxFlag){
			memset(uart1_buff.rx_data_buff,'\0',sizeof(uart1_buff.rx_data_buff));
      USART3_ReadRxData(uart1_buff.rx_data_buff);
			delay_ms(5);
			USART3RxFlag=0;
      sence_rec();
                  }
		//delay_ms(300);
		OSTimeDlyHMSM(0,0,0,300);					
	};
}

void gprs_task(void *pdata)
{	  
	while(1)
	{
		OSTimeDlyHMSM(0,0,0,300);
		if(!online){     //��������
			START_TIME2;
		           }
    if(USART1RxFlag){
			memset(uart4_buff.rx_data_buff,'\0',sizeof(uart4_buff.rx_data_buff));
      USART1_ReadRxData(uart4_buff.rx_data_buff);
			delay_ms(5);
			USART1RxFlag=0;
      USART2_Command();
                    }
		if(yanflag){
		yanflag=0;
		OSTaskSuspend(HAND_TASK_PRIO);	
    OSTimeDlyHMSM(0,0,0,500);			
		yandu_write(uart4_buff.rx_data_buff[11],uart4_buff.rx_data_buff[12],uart4_buff.rx_data_buff[13]);
		//delay_ms(500);
		OSTimeDlyHMSM(0,0,0,500);
		OSTaskResume(HAND_TASK_PRIO);
		            }
		if(zhunflag){
		zhunflag=0;
		OSTaskSuspend(HAND_TASK_PRIO);	
    OSTimeDlyHMSM(0,0,0,500);	
	  jiaozhun_send(uart4_buff.rx_data_buff[11],uart4_buff.rx_data_buff[14]);
		//delay_ms(500);
		OSTimeDlyHMSM(0,0,0,500);
		OSTaskResume(HAND_TASK_PRIO);
		             }
		//delay_ms(300);
		OSTimeDlyHMSM(0,0,0,300);					
	};
}
void hand_task(void *pdata)
{	 u8 m=0; 
	while(1)
	{
		//delay_ms(300);
		OSTimeDlyHMSM(0,0,0,300);
    relayshoutask();
		relaygutask();
		relayzhitask();
		for (m=0;m<second_device.relay_data;m++){
			 if(relay_result_data[m].state!=0xee){
				 relay_send(m+1);
			 OSTimeDlyHMSM(0,0,0,100);
			                                     }
		                                         }
		//delay_ms(300);
		OSTimeDlyHMSM(0,0,0,300);																					
	};
}
