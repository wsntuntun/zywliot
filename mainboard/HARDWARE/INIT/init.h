#ifndef __INIT_H
#define __INIT_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

#define	RS485_3Se()		GPIO_ResetBits(GPIOE,GPIO_Pin_15)		//PE15
#define	RS485_3Re()		GPIO_SetBits(GPIOE,GPIO_Pin_15)

#define RX_STATE0 	0 		//?????
#define RX_STATE1 	1 		//?????
#define RX_STATE2 	2 		//??????
#define RX_STATE3 	3 		//???????
#define RX_STATE4 	4 		//?????????
#define RX_STATE5 	5 		//??CRC???

#define GPRS_SUCCESS_ACK 0xD0 //?????
#define GPRS_ERROR_ACK 0xD1 //?????
#define GPRS_ERROR_CMD 0xD2 //?????????,????
#define GPRS_ERROR_LENGTH 0xD3
#define GPRS_ERROR_CRC 0xD4 //????CRC????
#define GPRS_ERROR_ID 0xD5
#define GPRS_ERROR_Disable 0xD6


#define GPRS_OPER_SUCCESS 0xD0 //????
#define GPRS_OPER_ERROR 0xD1 //

#define TRANS_NOT_OPER 0xE0
#define TRANS_SUCCESS 0xE1
#define TRANS_ERROR_COMM 0xE2 //???????:????????
#define TRANS_NOT_EXIST 0xEE

#define RELAY_NOT_OPER 0xE0 //????????:???(????) 
#define RELAY_SUCCESS 0xE1 //?????
#define RELAY_ERROR_COMM 0xE2 //???????:????????
#define RELAY_NOT_EXIST 0xEE //??????

#define TASK_SUCCESS 0xE1 //????
#define TASK_NOT_EXIST 0xEE

#define PROBE_NOT_OPER 0xE0 //??????????:???(????) 
#define PROBE_SUCCESS 0xE1 //???????
#define PROBE_ERROR_COMM 0xE2 //
#define PROBE_ERROR_LEN 0xE3 //???????????
#define PROBE_ERROR_CRC 0xE4 //???????CRC??
#define PROBE_ERROR_OPER 0xE5 //
#define PROBE_ERROR_CALI 0xE6 //?????????
#define PROBE_ERROR_SALI 0xE7 //?????????
#define PROBE_ERROR_CMD 0xE8 //?????????
#define PROBE_NOT_EXIST 0xEE




//#define COMMAND_TEST 	1	

struct uart_data_buff {
		u8  rx_data_buff[100];
		u8  tx_data_buff[100];
		u8  rx_len;
		u8  tx_len;
};

struct id_data {
		u8 year_datah;     //主控ID
		u8 year_datal;
		u8 sn_datah;
		u8 sn_datal;
};

struct version_data {  //软硬件版本、SN
		u8 ver_data1;
		u8 ver_data2;
		u8 ver_data3;
	    u8 ver_data4;
	    u8 id_data[12];
};

struct yandu_data {  //盐度
		//u8 valid_data;
		u8 yandu_data1;
		u8 yandu_data2;
};

struct jiaozhen_data {  //校准
		u8 set_data1;
		u8 set_data2;
		u8 type_data;
		u8 para_data;
		u8 result_data;
		u16 set_data;

};

struct data_alarm {      //报警门限
		u8 integerh_temp1[16];
		u8 integerl_temp1[16];
    u8 integerh_temp2[16];
		u8 integerl_temp2[16]; 
    u8 integerh_temp0[16];
		u8 integerl_temp0[16];	

		u8 set_data1;   
		u8 set_data2;

		
		u16 set_data;
		//u16 integer_temp0[16];
		//u16 integer_temp1[16];
    //u16 integer_temp2[16];


	//	s16 DataMV_O2;
	//	s16 DataMV_PH;
		

};

struct Intelligent_control {  //智能控制使能
		u8 flag_data1;
	  u8 flag_data2;
	  u16 flag_data;
};

struct second_device_data {  //二级实际设备数量
		u8 trans_data;
	  u8 relay_data;
};

struct capacity_data  {     //系统最大容量设置
		u8 set_data1;
		u8 set_data2;
		u8 maxtrans_data;    //??????
		u8 maxsensor_data;   //????????
		u8 maxgradient_data;   //?????
		u8 maxcheck_data;   //????????
	  u8 maxrelay_data;     //??????
		u8 maxgallery_data;      //?????
		u8 maxungate_data;     //????????
	  u8 maxgate_data;   //????????
		u8 maxfixed_data;   //????????
	
};


struct system_information {//系统设置
		u8 display_time;
		u8 displayTo_time;
		u8 GPRS_time;
		u8 GPRSTo_time;
		u8 trans_time;
		u8 relay_time;
		u8 hand_time;
		u8 Alarm_time;
		u8 AlarmTo_time;
	  u8 set_data1;
	  u8 set_data2;
	  u16 set;
};

struct trans_query_date {  //变送器终端配置
		u8  ID;
		u8  state;
		u8  config_data1;
	    u8  config_data2;
		u8	probe_data1[16]; 
	    u8	probe_data2[16];
	u16  config_data;
		//u8  Intelligent_data1;
	//	u8  Intelligent_data2;
	//	u8 yandu_data1;
	//	u8 yandu_data2;
};

struct relay_query_date {   //继电器检测数据
     u8 data1[5];   //????
	   u8 data2[5];   //????
	   u16 data[5];
};
struct data_trans_result {   //传感器数据
		u8 integerh[16];
		u8 integerl[16]; 
	    u16 integer[16];
		u8 decimal[16];
		u8 flag[16];
		u8 mvdatah[16];
		u8 mvdatal[16];
	    u16 mvdata[16];
		u16 flagdata;
};

struct data_relay_result {  //继电器终端数据
	  
			u8  state;
	  u8  relaydata;

      u8 data1[16];   //????
	  u8 data2[16];   //????

	  u8 setData1;  //????
	  u8 setData2;  //????

	  u16 data[16];   //????
	
	  u16 setData;  //????

};

struct trans_alarm_enable {   //变送器报警
	  
      u16 data;   //????
	  u16 dataflag;   //??????

	  u8 set;  //??????
	  u8 setflag;  //??????

	  u8 datah;   
	  u8 datal;   

	  u8 dataflagh;
	  u8 dataflagl;
};

struct relay_alarm_enable { //继电器报警
	  
      u16 data;   //????
	  u16 dataflag;   //??????

	  u8 set;  //??????
	  u8 setflag;  //??????

	  u8 datah;   
	  u8 datal;   

	  u8 dataflagh;
	  u8 dataflagl;
};

struct relay_fixgate_enable { //继电器固定任务控制使能
	  
	  u8 set;  //??????
	  u8 setflag;  //??????

	  u8 datah;   
	  u8 datal;   

	  u8 dataflagh;
	  u8 dataflagl;
};

struct trans_pump {  //变送器水泵
	  
      u16 data;   //????
	  u16 dataflag[8];   //??????

	  u8 datah;   
	  u8 datal;   

	  u8 dataflagh[8];
	  u8 dataflagl[8];
};
struct shou_control {   //手动控制
	  
      //u16 timedata[16];   //??
	  u16 channeldata;   //??
	  u8  type[16];	   //????
	  u8  channelflag[16];	   //??
    u8   shouflag[16];
	  u8   guflag[16];
	  u8   zhiflag[16];
	  u8 timedatah[16];   
	  u8 timedatal[16];   

	  u8 channeldatah;
	  u8 channeldatal;
};
struct data_control {  //智能控制
		//u8 set_data1;		//????
	   //u8 set_data2;
		u8 gradient;   //????
		u8 gate_datal1[4];     //??
		u8 gate_datah1[4];
	  u8 gate_datal2[4]; 
		u8 gate_datah2[4];
		u8 relay_ID;     //???ID
		u8 relay_datah;    //?????
		u8 relay_datal;
		u8 relays[4];         //??????
		u8 contype[4];         //????
	  u16 gate_data1[4];
		u16 gate_data2[4];
		u16 relay_data;
		//u16 set_data;
};


struct Trans_control {
    struct data_control Transdata_control[16];
	u8 set_data1;		//????
	    u8 set_data2; 
			u16 set_data;
};


struct  setgate_Fixedopen {    //继电器固定时间任务
		u8 time_datal1[4];     //???
		u8 time_datah1[4];
	    u8 time_datal2[4]; 
		u8 time_datah2[4];
		u8 set_data[4];     //????
		//u16 time_data1[4];
		//u16 time_data2[4];


		u8 set_transl;     //????
		u8 set_transh;
	    u16 set_trans;
	    u8 trans_ID;     //???ID
		u8 set_time;     //????
		u8 relay_datah;    //?????
		u8 relay_datal;
	    u16 relay_data;
		u8 gate_type;     //??????
		u8 control_type;     //????
		u8 gate_datah;    //???
		u8 gate_datal;
	    u16 gate_data;
		u8 watch_datah;    //???
		u8 watch_datal;
	    u16 watch_data;

};
 
struct gate_Fixedopen {
     u8 gradient1;   //????1
	 u8 gradient2;   //????2
	 u16 gradient;   //????
	 struct setgate_Fixedopen relay_gateTask[8];
};

struct rtc_time {
	u8 tm_sec;
	u8 tm_min;
	u8 tm_hour;
	u8 tm_mday;
	u8 tm_week;
	u8 tm_mon;
	u8 tm_year;
};

void EepromWriteData_init(void);		  //初始数据写入存储器
void EepromReadData_init(void);			   //初始数据从存储器读出

void ReadIDData_init(void);
void ReadverData_init(void);
void ReadcomData_init(void);
void ReadenData_init(void);	
void ReadsysData_init(void);
void ReadcapData_init(void);
void ReadjzData_init(void);
void ReadsdData_init(void);
void ReadsetData_init(void);
void ReadinteData_init(void);	
void ReadyanData_init(void);
void ReadjiaoData_init(void);
void ReadalaData_init(void);
void ReadgateData_init(void);	
void ReadtransData_init(void);
void ReadrelayAlData_init(void);
//void ReadrelayungateData_init(void);
void ReadrelaygateData_init(void);	
void ReadrelaysetData_init(void);
void ReadtranspumpEN_init(void);
void ReadtransalarmEN_init(void);
void ReadrelayShouData_init(void);
void ReadrelaygateEnData_init(void);


//LED端口定义
#define LED0 PFout(9)	// DS0
#define LED1 PFout(10)	// DS1	 

void LED_Init(void);//初始化		 				    
#endif
