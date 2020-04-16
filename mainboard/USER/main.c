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
u8 USART2RxFlag = 0;
u8 USART3RxFlag = 0;
u8 USART1RxFlag = 0;
u8 UART5RxFlag = 0;


u32 CpuID[3];  
u32 Lock_Code; 


u8 state[16];
u8 Alarmdata[128];


u8 Time_GetData[32];

//====================系统状态====================


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

u32 device_address;			//设备地址码
u8 device_communication;	 //设备通信方式
u8 Time_Update; 			//RTC时钟一秒更新标志，进入秒中断时置1，当时间被刷新之后清0

u8 uart4_rxData;			//uart4接收到的数据
u8 uart4_Rx_State = 0;		//uart4当前所处的接收状态
u32 uart4_delay_count = 0;	//uart4接收数据计时，1秒超时，接收状态回零，准备下一帧数据的接收

u8 online=0;
u8 yanflag = 0;
u8 zhunflag = 0;
u8 seVerflag = 0;
u8 trans_address=1;


//*
//时间结构体
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


/////////////////////////UCOSII任务设置///////////////////////////////////
//START 任务
//设置任务优先级
#define START_TASK_PRIO      			10 //开始任务的优先级设置为最低
//设置任务堆栈大小
#define START_STK_SIZE  				64
//任务堆栈	
OS_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata);	
 			   
//INIT任务
//设置任务优先级
#define INIT_TASK_PRIO       			7 
//设置任务堆栈大小
#define INIT_STK_SIZE  		    		512
//任务堆栈	
OS_STK INIT_TASK_STK[INIT_STK_SIZE];
//任务函数
void init_task(void *pdata);


//LCD任务
//设置任务优先级
#define LCD_TASK_PRIO       			6 
//设置任务堆栈大小
#define LCD_STK_SIZE  					256
//任务堆栈
OS_STK LCD_TASK_STK[LCD_STK_SIZE];
//任务函数
void lcd_task(void *pdata);

//433内部网络任务
//设置任务优先级
#define W433_TASK_PRIO       			4 
//设置任务堆栈大小
#define W433_STK_SIZE  					256
//任务堆栈
OS_STK W433_TASK_STK[W433_STK_SIZE];
//任务函数
void w433_task(void *pdata);

//GPRS网络任务
//设置任务优先级
#define GPRS_TASK_PRIO       			3 
//设置任务堆栈大小
#define GPRS_STK_SIZE  					1000
//任务堆栈
OS_STK GPRS_TASK_STK[GPRS_STK_SIZE];
//任务函数
void gprs_task(void *pdata);
/*
//智能控制任务
//设置任务优先级
#define AUTO_TASK_PRIO       			3 
//设置任务堆栈大小
#define AUTO_STK_SIZE  					64
//任务堆栈
OS_STK AUTO_TASK_STK[AUTO_STK_SIZE];
//任务函数
void auto_task(void *pdata);

//定时控制任务
//设置任务优先级
#define TIME_TASK_PRIO       			2 
//设置任务堆栈大小
#define TIME_STK_SIZE  					64
//任务堆栈
OS_STK TIME_TASK_STK[TIME_STK_SIZE];
//任务函数
void time_task(void *pdata);
*/
//手动控制任务
//设置任务优先级
#define HAND_TASK_PRIO       			5 
//设置任务堆栈大小
#define HAND_STK_SIZE  					512
//任务堆栈
OS_STK HAND_TASK_STK[HAND_STK_SIZE];
//任务函数
void hand_task(void *pdata);

int main(void)
{ 
 
	delay_init(168);		  //初始化延时函数
	LED_Init();		        //初始化LED端口 
	IIC_Init();
	My_RTC_Init();
	EepromWriteData_init();
	EepromReadData_init();
	usart1_init(9600);
	usart3_init(9600);
	EXTIX_Init();
	TIM2_Int_Init(5000-1,8400-1);	//定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数5000次为500ms
	TIM3_Int_Init(10-1,8400-1);	//定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数10次为1ms
	TIM4_Int_Init(10-1,8400-1);	//定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数10次为1ms
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//write_byte(0,0,0x0002,0x55);
	//delay_ms(3);
	//tiim=read_random(0,0,0x0002);
	OSInit();   
 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//创建起始任务
	OSStart();	
}

 //开始任务
void start_task(void *pdata)
{
    OS_CPU_SR cpu_sr=0;
	pdata = pdata; 
  OS_ENTER_CRITICAL();			//进入临界区(无法被中断打断)    
 	OSTaskCreate(init_task,(void *)0,(OS_STK*)&INIT_TASK_STK[INIT_STK_SIZE-1],INIT_TASK_PRIO);						   
 	//OSTaskCreate(lcd_task,(void *)0,(OS_STK*)&LCD_TASK_STK[LCD_STK_SIZE-1],LCD_TASK_PRIO);
	OSTaskCreate(w433_task,(void *)0,(OS_STK*)&W433_TASK_STK[W433_STK_SIZE-1],W433_TASK_PRIO);						   
 	OSTaskCreate(gprs_task,(void *)0,(OS_STK*)&GPRS_TASK_STK[GPRS_STK_SIZE-1],GPRS_TASK_PRIO);	
	//OSTaskCreate(auto_task,(void *)0,(OS_STK*)&AUTO_TASK_STK[AUTO_STK_SIZE-1],AUTO_TASK_PRIO);
	//OSTaskCreate(time_task,(void *)0,(OS_STK*)&TIME_TASK_STK[TIME_STK_SIZE-1],TIME_TASK_PRIO);						   
 	OSTaskCreate(hand_task,(void *)0,(OS_STK*)&HAND_TASK_STK[HAND_STK_SIZE-1],HAND_TASK_PRIO);
	OSTaskSuspend(START_TASK_PRIO);	//挂起起始任务.
	OS_EXIT_CRITICAL();				//退出临界区(可以被中断打断)
} 

//初始化任务
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
					trans_address=i+16*(transID[m].probe_data1[i]*2-4+transID[m].probe_data2[i]);//查询传感器数据
					trans_send(m+1,transID[m].config_data1,transID[m].config_data2);
				  state[i]=state[i]+1;
					delay_ms(50);		       }
					config=config<<1;
					if(state[i]==10){
						 state[i]=0;
						 trans_query_data[m].decimal[i]=0xE2;   //探头通信错误
								          }
											     }
				 OSTimeDlyHMSM(0,0,0,200);
			                            }
		                                      }
		delay_ms(300);
	};
}
/*
//显示任务
void lcd_task(void *pdata)
{	  
	while(1)
	{
		
		delay_ms(50);
		if(!online){     //建立连接
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
		if(!online){     //建立连接
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
