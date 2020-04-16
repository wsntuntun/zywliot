#include "GPRS.h"
#include "dataprocess.h"
#include "delay.h"
#include "24cxx.h"
#include "rtc.h"
#include "timer.h"
#include "init.h"
#include "includes.h"
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
extern u32 device_address;
extern u8 device_communication;

extern u8 uart4_rxData;
extern u8 uart4_Rx_State;
extern u32 uart4_delay_count;
//extern u8 uart4_RxFrame_Over;
extern u8 UART5RxFlag;
extern u8 zhunflag;
extern u8 yanflag;
extern u8 seVerflag;

extern u8 online;
extern u8 enFlag;
extern u8 ComFlag;
extern u8 StartFlag;				//开始标志
extern u8 StopFlag;				//停止标志

extern struct uart_data_buff uart4_buff,uart1_buff;
extern struct id_data device_id;
extern struct version_data program_ver;
extern struct yandu_data device_yandu[4];
extern struct jiaozhen_data device_jiaozhun[4];
//extern struct data_alarm ppm_prealarm_value;
extern struct data_alarm data_alarm_value[4];
extern struct Intelligent_control Intelligent_value[4];

extern struct trans_query_date transID[4];
extern struct data_trans_result trans_query_data[4];
extern struct system_information system_info;
extern struct relay_query_date relay_date[4];
extern struct data_relay_result relay_result_data[4];
extern struct Trans_control control_info[4];
extern struct second_device_data second_device;
extern struct shou_control shoudong_control[4];
//extern struct jiaozhun_Fixed  jiaozhun_info;
//extern struct ungate_Fixedopen relay_ungate_data[4];
extern struct gate_Fixedopen relay_gate_data[4];
extern struct capacity_data capacity_info;
extern struct rtc_time settmtime;
extern struct rtc_time systmtime;
extern struct trans_alarm_enable  transAlarm_En[4];
extern struct trans_pump  transpump_En[4];
extern struct relay_alarm_enable  relayAlarm_En[4];
extern struct relay_fixgate_enable relay_gate_En[4];

extern u8 setflag;			//变送器配置标志
extern u8 openfixflag ;			//继电器固定开启任务（无门限）配置标志
extern u8 openunfixflag;			//继电器固定开启任务（有门限）配置标志
extern u8 fixedflag ;			//固定时间校正配置标志

extern u8 Time_GetData[32];
extern u32 CpuID[3];  
extern u32 Lock_Code;

extern u8  IDDataflag,verDataflag,comDataflag,enDataflag;
extern u8  sysDataflag,capDataflag,jzDataflag,sdDataflag;
extern u8  setDataflag[4],inteDataflag[4],yanDataflag[4],jiaoDataflag;
extern u8  alaDataflag[4],gateDataflag[4],transDataflag,relayshouDataflag[4];
extern u8  relaygateDataflag[4],relayalDataflag[4],relaysetflag[4],relaygateENflag[4];
extern u8  transalarmENflag[4],transpumpENflag[4];
extern u8 shoudong[16];
extern u8 zidong[16];
extern u8 guding[16];
extern u8 Alarmdata[128];
extern u8  transflag[4],relayflag[4];


extern u16  timeout_flag;
//CRC16查表计算高位字节表
uc8 aucCRCHi[] = 
{
   0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
   0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
   0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
   0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
   0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
   0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
   0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
   0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
   0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
   0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
   0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
   0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
   0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
   0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
   0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
   0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
   0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
   0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
   0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
   0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
   0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
   0x00, 0xC1, 0x81, 0x40
};

//CRC16查表计算低位字节表
uc8 aucCRCLo[] = 
{
   0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
   0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
   0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
   0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
   0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
   0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
   0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
   0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
   0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
   0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
   0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
   0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
   0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
   0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
   0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
   0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
   0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
   0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
   0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
   0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
   0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
   0x41, 0x81, 0x80, 0x40
};

void SoftReset(void) 
{  __set_FAULTMASK(1);      // 关闭所有中端
 NVIC_SystemReset();// 复位
 
}
  
void GetLockCode(void)  
{  
//获取CPU唯一ID  
CpuID[0]=*(vu32*)(0x1ffff7e8);  
CpuID[1]=*(vu32*)(0x1ffff7ec);  
CpuID[2]=*(vu32*)(0x1ffff7f0);  
//加密算法,很简单的加密算法  
Lock_Code=(CpuID[0]>>1)+(CpuID[1]>>2)+(CpuID[2]>>3);  
} 

/**********************************************************
 *
 * @file	: mem_copy_byte()
 * @param	: u8 *source: 原始数组
 *			  u8 *direct: 目标数组
 *       	  u8 length: 要拷贝的数据长度
 * @retval	: None
 * @brief	: 从原始数组拷贝length个长度的数据到目标数组中
 *
 **********************************************************/
void mem_copy_byte(u8 *source, u8 *direct, u8 length)//拷贝字符型数组
{
	while(length--)
	{
		*direct++ = *source++;
	}
}

/**********************************************************
 *
 * @file	: UART4_Command()
 * @param	: None
 * @retval	: None
 * @brief	: 上位机发送的帧数据解析及回应
 *
 **********************************************************/
void USART2_Command(void)
{
	u16 temp;
	u8 crc_datah, crc_datal;
	u8 comend_Len, comend_Buff[100]; 	//Modbus帧数据BUFF

	//if (UART5RxFlag==1)		 	//Modbus帧一帧数据接收完成，进行命令字解析
	{
		comend_Len = uart4_buff.rx_len;
		mem_copy_byte((u8 *)uart4_buff.rx_data_buff, (u8 *)comend_Buff, comend_Len);//缓存数据

		//UART5RxFlag = 0x00;	//上一帧数据缓存完成，允许接收下一帧数据

		//帧设备ID数据解析
		if ((comend_Buff[0] !=device_id.year_datah)||(comend_Buff[1] !=device_id.year_datal)||
		(comend_Buff[2] !=device_id.sn_datah)||(comend_Buff[3] !=device_id.sn_datal))
		//if (comend_Buff[0] != device_address)	//如果不是本机设备地址码，则丢弃本帧数据
		{
			Comeback_err((u8 *)comend_Buff, comend_Len,GPRS_ERROR_ID);
			return ;
		}
    //帧数据长度解析
		if (comend_Buff[6] != (comend_Len-7))	//如果数据长度不匹配，则丢弃本帧数据
		{
			Comeback_err((u8 *)comend_Buff, comend_Len,GPRS_ERROR_LENGTH);
			return ;
		}
		//	if ((!online)&&(comend_Buff[7]!=0x0E))	//如果不在线，则丢弃本帧数据
		//{
		//	return ;
		//}
			if ((!enFlag)&&(comend_Buff[7]!=0x0E)&&(comend_Buff[7]!=0x10))	//如果禁用，则丢弃本帧数据
		{
		   Comeback_err((u8 *)comend_Buff, comend_Len,GPRS_ERROR_Disable);
			return ;
		}
		//CRC校验帧数据	
		temp = CRC16((u8 *)comend_Buff, (comend_Len-2));
		crc_datah = (u8)((temp>>8)&0x00ff);
		crc_datal = (u8)(temp&0x00ff);
		
		if ((comend_Buff[comend_Len-2] != crc_datah)||(comend_Buff[comend_Len-1] != crc_datal))
		{	//CRC检验错误，则返回错误信息
			Comeback_err((u8 *)comend_Buff, comend_Len,GPRS_ERROR_CRC);
		return ;	
		}
	 
		//命令字检查，若命令字错误，则返回错误信息
		if ((comend_Buff[7]<0x01)||(comend_Buff[7]>0x20))
		{
			Comeback_err((u8 *)comend_Buff, comend_Len,GPRS_ERROR_CMD);
			return ;	
		}	 
		switch(comend_Buff[4]){
				case 0x03:		//读命令字
				switch(comend_Buff[7])
				{
					case 0x01:		//读设备ID
					  ID_Read_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x02:		//读设备序列号
						DeviceHardware_Read_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x03:		//读变送器配置
					  Transset_Read_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x05:		//读报警门限	
					  Alarm_Read_back((u8 *)comend_Buff, comend_Len);
						break;
					
					case 0x07:		//读盐度补偿
					  Salt_Read_back((u8 *)comend_Buff, comend_Len);
						break;
						 
					case 0x0D:		//读系统设置
					  Systemset_Read_back((u8 *)comend_Buff, comend_Len);
						break;

		 			case 0x0E:		//建立连接
						Handshake_Read_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x10:		//读设备使能
					  DeviceCon_Read_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x11:		//读取最大挂载数量
						SetmaxSecondID_Read_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x13:		//读变送器数据
					  Transdata_Read_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x15:		//读取实际挂载数量
						ExistSecondID_Read_back((u8 *)comend_Buff, comend_Len);
						break;
					
					case 0x19:		//读变送器报警使能
						TransAlarm_Read_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x1E:		//读变送器水泵
						Transpump_Read_back((u8 *)comend_Buff, comend_Len);
						break;
					case 0x04:		//读变送器控制门限
					  Transcon_Read_back((u8 *)comend_Buff, comend_Len);
						break;

					//case 0x05:		//读继电器固定开启（无门限）
					//  Relayopen_Read_back((u8 *)comend_Buff, comend_Len);
					//	break;

				//	case 0x06:		//读变送器固定时间校正
					//  Transadjust_Read_back((u8 *)comend_Buff, comend_Len);
				//		break;

					
					case 0x12:		//读继电器检测数据
					  Relaydata_Read_back((u8 *)comend_Buff, comend_Len);
					  break;
					
					//case 0x5A:		//读全部数据
					//	AllData_Read_back((u8 *)comend_Buff, comend_Len);
					//	break;
					
					
					case 0x06:		//读继电器固定开启（）
					  Relayopengate_Read_back((u8 *)comend_Buff, comend_Len);
						break;
					
					case 0x0C:		//读继电器终端状态
					  Relaystate_Read_back((u8 *)comend_Buff, comend_Len);
						break;
					
					case 0x16:		//二级设备版本
						Secondver_Read_back((u8 *)comend_Buff, comend_Len);
						break;
					
					case 0x0f:		//读智能控制
					  Intelligence_Read_back((u8 *)comend_Buff, comend_Len);
						break;
					
					case 0x1a:		//读继电器报警使能
						RelayAlarm_Read_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x1b:		//读继电器配置
					  Relayset_Read_back((u8 *)comend_Buff, comend_Len);
						break;
					
					case 0x1c:		//读继电器固定时间任务控制使能
						RelayFixEnable_Read_back((u8 *)comend_Buff, comend_Len);
						break;
			//	case 0x14:		//读系统使能
					//	SysEnable_Read_back((u8 *)comend_Buff, comend_Len);
					//	break;
					default: 
						break;
					}
						break;
						//写命令字
				  case 0x16:		//写命令字
				switch(comend_Buff[7]){

					case 0x01:		//写设备ID
						ID_Write_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x02:		//写设备序列号
						DeviceHardware_Write_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x03:		//写变送器配置
						Transset_Write_back((u8 *)comend_Buff, comend_Len);
						break;
					
					case 0x05:		//写变送器报警门限
						Alarm_Write_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x07:        //写盐度补偿数据
				        Salt_Write_back((u8 *)comend_Buff, comend_Len);
					    break;

					case 0x08:		//设置变送器校准
						Transzhun_Write_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x0a:		//设置系统时间
						Systime_Write_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x0D:		//写系统设置
						Systemset_Write_back((u8 *)comend_Buff, comend_Len);
						break; 
						 
					case 0x10:		//设备使能控制
					  DeviceCon_Write_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x11:		//设置最大挂载数量
						SetmaxSecondID_Write_back((u8 *)comend_Buff, comend_Len);
						break;
								
					case 0x14:		//恢复出厂设置
						Restoresetting_Write_back((u8 *)comend_Buff, comend_Len);
						break;
					
					case 0x15:		//设置挂载数量
						ExistSecondID_Write_back((u8 *)comend_Buff, comend_Len);
						break;
					
					case 0x19:		//写变送器报警使能
						TransAlarm_Write_back((u8 *)comend_Buff, comend_Len);
						break;
					
					case 0x1D:		//写设备名称
						DeviceName_Write_back((u8 *)comend_Buff, comend_Len);
						break;
						
					case 0x1E:		//写变送器水泵控制
						Transpump_Write_back((u8 *)comend_Buff, comend_Len);
						break;

				   	case 0x1F:		//写通道名称
						PathwayName_Write_back((u8 *)comend_Buff, comend_Len);
						break;
					 case 0x04:		//写变送器控制门限
						Transcon_Write_back((u8 *)comend_Buff, comend_Len);
						break;
				//	case 0x06:		//写继电器固定开启（无门限）
				//		Relayopen_Write_back((u8 *)comend_Buff, comend_Len);
				//		break;
				//	case 0x08:		//写变送器校正时间
					//	Transadjust_Write_back((u8 *)comend_Buff, comend_Len);
					//	break;
					case 0x0B:		//写继电器手动控制
						 Relaycon_Write_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x06:		//写继电器固定开启（）
					  Relayopengate_Write_back((u8 *)comend_Buff, comend_Len);
						break;
					
				//	case 0x10:		//重启控制
					//	Reset_Write_back((u8 *)comend_Buff, comend_Len);
					//	break;
					
					case 0x0f:		//智能控制开关
						Intelligence_Write_back((u8 *)comend_Buff, comend_Len);
						break;
					
					case 0x1B:		//写继电器配置
					  Relayset_Write_back((u8 *)comend_Buff, comend_Len);
						break;
				//	case 0x12:		//二级ID设置
				//		SecondID_Write_back((u8 *)comend_Buff, comend_Len);
				//		break;
							
				//	case 0x13:		//设置系统使能
				//		SysEnable_Write_back((u8 *)comend_Buff, comend_Len);
				//		break;
				
					case 0x1A:		//写继电器报警使能
						RelayAlarm_Write_back((u8 *)comend_Buff, comend_Len);
						break;
					case 0x1c:		//写继电器固定时间任务控制使能
						RelayFixEnable_Write_back((u8 *)comend_Buff, comend_Len);
						break;
						
					default: 
						break;
				}
				 break;

				default: 
						break;
			   }
		}
	}
		/**********************************************************
 *
 * @file	: Intelligence_Read_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 读继电器固定时间任务控制使能的返回信息
 *
 **********************************************************/
 void RelayFixEnable_Read_back(u8 *buffer, u8 Len)
{
	u8 i,k,n= 0;
	
	u16 temp;
	//u8 datatem;//变送器数量
	uart4_buff.tx_len =20;
//	datatem=second_device.trans_data;
	for (i=0;i<11;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//正常
	n=buffer[11]-1;
		              for (k=0;k<6;k++){
		                uart4_buff.tx_data_buff[i++] = read_random(0,0,(366*n+102+k));//相应配置对应变送器智能控制   读取存储的相应配置信息
	                  delay_ms(3);
		                               }
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRC校验低位
	
USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}
/**********************************************************
 *
 * @file	: Systemset_Write_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 写继电器固定时间任务控制使能及返回信息
 *
 **********************************************************/
void RelayFixEnable_Write_back(u8 *buffer, u8 Len)
{
u8 n= 0;
u8 x= 0;	
//	u16 temp;
//	u8 datatrans;//变送器数量
//	u16 datatem;
	//alaDataflag=0;  
	n=buffer[11]-1;
	relaygateENflag[n]=0;
		for(x=0;x<6;x++) { 
	  write_byte(0,0,366*n+102+x,buffer[12+x]);//相应配置对应变送器配置类型   读取存储的相应信息
	  delay_ms(3);
                    }
	Write_back( buffer, Len );
} 
		/**********************************************************
 *
 * @file	: Relayset_Read_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 读继电器配置的返回信息
 *
 **********************************************************/
 void Relayset_Read_back(u8 *buffer, u8 Len)
{
	u8 i,k,n= 0;
	u8 j= 0;
	u16 temp;
	//u16 datatem;
//	u8 datatem;//变送器数量
	//uart4_buff.tx_len =15;

	for (i=0;i<11;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//正常
	if(buffer[11]>second_device.relay_data){
	Comeback_err((u8 *)buffer, Len,RELAY_NOT_EXIST);}
	else {
		if(buffer[11]){
			uart4_buff.tx_data_buff[i++] = buffer[11];
	for (j=0;j<buffer[11];j++){
	 n=buffer[12+j]-1;	   //ID
		uart4_buff.tx_data_buff[i++] = buffer[12+j];
    for (k=0;k<3;k++){
	  uart4_buff.tx_data_buff[i++] = read_random(0,0,(366*n+k+67));//相应配置对应变送器配置类型   读取存储的相应信息
	  delay_ms(3);	    
	                   }
		 			                     }
	uart4_buff.tx_len =4*buffer[11]+14;
	uart4_buff.tx_data_buff[6]=4*buffer[11]+7;
                  }
	else { uart4_buff.tx_data_buff[i++] = second_device.relay_data;
	for (j=0;j<second_device.relay_data;j++){
		uart4_buff.tx_data_buff[i++] = j+1;
    for (k=0;k<3;k++){
	  uart4_buff.tx_data_buff[i++] = read_random(0,0,(366*n+k+67));//相应配置对应变送器配置类型   读取存储的相应信息
	  delay_ms(3);	    
	                     }
	                                        }
	   uart4_buff.tx_len =4*second_device.relay_data+14;
	   uart4_buff.tx_data_buff[6]=4*second_device.relay_data+7;
	      }
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRC校验低位
	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);}
}
	/**********************************************************
 *
 * @file	: Intelligence_Read_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 读继电器终端报警使能的返回信息
 *
 **********************************************************/
 void RelayAlarm_Read_back(u8 *buffer, u8 Len)
{
	u8 i,k,n= 0;
	
	u16 temp;
//	u8 datatem;//变送器数量
	uart4_buff.tx_len =20;
//	datatem=second_device.trans_data;
	for (i=0;i<11;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//正常
	n=buffer[11]-1;
	if(buffer[11]>second_device.relay_data){
	Comeback_err((u8 *)buffer, Len,TRANS_NOT_EXIST);}
	else {
		              for (k=0;k<6;k++){
		                uart4_buff.tx_data_buff[i++] = read_random(0,0,(366*n+61+k));//相应配置对应变送器智能控制   读取存储的相应配置信息
	                  delay_ms(3);
		                  }
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRC校验低位
	
	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}
	}
/**********************************************************
 *
 * @file	: Systemset_Write_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 写继电器终端报警使能及返回信息
 *
 **********************************************************/
void RelayAlarm_Write_back(u8 *buffer, u8 Len)
{
u8 n= 0;
u8 x= 0;	
	//alaDataflag=0;  
	n=buffer[11]-1;
	relayalDataflag[n]=0;
		for(x=0;x<6;x++) { 
	  write_byte(0,0,366*n+61+x,buffer[12+x]);//相应配置对应变送器配置类型   读取存储的相应信息
	  delay_ms(3);
                    }
	Write_back( buffer, Len );
} 

/**********************************************************
 *
 * @file	: TransAlarm_Read_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 读变送器终端报警使能的返回信息
 *
 **********************************************************/
 void TransAlarm_Read_back(u8 *buffer, u8 Len)
{
	u8 i,k,n= 0;
	
	u16 temp;
//	u8 datatem;//变送器数量
	uart4_buff.tx_len =20;
//	datatem=second_device.trans_data;
	for (i=0;i<12;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//正常
	uart4_buff.tx_data_buff[6] = 0x0D;		//正常
	if(buffer[11]>second_device.trans_data){
	Comeback_err((u8 *)buffer, Len,TRANS_NOT_EXIST);}
	else{
	n=buffer[11]-1;
		              for (k=0;k<6;k++){
		                uart4_buff.tx_data_buff[i++] = read_random(0,1,(708*n+690+k));//相应配置对应变送器智能控制   读取存储的相应配置信息
	                  delay_ms(3);
		                  }
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRC校验低位
	
	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}
   }
   /**********************************************************
 *
 * @file	: Transpump_Read_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 读变送器水泵的返回信息
 *
 **********************************************************/
 void Transpump_Read_back(u8 *buffer, u8 Len)
{
	u8 i,k,n= 0;
	
	u16 temp;
//	u8 datatem;//变送器数量
	//uart4_buff.tx_len =26;
//	datatem=second_device.trans_data;
	for (i=0;i<12;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//正常
	if(buffer[11]>second_device.trans_data){
	Comeback_err((u8 *)buffer, Len,TRANS_NOT_EXIST);}
	else{
	n=buffer[11]-1;
	uart4_buff.tx_data_buff[i++] =transpump_En[n].datah;
  uart4_buff.tx_data_buff[i++] =transpump_En[n].datal;
	if(transpump_En[n].data){
		           uart4_buff.tx_len =26;
		           uart4_buff.tx_data_buff[6] =19;
		              for (k=0;k<5;k++){
		                //uart4_buff.tx_data_buff[i++] = read_random(0,1,(708*n+698+k));//相应配置对应变送器智能控制   读取存储的相应配置信息
	                  uart4_buff.tx_data_buff[i++] =transpump_En[n].dataflagh[k];
										uart4_buff.tx_data_buff[i++] =transpump_En[n].dataflagl[k];
										//delay_ms(3);
									                  }
		                      }
	else {uart4_buff.tx_len =16;
        uart4_buff.tx_data_buff[6] =9;	}						
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRC校验低位
	
	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}
   }
/**********************************************************
 *
 * @file	: Systemset_Write_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 写变送器终端报警使能及返回信息
 *
 **********************************************************/
void TransAlarm_Write_back(u8 *buffer, u8 Len)
{
u8 n= 0;
u8 x= 0;	
	//u16 temp;
	//u8 datatrans;//变送器数量
	//u16 datatem;
	//alaDataflag=0;
	n=buffer[11]-1;
	transalarmENflag[n]=0;
		for(x=0;x<6;x++) { 
	  write_byte(0,1,708*n+691+x,buffer[12+x]);//相应配置对应变送器配置类型   读取存储的相应信息
	  delay_ms(3);
                    }
	Write_back( buffer, Len );
} 

/**********************************************************
 *
 * @file	: DeviceName_Write_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 写设备名称及返回信息
 *
 **********************************************************/
void DeviceName_Write_back(u8 *buffer, u8 Len)
{
u8 k,m,n;
//u8 y,x;
//u8 i,j=0;
u8 t=0;	
//	u16 temp;
	u8 datatrans;//变送器数量
	u8 datarelay;
//	u8 sdata[2];
//	u16 datatem;
	datatrans=second_device.trans_data;
	datarelay=second_device.relay_data;
	//setDataflag=0;
  //	n=buffer[10]-1;	   //ID
	//datatem=buffer[11]*256+buffer[12];

	switch(buffer[11]){
					case 0x52:		//删除变送器配置
						switch(buffer[13]){
						case 0x11:
						    for (k=0;k<20;k++){
								write_byte(0,0,32000+90+k,0x00);
	                            delay_ms(3);         //擦写存储变送器的地址
		                               }
						    break;
						case 0x21:
						if(!buffer[12]){
						   for (m=0;m<datatrans;m++)
	                               {
	              	        for (k=0;k<10;k++){
								write_byte(0,0,32000+10*m+110+k,0x00);
	                            delay_ms(3);         //擦写存储变送器的地址
		                                       }
	                               }
									   }
						else{
						 for (m=0;m<buffer[12];m++)
	                    {
		                   for (n=1;n<(datatrans+1);n++)      {
			                 if(buffer[14+m]==n){
		                       for (k=0;k<10;k++){
		                       //擦写存储变送器的地址
													 write_byte(0,0,32000+10*m+110+k,0x00);
													 delay_ms(3);
		                                        }   
			                          break;     
                     						          }
										                            }
	                     }
						    }
						    break;
						case 0x31:
						if(!buffer[12]){
						    for (m=0;m<datarelay;m++)
	                               {
	              	        for (k=0;k<10;k++){
								write_byte(0,0,32000+10*m+1390+k,0x00);
	                            delay_ms(3);         //擦写存储变送器的地址
		                                      }
	                               }
								       }
						else {
							 for (m=0;m<buffer[12];m++)
	                    {
		                   for (n=1;n<(datarelay+1);n++)      {
			                 if(buffer[14+m]==n){
		                       for (k=0;k<10;k++){
		                       //擦写存储变送器的地址
													 write_byte(0,0,32000+10*m+1390+k,0x00);
													 delay_ms(3);
		                                        }   
			                          break;     
                     						          }
										                            }
	                     }
						     }
						    break;
						
						   default: 
						   break;
					                      }
						break;
					
					case 0x51:		//添加修改变送器配置
						  switch(buffer[13]){
						case 0x11:
						    for (k=0;k<buffer[14]+1;k++){
								write_byte(0,0,90+k,buffer[14+k]);
	                            delay_ms(3);         //擦写存储变送器的地址
		                               }
						    break;
						case 0x21:
						 for (m=0;m<buffer[12];m++)
	                    {
		                   for (n=1;n<(datatrans+1);n++)      {
			                 if(buffer[14+t]==n){
		                       for (k=0;k<buffer[15+t]+1;k++){
		                       //擦写存储变送器的地址
													 write_byte(0,0,10*buffer[14+t]+110+k,buffer[15+t+k]);
													 delay_ms(3);
		                                        }
									  t=buffer[15+t]+2;			   
			                          break;     
                     						          }
										                            }
	                     }
						    break;
						case 0x31:
							 for (m=0;m<buffer[12];m++)
	                    {
		                   for (n=1;n<(datarelay+1);n++)      {
			                 if(buffer[14+t]==n){
		                       for (k=0;k<buffer[15+t]+1;k++){
		                       //擦写存储变送器的地址
													 write_byte(0,0,10*buffer[14+t]+1390+k,buffer[15+t+k]);
													 delay_ms(3);
		                                        }
									  t=buffer[15+t]+2;			  
			                          break;     
                     						          }
										                            }
	                     }
						    break;
						
						   default: 
						   break;
					                      }
						break;
					default: 
						break;
	}
    Write_back( buffer, Len );
}
 /**********************************************************
 *
 * @file	: PathwayName_Write_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 写通道名称及返回信息
 *
 **********************************************************/
void PathwayName_Write_back(u8 *buffer, u8 Len)
{
u8 k,m,n;
u8 x;
u8 i,j=0;
u8 t=0;	
//	u16 temp;
	u8 datatrans;//变送器数量
	u8 sdata[2];
	u16 datatem;
	datatrans=second_device.relay_data;
	//setDataflag=0;
  //	n=buffer[10]-1;	   //ID
	//datatem=buffer[11]*256+buffer[12];

	switch(buffer[11]){
					case 0x52:		//删除变送器配置
					    if(!buffer[12]){
						for (m=0;m<datatrans;m++)
	                               {
	              	for (k=1;k<3;k++){
								write_byte(0,1,708*m+k,0x00);
	                            delay_ms(3);         //擦写存储变送器的地址
		                               }
	                               }
						               }

						else {
						for (m=0;m<buffer[12];m++)
	                    {
		             for (n=1;n<(datatrans+1);n++)      {
			                 if(buffer[13+m]==n){
		                       for (k=1;k<3;k++){
		                       //擦写存储变送器的地址
													 write_byte(0,1,708*m+k,0x00);
													 delay_ms(3);
		                                        }   
			                          break;     
                     						          }
										                            }
	                     }
						}									
						break;
					
					case 0x50:		//添加修改变送器配置
						  for (m=0;m<buffer[12];m++)
	                    {           //判断配置了几种传感器探头
		                   for (n=1;n<81;n++)      {
			                      if(buffer[13+i]==n){         //ID匹配
										for (k=0;k<2;k++){
										     sdata[k]=read_random(0,1,708*(n-1)+k);
											 delay_ms(3);
											 sdata[k] |=buffer[14+i+k];
											             }
										for (k=0;k<2;k++){				  //写终端配置
												write_byte(0,1,k+701*(n-1)+1,sdata[k]);
												delay_ms(3);
		                                                   }
													datatem=buffer[14+i]*256+buffer[15+i];				  
												    for(x=0;x<4;x++) { 
                                       if((datatem&0x8000)==0x8000){           //判断传感器探头类型  写入相应类型的数据
																		 for (k=0;k<2;k++){
																			 write_byte(1,0,k+701*(n-1)+3+2*x,buffer[16+i+2*t+k]);
																			 delay_ms(3);
		                                                            }
																	t++;           
																}
                                       datatem=datatem<<1; 
                                              }    
			                               break;     
                     						                  }
										                          }
						     datatem=buffer[14+i]*256+buffer[15+i];//第一个变送器配置状态
							for(x=0;x<4;x++) { 
                            if((datatem&0x8000)==0x8000){j++;} 
                            datatem=datatem<<1; 
                                         } 
										        i+=2*j+3;                   //判断配置了几种传感器探头

	                     } 				
						break;
					
					case 0x51:		//覆盖变送器配置
						   for (m=0;m<buffer[12];m++)
	                    {           //判断配置了几种传感器探头
		                   for (n=1;n<datatrans+1;n++)      {
			                      if(buffer[13+i]==n){         //ID匹配
										for (k=0;k<2;k++){				  //写终端配置
												write_byte(0,1,k+701*(n-1)+1,buffer[14+i+k]);
												delay_ms(3);
		                                                   }
													datatem=buffer[14+i]*256+buffer[15+i];				  
												    for(x=0;x<4;x++) { 
                                       if((datatem&0x8000)==0x8000){           //判断传感器探头类型  写入相应类型的数据
																		 for (k=0;k<2;k++){
																			 write_byte(1,0,k+701*(n-1)+3+2*x,buffer[16+i+2*t+k]);
																			 delay_ms(3);
		                                                            }
																	t++;           
																}
                                       datatem=datatem<<1; 
                                              }    
			                               break;     
                     						                  }
										                          }
						     datatem=buffer[14+i]*256+buffer[15+i];//第一个变送器配置状态
							for(x=0;x<4;x++) { 
                            if((datatem&0x8000)==0x8000){j++;} 
                            datatem=datatem<<1; 
                                         } 
										        i+=2*j+3;                   //判断配置了几种传感器探头

	                     } 				
						break;
					
					default: 
						break;
	}	     
    Write_back( buffer, Len );
}
/**********************************************************
 *
 * @file	: Transpump_Write_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 写变送器水泵控制及返回信息
 *
 **********************************************************/
void Transpump_Write_back(u8 *buffer, u8 Len)
{
u8 n= 0;
u8 x= 0;
	u16 temp;
	n=buffer[11]-1;
	transpumpENflag[n]=0;
		for(x=0;x<4;x++) { 
	  write_byte(0,1,708*n+696+x,buffer[12+x]);//相应配置对应变送器配置类型   读取存储的相应信息
	  delay_ms(3);
                    }
		temp=buffer[14]*256+buffer[15];
	//pump_write(buffer[11],buffer[12],buffer[13],buffer[14],buffer[15]);
	if((temp&0x4000)==0x4000){
	       PBout(6)=0;                  //开水泵
	                          }
	else PBout(6)=1; 
	Write_back( buffer, Len );
} 
/**********************************************************
 *
 * @file	: Intelligence_Read_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 读系统控制使能的返回信息
 *
 **********************************************************/
 void SysEnable_Read_back(u8 *buffer, u8 Len)
{
	u8 i,k= 0;
	
	u16 temp;
//	u8 datatem;//变送器数量
	uart4_buff.tx_len =40;
//	datatem=second_device.trans_data;
	for (i=0;i<10;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//正常
		              for (k=0;k<28;k++){
		                uart4_buff.tx_data_buff[i++] = read_random(0,0,(36+k));//相应配置对应变送器智能控制   读取存储的相应配置信息
	                  delay_ms(3);
		                  }
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRC校验低位
	
	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}
/**********************************************************
 *
 * @file	: Systemset_Write_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 写系统控制使能及返回信息
 *
 **********************************************************/
void SysEnable_Write_back(u8 *buffer, u8 Len)
{
u8 x= 0;	
//	u16 temp;
//	u8 datatrans;//变送器数量
//	u16 datatem;
	//alaDataflag=0;
		for(x=0;x<28;x++) { 
	  write_byte(0,0,36+x,buffer[10+x]);//相应配置对应变送器配置类型   读取存储的相应信息
	  delay_ms(3);
                    }
	Write_back( buffer, Len );
} 
/**********************************************************
 *
 * @file	: Intelligence_Read_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 读变送器智能控制的返回信息
 *
 **********************************************************/
void Intelligence_Read_back(u8 *buffer, u8 Len)
{
	u8 i,k= 0;
	
	u16 temp;
	u8 datatem;//变送器数量
	uart4_buff.tx_len =16;
	datatem=second_device.trans_data;
	for (i=0;i<12;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//正常
	datatem=buffer[11]-1;	
		              for (k=0;k<2;k++){
		                uart4_buff.tx_data_buff[i++] = read_random(0,1,(708*datatem+590+k));//相应配置对应变送器智能控制   读取存储的相应配置信息
	                  delay_ms(3);
		                  }
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRC校验低位
	
	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}


/**********************************************************
 *
 * @file	: SetmaxSecondID_Read_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 读最大挂载数量及返回信息
 *
 **********************************************************/
void SetmaxSecondID_Read_back(u8 *buffer, u8 Len)
{
    u8 i,x;
	u8 y=0;
	u16 temp;	
	u16 datatem;
	
	for (i=0;i<13;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	datatem=buffer[11]*256+buffer[12];
	for(x=0;x<16;x++) { 
         if((datatem&0x8000)==0x8000){
	  uart4_buff.tx_data_buff[i++] = read_random(0,0,0xC000+66+x);//相应配置对应变送器配置类型   读取存储的相应信息
	  delay_ms(3);	    
				 y++;                    }
          datatem=datatem<<1;
                    }
	uart4_buff.tx_data_buff[8] = 0xd0;		//正常
	uart4_buff.tx_len = 15+y;	
  uart4_buff.tx_data_buff[6] = 8+y;		//正常										
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff); 		//CRC校验低位
	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}
/**********************************************************
 *
 * @file	: ExistSecondID_Read_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 读实际挂载数量及返回信息
 *
 **********************************************************/
void ExistSecondID_Read_back(u8 *buffer, u8 Len)
{
    u8 i,x= 0;
	u16 temp;	
	uart4_buff.tx_len = 15;
	
	for (i=0;i<11;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//正常
		for(x=0;x<2;x++) { 
						uart4_buff.tx_data_buff[i++] = read_random(0,0,0xC000+x+82);	//读取相应的挂载参数							
					       delay_ms(3);
                      }							
	temp = CRC16( (u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff); 		//CRC校验低位
	
	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}
/**********************************************************
 *
 * @file	: CRC16()
 * @param	: u8 *buffer: 指向要进行CRC16校验数据的指针
 *			  u8 usLen: 要进行CRC16校验的数据长度
 * @retval	: CRC16校验的结果
 * @brief	: 对usLen个长度的数据进行CRC16校验
 *
 **********************************************************/
u16 CRC16(u8 *buffer, u8 usLen)
{
	u8 i = 0;
	u8 ucCRCHi = 0xFF;
	u8 ucCRCLo = 0xFF;
	u8 usInx = 0x00;

	while (usLen-- > 0)
	{
	//	usInx   = ucCRCLo ^ buffer[i++];
	//	ucCRCLo = ucCRCHi ^ aucCRCHi[usInx];
	//	ucCRCHi = aucCRCLo[usInx];
	usInx = ucCRCHi ^ buffer[i++]; /* 计算CRC */
    ucCRCHi = ucCRCLo ^ aucCRCHi[usInx] ;
    ucCRCLo = aucCRCLo[usInx] ;
	}

	return (u16)(ucCRCHi << 8 | ucCRCLo);
}

/**********************************************************
 *
 * @file	: Comeback_err()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 帧数据错误，返回错误信息
 *
 **********************************************************/
void Comeback_err(u8 *buffer, u8 Len,u8 comend_ack)
{
	u8 i = 0;
	u16 temp;
	uart4_buff.tx_len = 0x0d;
	for (i=0;i<11;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[4] = buffer[4] + 0x80;			//功能码
	uart4_buff.tx_data_buff[6] = 5;		//长度
	uart4_buff.tx_data_buff[8] = comend_ack;					//预留1
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRC校验低位
    USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}

/**********************************************************
 *
 * @file	: Relaydata_Read_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 读继电器检测状态数据的返回信息
 *
 **********************************************************/
void Relaydata_Read_back(u8 *buffer, u8 Len)
{
		u8 i,m,n= 0;
	u8 x,j,y= 0;
	u16 temp;
	u16 datatem;//变送器数量
	u8 relays=0;
	u8 datatrans;//变送器数量
	u8 temdata1,temdata2;
	datatrans=second_device.relay_data;
	for (i=0;i<11;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//正常  
	temp=0;
	if(buffer[11]){
	    uart4_buff.tx_data_buff[i++] =buffer[11];
		temp++;
		for (y=0;y<buffer[11];y++)
	{
	    n=buffer[12+y]-1;
		
		uart4_buff.tx_data_buff[i++] =buffer[12+y];
		temp++;
		uart4_buff.tx_data_buff[i++] = relay_result_data[n].state;   //继电器终端状态
		temp++;
		if(relay_result_data[n].state!=0xee){
		for(j=0;j<2;j++){
		uart4_buff.tx_data_buff[i++] = read_random(0,0,(366*n+j+68));   //继电器检测配置
		temp++;
		delay_ms(3);}
	    datatem=uart4_buff.tx_data_buff[i-2]*256+uart4_buff.tx_data_buff[i-1];   //读取变送器配置的地址数据
		//datatim=trans_query_data[n].flagdata;
		for(x=0;x<16;x++) { 
         if((datatem&0x8000)==0x8000){//判断配置了几种检测功能
		       uart4_buff.tx_data_buff[i++] = relay_result_data[n].data1[x];temp++;
               uart4_buff.tx_data_buff[i++] = relay_result_data[n].data2[x];temp++;
											   }
						  datatem=datatem<<1;
						  }
                                        }
			}
         }
   else{
   uart4_buff.tx_data_buff[i++] = datatrans;
   temp++;
   for(m=0;m<datatrans;m++){
		 if(relay_result_data[m].state!=0xee){
			 relays++;
        uart4_buff.tx_data_buff[i++] = m+1;
		temp++;
		uart4_buff.tx_data_buff[i++] = relay_result_data[m].state;   //继电器终端状态
		temp++;
		temdata1=relay_result_data[n].setData1;
		
	    temdata2=relay_result_data[n].setData2;   //读取继电器配置的地址数据

		 uart4_buff.tx_data_buff[i++] = temdata1;
		 temp++;
		 uart4_buff.tx_data_buff[i++] = temdata2;
		 temp++;
		datatem=temdata1*256+temdata2;
		for(x=0;x<16;x++) { 
         if((datatem&0x8000)==0x8000){//判断配置了几种检测功能
		       uart4_buff.tx_data_buff[i++] = relay_result_data[n].data1[x];temp++;
               uart4_buff.tx_data_buff[i++] = relay_result_data[n].data2[x];temp++;
											   }
						  datatem=datatem<<1; 
						  }
						                             }
                       }
	 uart4_buff.tx_data_buff[11] = relays;
        }
   	uart4_buff.tx_data_buff[6]=temp+6;
	uart4_buff.tx_len =temp+13;
	 
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRC校验低位

USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
 
	for(n=0;n<datatrans;n++){
		if(relay_result_data[n].state!=0xee){		
	datatem=relay_result_data[n].setData1*256+relay_result_data[n].setData2;		  //存储传感数据
			write_byte(0,0,(366*n),relay_result_data[n].state);        
	      delay_ms(3);
	   for(x=0;x<16;x++) { 
         if((datatem&0x8000)==0x8000){//判断配置了几种检测功能
		       	write_byte(0,0,(366*n+70+2*x),relay_result_data[n].data1[x]);        
	          delay_ms(3);
	          write_byte(0,0,(366*n+71+2*x),relay_result_data[n].data2[x]);        
	          delay_ms(3);
											               }
						  datatem=datatem<<1; 
						           }                     }
						               }
 for (i=0;i<12;i++){
	                   	uart4_buff.tx_data_buff[i] = buffer[i];
                                         }
						uart4_buff.tx_data_buff[9] = 0x60;
						uart4_buff.tx_data_buff[11] =0;
 for(n=0;n<datatrans;n++){
        for(x=0;x<4;x++) {
            if(!(trans_query_data[n].decimal[x]|trans_query_data[n].flag[x])){
				 Alarmdata[n]++;
				 if(Alarmdata[n]==2){
				 	  Alarmdata[n]=0;
					  uart4_buff.tx_data_buff[11] +=1;
					  uart4_buff.tx_data_buff[i++] =n;
					  for(j=0;j<3;j++){
		uart4_buff.tx_data_buff[i++] = read_random(0,1,(708*m+j));   //变送器
		temp++;
		delay_ms(3);}
		temdata1=read_random(0,1,(708*m+1));
		delay_ms(3);
	    temdata2=read_random(0,1,(708*m+2));   //读取变送器配置的地址数据
		delay_ms(3);
		datatem=temdata1*256+temdata2;
		uart4_buff.tx_data_buff[i++] = trans_query_data[m].flagdata/256;  //变送器探头状态位
		temp++;
		uart4_buff.tx_data_buff[i++] = trans_query_data[m].flagdata%256;  
		temp++;
	//	datatim=trans_query_data[m].flagdata;
		for(x=0;x<4;x++) { 
         if((datatem&0x8000)==0x8000){//判断配置了几种传感器探头
		       uart4_buff.tx_data_buff[i++] = trans_query_data[m].integerh[x];temp++;
               uart4_buff.tx_data_buff[i++] = trans_query_data[m].integerl[x];temp++;
			   uart4_buff.tx_data_buff[i++] = trans_query_data[m].mvdatah[x];temp++;
			   uart4_buff.tx_data_buff[i++] = trans_query_data[m].mvdatal[x];temp++; 
               uart4_buff.tx_data_buff[i++] = trans_query_data[m].decimal[x];temp++;
			   uart4_buff.tx_data_buff[i++] = trans_query_data[m].flag[x];temp++;
											   }
						  datatem=datatem<<1; 
						  }
  

				                     }
				 															  }
				  Alarmdata[n]=0;
				         }
			    }
}



/**********************************************************
 *
 * @file	: Relaystate_Read_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 读继电器终端状态的返回信息
 *
 **********************************************************/
void Relaystate_Read_back(u8 *buffer, u8 Len)
{
		u8 i,m,n= 0;
	u8 x,j,y= 0;
	u16 temp;
	u16 datatem;//变送器数量
	u8 relays=0;
	u8 datatrans;//变送器数量
	u8 temdata1,temdata2;
	datatrans=second_device.relay_data;
	for (i=0;i<11;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//正常  
	temp=0;
	if(buffer[11]){
	    uart4_buff.tx_data_buff[i++] =buffer[11];
		temp++;
		for (y=0;y<buffer[11];y++)
	{
	    n=buffer[12+y]-1;
		uart4_buff.tx_data_buff[i++] =buffer[12+y];
		temp++;
		uart4_buff.tx_data_buff[i++] = relay_result_data[n].state;   //变送器
		temp++;
		if(relay_result_data[n].state!=0xee){
		for(x=0;x<5;x++) { 
		       uart4_buff.tx_data_buff[i++] = relay_date[n].data1[x];temp++;
           uart4_buff.tx_data_buff[i++] = relay_date[n].data2[x];temp++;
						  }
        }                             }
         }
   else{
   uart4_buff.tx_data_buff[i++] = datatrans;
   temp++;
   for(m=0;m<datatrans;m++){
		 if(relay_result_data[m].state!=0xee){
			 relays++;
        uart4_buff.tx_data_buff[i++] = m+1;
		temp++;
		uart4_buff.tx_data_buff[i++] = relay_result_data[m].state;   //变送器
		temp++;
		delay_ms(3);
		for(x=0;x<5;x++) { 
		       uart4_buff.tx_data_buff[i++] =relay_date[n].data1[x];temp++;
               uart4_buff.tx_data_buff[i++] = relay_date[n].data2[x];temp++;
											   }
   }                                      }
	 uart4_buff.tx_data_buff[11] = relays;
   }
  uart4_buff.tx_data_buff[6]=temp+6;
	uart4_buff.tx_len =temp+13;
	 
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRC校验低位

USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);

	  //存储数据
	for(n=0;n<datatrans;n++){
		if(relay_result_data[n].state!=0xee){
	   for(x=0;x<5;x++) { 
		       	write_byte(0,0,(366*n+1+2*x),relay_date[n].data1[x]);        
	          delay_ms(5);
	          write_byte(0,0,(366*n+2+2*x),relay_date[n].data2[x]);        
	          delay_ms(5);
						           }                 }
						               }             
 for (i=0;i<12;i++){
	                   	uart4_buff.tx_data_buff[i] = buffer[i];
                                         }
						uart4_buff.tx_data_buff[9] = 0x60;
						uart4_buff.tx_data_buff[11] =0;
 for(n=0;n<datatrans;n++){
        for(x=0;x<4;x++) {
            if(!(trans_query_data[n].decimal[x]|trans_query_data[n].flag[x])){
				 Alarmdata[n]++;
				 if(Alarmdata[n]==2){
				 	  Alarmdata[n]=0;
					  uart4_buff.tx_data_buff[11] +=1;
					  uart4_buff.tx_data_buff[i++] =n;
					  for(j=0;j<3;j++){
		uart4_buff.tx_data_buff[i++] = read_random(0,1,(708*m+j));   //变送器
		temp++;
		delay_ms(3);}
		temdata1=read_random(0,1,(708*m+1));
		delay_ms(3);
	    temdata2=read_random(0,1,(708*m+2));   //读取变送器配置的地址数据
		delay_ms(3);
		datatem=temdata1*256+temdata2;
		uart4_buff.tx_data_buff[i++] = trans_query_data[m].flagdata/256;  //变送器探头状态位
		temp++;
		uart4_buff.tx_data_buff[i++] = trans_query_data[m].flagdata%256;  
		temp++;
	//	datatim=trans_query_data[m].flagdata;
		for(x=0;x<4;x++) { 
         if((datatem&0x8000)==0x8000){//判断配置了几种传感器探头
		       uart4_buff.tx_data_buff[i++] = trans_query_data[m].integerh[x];temp++;
               uart4_buff.tx_data_buff[i++] = trans_query_data[m].integerl[x];temp++;
			   uart4_buff.tx_data_buff[i++] = trans_query_data[m].mvdatah[x];temp++;
			   uart4_buff.tx_data_buff[i++] = trans_query_data[m].mvdatal[x];temp++; 
               uart4_buff.tx_data_buff[i++] = trans_query_data[m].decimal[x];temp++;
			   uart4_buff.tx_data_buff[i++] = trans_query_data[m].flag[x];temp++;
											   }
						  datatem=datatem<<1; 
						  }
  

				                     }
				 															  }
				  Alarmdata[n]=0;
				         }
			    }
}

/**********************************************************
 *
 * @file	: Transset_Read_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 读变送器配置的返回信息
 *
 **********************************************************/
void Transset_Read_back(u8 *buffer, u8 Len)
{
	u8 i,k,n,j= 0;
	u8 x,y= 0;
	u8 m= 0;
	u16 temp;
	u16 datatem;//变送器数量
//	u8 transdata=second_device.trans_data;
	
	for (i=0;i<12;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//正常
	if(buffer[11]>second_device.trans_data){
	Comeback_err((u8 *)buffer, Len,TRANS_NOT_EXIST);}
	else {
	for (j=0;j<buffer[11];j++){
	n=buffer[12+j]-1;	   //ID
		uart4_buff.tx_data_buff[i++] = buffer[12+j];
		m++;
    for (k=1;k<3;k++){
	  uart4_buff.tx_data_buff[i++] = read_random(0,1,(708*n+k));//相应配置对应变送器配置类型   读取存储的相应信息
	  delay_ms(3);
   m++;	    
	}
	  datatem=uart4_buff.tx_data_buff[14]+uart4_buff.tx_data_buff[13]*256;
		for(x=0;x<8;x++) { 
         if((datatem&0x8000)==0x8000){
		   for (k=0;k<2;k++){
	  uart4_buff.tx_data_buff[i++] = read_random(0,1,(708*n+3+k+2*x));//相应配置对应变送器配置类型   读取存储的相应信息
	  delay_ms(3);
     m++;				 
	}
				 y++;
				                           }
          datatem=datatem<<1;
                    }
		 			   }
	uart4_buff.tx_len =m+14;
	uart4_buff.tx_data_buff[6]=m+7;
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRC校验低位
	
	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}
 }
/**********************************************************
 *
 * @file	: Transcon_Read_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 读变送器控制门限的返回信息
 *
 **********************************************************/
void Transcon_Read_back(u8 *buffer, u8 Len)
{
	u8 i,k,n= 0;
	u8 x,y= 0;
	u16 temp;
	u16 datatem;//变送器配置
	u8 datatrans;//变送器数量
//	u8 datarelay;//继电器数量
	u8 datatim;//变送器梯度配置
	//datarelay=second_device.trans_data;
	for (i=0;i<12;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//正常
	temp=0;
	n=buffer[11]-1;
	datatrans=read_random(0,1,(708*n+35));//变送器终端配置
				         delay_ms(3); 
	datatem=datatrans*256+read_random(0,1,(708*n+36));//变送器终端配置
				         delay_ms(3); 
		             uart4_buff.tx_data_buff[i++]=datatrans;//				   
		             temp++; 
		             uart4_buff.tx_data_buff[i++]=datatem%256;
		             temp++;
					 
				          for(x=0;x<4;x++) { 
									if((datatem&0x8000)==0x8000){
										uart4_buff.tx_data_buff[i++] = read_random(0,1,(708*n+37+28*x));//读取变送器控制门限的数据 继电器ID
					          delay_ms(3);
					          temp++;
					          uart4_buff.tx_data_buff[i++] = read_random(0,1,(708*n+38+28*x));//读取变送器控制门限的数据 继电器通道
					          delay_ms(3);
					          temp++;
					          uart4_buff.tx_data_buff[i++] = read_random(0,1,(708*n+39+28*x));//读取变送器控制门限的数据 继电器通道
					          delay_ms(3);
					          temp++;
										datatim=read_random(0,1,(708*n+40+28*x));	//存储的梯度配置
										delay_ms(3);
										uart4_buff.tx_data_buff[i++] =datatim;
										temp++;
										for(y=0;y<2;y++){
											if((datatim&0x80)==0x80){
												for (k=0;k<6;k++){
												uart4_buff.tx_data_buff[i++] = read_random(0,1,(708*n+41+28*x+6*y+k));//读取变送器控制门限的数据
												delay_ms(3);
												temp++;
		                                              } 	
												                     }
												datatim=datatim<<1;
														 } 
														         } 
                            datatem=datatem<<1;
                                         } 
	

	    
	uart4_buff.tx_len =temp+14;
	uart4_buff.tx_data_buff[6]=temp+7;
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRC校验低位
	temp = 0;
USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}

/**********************************************************
 *
 * @file	: Alarm_Read_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 读报警门限的返回信息
 *
 **********************************************************/
void Alarm_Read_back(u8 *buffer, u8 Len)
{
	u8 i,k,n= 0;
	u8 x= 0;
	u16 temp;
	u16 datatem;//变送器数量
	u8 datatrans;//变送器数量
		u8 datatim;//变送器数量
	//datatrans=second_device.trans_data;
	for (i=0;i<12;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//正常
	temp=1;
		if(buffer[11]>second_device.trans_data){
	Comeback_err((u8 *)buffer, Len,TRANS_NOT_EXIST);}
	  else {
	n=buffer[11]-1;
	datatrans=read_random(0,1,(708*n+485));//变送器报警配置
				         delay_ms(3); 
	datatim=read_random(0,1,(708*n+486));
	delay_ms(3);
	datatem=datatrans*256+datatim;//变送器报警配置
				         
		             uart4_buff.tx_data_buff[i++]=datatrans;//				   
		             temp++; 
		             uart4_buff.tx_data_buff[i++]=datatim;
		             temp++;
				for(x=0;x<4;x++) { 
                            if((datatem&0x8000)==0x8000){
															for (k=0;k<6;k++){
															uart4_buff.tx_data_buff[i++] = read_random(0,1,(708*n+487+6*x+k));//读取变送器相应探头的报警值
															delay_ms(3);
																	temp++;
		                                            } 
													                      }
                            datatem=datatem<<1; 
                           }
			               
	uart4_buff.tx_len =temp+13;
	uart4_buff.tx_data_buff[6]=temp+6;
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRC校验低位
	
	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}
	}
/**********************************************************
 *
 * @file	: Relayopen_Read_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 读继电器固定时间开启任务的返回信息（无门限）
 *
 **********************************************************/
void Relayopen_Read_back(u8 *buffer, u8 Len)
{
	u8 i,m,k,n= 0;
	u8 x,y= 0;
	u16 temp;
	u8 datatem;//固定时间开启数量
	u8 datatim;//固定时间开启数量
	u8 datatrans;//变送器数量
	datatrans=second_device.relay_data;
	for (i=0;i<10;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[3] = 0xd0;		//正常
	temp=1;	
	if(!buffer[12]){             //读取全部
	uart4_buff.tx_data_buff[i++]=temp/256;
	uart4_buff.tx_data_buff[i++]=temp%256;
	uart4_buff.tx_data_buff[i++]=datatem;
	for (m=1;m<(datatrans+1);m++)
	{
		 		datatem=read_random(0,1,(483*m+11-483));//任务配置
				delay_ms(3);
		    uart4_buff.tx_data_buff[i++]=read_random(0,1,(483*m-483));//继电器终端ID
				delay_ms(3); 
		    temp++; 
		    uart4_buff.tx_data_buff[i++]=read_random(0,1,(483*m+1-483));//继电器终端状态
				delay_ms(3);
		    temp++; 
		    uart4_buff.tx_data_buff[i++]=datatem;
		    temp++;
				for(x=0;x<4;x++) { 
                            if((datatem&0x80)==0x80){
														datatim=read_random(0,1,(483*m+14+19*x-483));	//存储的时间配置
														delay_ms(3);
		                        uart4_buff.tx_data_buff[i++]=read_random(0,1,(483*m+12+19*x-483));//继电器通道
				                    delay_ms(3);
		                        temp++;
		                        uart4_buff.tx_data_buff[i++]=read_random(0,1,(483*m+13+19*x-483));
				                    delay_ms(3);
		                        temp++;
														uart4_buff.tx_data_buff[i++]=datatim;
		                        temp++;
														for(y=0;y<4;y++){
														  if((datatim&0x80)==0x80){	
															for (k=0;k<4;k++){
															uart4_buff.tx_data_buff[i++] = read_random(0,1,(483*m+15+19*x+4*y+k-483));//读取变送器相应探头的报警值
															delay_ms(3);
																	temp++;
		                                            }     }
															datatim=datatim<<1;
														                } 
														                        } 
                            datatem=datatem<<1;
                           } 
	}
	              }
  else {
	temp=9*buffer[12]+1;	
	uart4_buff.tx_data_buff[i++]=temp/256;
	uart4_buff.tx_data_buff[i++]=temp%256;
	uart4_buff.tx_data_buff[i++]=buffer[12];
	for (m=0;m<buffer[12];m++)
	{
		for (n=1;n<(datatrans+1);n++){
			       if(buffer[13+m]==n){
							            datatem=read_random(0,1,(483*n+11-483));//任务配置
				delay_ms(3);
		    uart4_buff.tx_data_buff[i++]=read_random(0,1,(483*n-483));//继电器终端ID
				delay_ms(3); 
		    temp++; 
		    uart4_buff.tx_data_buff[i++]=read_random(0,1,(483*n+1-483));//继电器终端状态
				delay_ms(3);
		    temp++; 
		    uart4_buff.tx_data_buff[i++]=datatem;
		    temp++;
								for(x=0;x<4;x++) { 
                            if((datatem&0x80)==0x80){
														datatim=read_random(0,1,(483*n+14+19*x-483));	//存储的时间配置
														delay_ms(3);
		                        uart4_buff.tx_data_buff[i++]=read_random(0,1,(483*n+12+19*x-483));//继电器通道
				                    delay_ms(3);
		                        temp++;
		                        uart4_buff.tx_data_buff[i++]=read_random(0,1,(483*n+13+19*x-483));
				                    delay_ms(3);
		                        temp++;
														uart4_buff.tx_data_buff[i++]=datatim;
		                        temp++;
														for(y=0;y<4;y++){
														  if((datatim&0x80)==0x80){	
															for (k=0;k<4;k++){
															uart4_buff.tx_data_buff[i++] = read_random(0,1,(483*n+15+19*x+4*y+k-483));//读取变送器相应探头的报警值
															delay_ms(3);
																	temp++;
		                                            }     }
															datatim=datatim<<1;
														                } 
														                        } 
                            datatem=datatem<<1;
                           } 
			                    break;     
                     						 }
										       }
	}
	     }
	uart4_buff.tx_len =temp+14;
	uart4_buff.tx_data_buff[10]=temp/256;
	uart4_buff.tx_data_buff[11]=temp%256;
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRC校验低位
	
	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}

/**********************************************************
 *
 * @file	: Relayopen_Read_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 读继电器固定时间开启任务的返回信息（）
 *
 **********************************************************/
void Relayopengate_Read_back(u8 *buffer, u8 Len)
{
	u8 i,k,n= 0;
	u8 x,y,z= 0;
	u16 temp;
	u16 datatem;//变送器配置
	u8 datatrans;//变送器数量
	//u8 datarelay;//继电器数量
	u8 datatim;//变送器梯度配置
	//datarelay=second_device.trans_data;
	for (i=0;i<12;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//正常
	temp=1;
	n=buffer[11]-1;
	datatrans=read_random(0,0,(366*n+108));//继电器固定任务配置
				         delay_ms(3); 
	datatem=datatrans*256+read_random(0,0,(366*n+109));//继电器固定任务配置
				         delay_ms(3); 
		             uart4_buff.tx_data_buff[i++]=datatrans;//				   
		             temp++; 
		             uart4_buff.tx_data_buff[i++]=datatem%256;
		             temp++;
					 
				          for(x=0;x<8;x++) { 
									if((datatem&0x8000)==0x8000){
											   for(y=0;y<11;y++) {
					          uart4_buff.tx_data_buff[i++] = read_random(0,0,(366*n+110+32*x+y));//读取变送器控制门限的数据 继电器通道
					          delay_ms(3);
					          temp++;                }
										datatim=read_random(0,0,(366*n+121+32*x));	//存储的梯度配置
										delay_ms(3);
										uart4_buff.tx_data_buff[i++] =datatim;
										temp++;
										for(z=0;z<4;z++){
											if((datatim&0x80)==0x80){
												for (k=0;k<5;k++){
												uart4_buff.tx_data_buff[i++] = read_random(0,1,(366*n+122+32*x+5*z+k));//读取变送器控制门限的数据
												delay_ms(3);
												temp++;
		                                              } 	
												                     }
												datatim=datatim<<1;
														       } 
														                 } 
                            datatem=datatem<<1;
                                         } 
	uart4_buff.tx_len =temp+13;
	uart4_buff.tx_data_buff[6]=temp+6;
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRC校验低位
	temp = 0;
USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}
/**********************************************************
 *
 * @file	: Transadjust_Read_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 读变送器固定时间校正的返回信息
 *
 **********************************************************/

/**********************************************************
 *
 * @file	: AllData_Read_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 读所有数据的返回信息
 *
 **********************************************************/
void AllData_Read_back(u8 *buffer, u8 Len)
{
	u8 i,m,k= 0;
	
	u16 temp;
	u8 datatrans;//固定时间数量
	u8 datareley;
	
	for (i=0;i<10;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[3] = 0xd0;		//正常
	
	temp=9*(datatrans+datareley)+2;	
	uart4_buff.tx_data_buff[i++]=temp/256;
	uart4_buff.tx_data_buff[i++]=temp%256;
	uart4_buff.tx_data_buff[i++]=datatrans;
	for (m=0;m<datatrans;m++)
	{
		uart4_buff.tx_data_buff[i++] = m+1;   //变送器
		for (k=0;k<16;k++){
		uart4_buff.tx_data_buff[i++] = buffer[k];//变送器数据   读取存储的相应配置信息
		                  }
	}

	uart4_buff.tx_data_buff[i++]=datareley;
	for (m=0;m<datareley;m++)
	{
		uart4_buff.tx_data_buff[i++] = m+1;   //继电器
		              for (k=0;k<4;k++){
		              uart4_buff.tx_data_buff[i++] = buffer[k];//相应继电器数据
		                               }   
	}

	uart4_buff.tx_len =uart4_buff.tx_data_buff[10]*256+uart4_buff.tx_data_buff[11]+14;
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRC校验低位

	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}

/**********************************************************
 *
 * @file	: ID_Read_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 读设备ID
 *
 **********************************************************/
void ID_Read_back(u8 *buffer, u8 Len)
{
u8 i= 0;
	
	u16 temp;

	uart4_buff.tx_len = 0x11;
	
	for (i=0;i<11;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//正常
	uart4_buff.tx_data_buff[i++] =device_id.year_datah;		  //读存储的设备ID
	uart4_buff.tx_data_buff[i++] =device_id.year_datal;
	uart4_buff.tx_data_buff[i++] =device_id.sn_datah;
	uart4_buff.tx_data_buff[i++] =device_id.sn_datal;

	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff); 		//CRC校验低位
	
	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
	}
/**********************************************************
 *
 * @file	: Systemset_Read_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 读系统设置的返回信息
 *
 **********************************************************/
void Systemset_Read_back(u8 *buffer, u8 Len)
{
	u8 i,x,y= 0;
	//u8 data1,data2;
	u16 temp;
  u16 datatem;
//	uart4_buff.tx_len = 17;
	
	for (i=0;i<13;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//正常 
	 datatem=buffer[11]*256+buffer[12];
		for(x=0;x<16;x++) { 				  
         if((datatem&0x8000)==0x8000){
	  uart4_buff.tx_data_buff[i++] = read_random(0,0,0xC000+18+x);	//系统数据								
		delay_ms(3);
				 y++;
				                           }
          datatem=datatem<<1;
                    }
	uart4_buff.tx_data_buff[6] = y+8;
	uart4_buff.tx_len = 15+y;
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff); 		//CRC校验低位

USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}

/**********************************************************
 *
 * @file	: Transdata_Read_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 读变送器数据的返回信息
 *
 **********************************************************/
void Transdata_Read_back(u8 *buffer, u8 Len)
{
	u8 i,m,n= 0;
	u8 x,j,y= 0;
	u16 temp;
	u16 datatem;//变送器数量
	u8 trans=0;
	u8 datatrans;//变送器数量
	u8 temdata1,temdata2;
	datatrans=second_device.trans_data;
	for (i=0;i<11;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//正常  
	temp=0;
	if(buffer[11]){
	    uart4_buff.tx_data_buff[i++] =buffer[11];
		temp++;
		for (y=0;y<buffer[11];y++)
	{
	    n=buffer[12+y]-1;
		uart4_buff.tx_data_buff[i++] =buffer[12+y];
		temp++;
		//uart4_buff.tx_data_buff[i++] =read_random(0,1,(701*n+1+j));
		//temp++;
		uart4_buff.tx_data_buff[i++] = transID[n].state;   //变送器
		temp++;
		if(transID[n].state!=0xee){
		uart4_buff.tx_data_buff[i++] = transID[n].config_data1;   //变送器
		temp++;
		uart4_buff.tx_data_buff[i++] = transID[n].config_data2;   //变送器
		temp++;
	  datatem=transID[n].config_data1*256+transID[n].config_data2;   //读取变送器配置的地址数据
	//	delay_ms(3);
		uart4_buff.tx_data_buff[i++] = trans_query_data[n].flagdata/256;  //变送器探头状态位
		temp++;
		uart4_buff.tx_data_buff[i++] = trans_query_data[n].flagdata%256;  
		temp++;
		//datatim=trans_query_data[n].flagdata;
		for(x=0;x<4;x++) { 
         if((datatem&0x8000)==0x8000){//判断配置了几种传感器探头
		       uart4_buff.tx_data_buff[i++] = trans_query_data[n].integerh[x];temp++;
               uart4_buff.tx_data_buff[i++] = trans_query_data[n].integerl[x];temp++;
			   uart4_buff.tx_data_buff[i++] = trans_query_data[n].mvdatah[x];temp++;
			   uart4_buff.tx_data_buff[i++] = trans_query_data[n].mvdatal[x];temp++; 
               uart4_buff.tx_data_buff[i++] = trans_query_data[n].decimal[x];temp++;
			   uart4_buff.tx_data_buff[i++] = trans_query_data[n].flag[x];temp++;
											   }
						  datatem=datatem<<1;
						  }
        }                    }
         }
   else{
   uart4_buff.tx_data_buff[i++] = datatrans;
   temp++;
   for(m=0;m<datatrans;m++){
		 if(transID[n].state!=0xee){
			 trans++;
        uart4_buff.tx_data_buff[i++] = m+1;
		temp++;
 		uart4_buff.tx_data_buff[i++] = transID[m].state;   //变送器
		temp++;
		uart4_buff.tx_data_buff[i++] = transID[m].config_data1;   //变送器
		temp++;
		uart4_buff.tx_data_buff[i++] = transID[m].config_data2;   //变送器
		temp++;
		temdata1=transID[m].config_data1;
		delay_ms(3);
	  temdata2=transID[m].config_data2;   //读取变送器配置的地址数据
		delay_ms(3);
		datatem=temdata1*256+temdata2;
		uart4_buff.tx_data_buff[i++] = trans_query_data[m].flagdata/256;  //变送器探头状态位
		temp++;
		uart4_buff.tx_data_buff[i++] = trans_query_data[m].flagdata%256;  
		temp++;
	//	datatim=trans_query_data[m].flagdata;
		for(x=0;x<4;x++) { 
         if((datatem&0x8000)==0x8000){//判断配置了几种传感器探头
		       uart4_buff.tx_data_buff[i++] = trans_query_data[m].integerh[x];temp++;
               uart4_buff.tx_data_buff[i++] = trans_query_data[m].integerl[x];temp++;
			   uart4_buff.tx_data_buff[i++] = trans_query_data[m].mvdatah[x];temp++;
			   uart4_buff.tx_data_buff[i++] = trans_query_data[m].mvdatal[x];temp++; 
               uart4_buff.tx_data_buff[i++] = trans_query_data[m].decimal[x];temp++;
			   uart4_buff.tx_data_buff[i++] = trans_query_data[m].flag[x];temp++;
											   }
						  datatem=datatem<<1; 
						  }
            }
   }
	 uart4_buff.tx_data_buff[11] = trans;
   }
   	uart4_buff.tx_data_buff[6]=temp+6;
	uart4_buff.tx_len =temp+13;
	 
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRC校验低位

USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);

  for(m=0;m<datatrans;m++){
		 if(transID[m].state!=0xee){
	datatem=transID[m].config_data1*256+transID[m].config_data2;		  //存储传感数据
	for(x=0;x<4;x++) { 
         if((datatem&0x8000)==0x8000){//判断配置了几种传感器探头
		       	write_byte(0,1,(708*n+594+6*x),trans_query_data[n].integerh[x]);        
	delay_ms(5);
	write_byte(0,1,(708*n+595+6*x),trans_query_data[n].integerl[x]);        
	delay_ms(5);
	write_byte(0,1,(708*n+596+6*x),trans_query_data[n].mvdatah[x]);        
	delay_ms(5);
	write_byte(0,1,(708*n+597+6*x),trans_query_data[n].mvdatal[x]);        
  delay_ms(5);
	write_byte(0,1,(708*n+598+6*x),trans_query_data[n].decimal[x]);       
  delay_ms(5);
  write_byte(0,1,(708*n+599+6*x),trans_query_data[n].flag[x]);        
	delay_ms(5);  
											   }
						  datatem=datatem<<1; 
						  }          }}

 for (i=0;i<12;i++){
	                   	uart4_buff.tx_data_buff[i] = buffer[i];
                                         }
						uart4_buff.tx_data_buff[9] = 0x60;
						uart4_buff.tx_data_buff[11] =0;
						temp=0;
 for(n=0;n<datatrans;n++){
        for(x=0;x<4;x++) {
            if(!(trans_query_data[n].decimal[x]|trans_query_data[n].flag[x])){  //上位机查询2次以上都是异常数据则主动上报
				 Alarmdata[n]++;
				 if(Alarmdata[n]==2){
				 	  Alarmdata[n]=0;
					  uart4_buff.tx_data_buff[11] +=1;
					  uart4_buff.tx_data_buff[i++] =n;
					 temp++;
					  for(j=0;j<3;j++){
		uart4_buff.tx_data_buff[i++] = read_random(0,1,(708*m+j));   //变送器
		temp++;
		delay_ms(3);}
		temdata1=read_random(0,1,(708*m+1));
		delay_ms(3);
	    temdata2=read_random(0,1,(708*m+2));   //读取变送器配置的地址数据
		delay_ms(3);
		datatem=temdata1*256+temdata2;
		uart4_buff.tx_data_buff[i++] = trans_query_data[m].flagdata/256;  //变送器探头状态位
		temp++;
		uart4_buff.tx_data_buff[i++] = trans_query_data[m].flagdata%256;  
		temp++;
	//	datatim=trans_query_data[m].flagdata;
		for(x=0;x<4;x++) { 
         if((datatem&0x8000)==0x8000){//判断配置了几种传感器探头
		       uart4_buff.tx_data_buff[i++] = trans_query_data[m].integerh[x];temp++;
               uart4_buff.tx_data_buff[i++] = trans_query_data[m].integerl[x];temp++;
			   uart4_buff.tx_data_buff[i++] = trans_query_data[m].mvdatah[x];temp++;
			   uart4_buff.tx_data_buff[i++] = trans_query_data[m].mvdatal[x];temp++; 
               uart4_buff.tx_data_buff[i++] = trans_query_data[m].decimal[x];temp++;
			   uart4_buff.tx_data_buff[i++] = trans_query_data[m].flag[x];temp++;
											   }
						  datatem=datatem<<1; 
						         }
				                     }
				 															  }
				  Alarmdata[n]=0;
				         }
			    }
  uart4_buff.tx_data_buff[6]=temp+6;
	uart4_buff.tx_len =temp+13;
 if(temp){
 USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);}
}
/**********************************************************
 *
 * @file	: Handshake_Read_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 读设备是否连接正常的返回信息
 *
 **********************************************************/
void Handshake_Read_back(u8 *buffer, u8 Len)
{
	u8  i;
	u16 temp;
	uart4_buff.tx_len = 13;
		for (i=0;i<11;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[6] = 0x06;		//正常
		temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
		uart4_buff.tx_data_buff[11] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
		uart4_buff.tx_data_buff[12] = (u8)(temp&0x00ff); 		//CRC校验低位

	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
	online=1;
	//STOP_TIME3;
}
void heartbeat(void)
{
	u16 temp;
		uart4_buff.tx_len = 14;
	
	
		uart4_buff.tx_data_buff[0] = device_id.year_datah;
		uart4_buff.tx_data_buff[1] = device_id.year_datal;
		uart4_buff.tx_data_buff[2] = device_id.sn_datah;
		uart4_buff.tx_data_buff[3] = device_id.sn_datal;					// 	
		uart4_buff.tx_data_buff[4] =0x03;
		uart4_buff.tx_data_buff[5] =0x40 ;
		uart4_buff.tx_data_buff[6] = 0x07;
		uart4_buff.tx_data_buff[7] = 0x0E; 	//长度
		uart4_buff.tx_data_buff[8] = 0x00;
		uart4_buff.tx_data_buff[9] = 0x00;
	  uart4_buff.tx_data_buff[10] = 0x01;
	  uart4_buff.tx_data_buff[11] = ComFlag;


		temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
		uart4_buff.tx_data_buff[12] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
		uart4_buff.tx_data_buff[13] = (u8)(temp&0x00ff); 		//CRC校验低位

	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);

}

/**********************************************************
 *
 * @file	: Salt_Read_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 读盐度补偿的返回信息
 *
 **********************************************************/
void Salt_Read_back(u8 *buffer, u8 Len)
{
	u8 i,n= 0;
	u16 temp;
//	u8 datatem;//变送器数量
//	datatem=second_device.trans_data;
	uart4_buff.tx_len =16;

	for (i=0;i<12;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//正常
	uart4_buff.tx_data_buff[6] = 0x09;
	if(buffer[11]>second_device.trans_data){
	Comeback_err((u8 *)buffer, Len,TRANS_NOT_EXIST);}
	else{
	n=buffer[11]-1;
	yandu_read(n);
	    }
}
/**********************************************************
 *
 * @file	: Write_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 写命令字帧数据正确的返回信息
 *
 **********************************************************/
void Write_back(u8 *buffer, u8 Len)
{
	u8 i = 0;
	u16 temp;

	uart4_buff.tx_len = 0x0d;

	for (i=0;i<11;i++)

	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[6] = uart4_buff.tx_len - 7; 	//长度
	uart4_buff.tx_data_buff[8] = 0xd0; 	//
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff); 		//CRC校验低位


	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
	
}

/**********************************************************
 *
 * @file	: ID_Write_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 写设备ID及返回信息
 *
 **********************************************************/
void ID_Write_back(u8 *buffer, u8 Len)
{
	write_byte(0,0,0xC000,buffer[11]);
	delay_ms(5);
	write_byte(0,0,0xC001,buffer[12]);
	delay_ms(5);
	write_byte(0,0,0xC002,buffer[13]);
	delay_ms(5);
	write_byte(0,0,0xC003,buffer[14]);
	delay_ms(5);
	IDDataflag=0;
	write_byte(0,0,0xF000,0x25);
	delay_ms(5);
	Write_back( buffer, Len );
}
/**********************************************************
 *
 * @file	: DeviceHardware_Write_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 写固件版本及返回信息
 *
 **********************************************************/
void DeviceHardware_Write_back(u8 *buffer, u8 Len)
{
	write_byte(0,0,0xC004,buffer[11]);
	delay_ms(3);
	write_byte(0,0,0xC005,buffer[12]);
	delay_ms(3);
	write_byte(0,0,0xC006,buffer[13]);
	delay_ms(3);
	write_byte(0,0,0xC007,buffer[14]);
	delay_ms(3);
	write_byte(0,0,0xC008,buffer[15]);
	delay_ms(3);
	write_byte(0,0,0xC009,buffer[16]);
	delay_ms(3);
	write_byte(0,0,0xC00A,buffer[17]);
	delay_ms(3);
	write_byte(0,0,0xC00B,buffer[18]);
	delay_ms(3);
	write_byte(0,0,0xC00C,buffer[19]);
	delay_ms(3);
	write_byte(0,0,0xC00D,buffer[20]);
	delay_ms(3);
	verDataflag=0;
	write_byte(0,0,0xF001,0x25);
	delay_ms(3);
	Write_back( buffer, Len );
}
/**********************************************************
 *
 * @file	: Relayset_Write_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 写继电器配置及返回信息
 *
 **********************************************************/
void Relayset_Write_back(u8 *buffer, u8 Len)
{
u8 k,m;
u8 n=0;
u8 i=0;	
//	u16 temp;
	u8 datatrans;//变送器数量
	//u8 sdata[2];
	//u16 datatem;
	datatrans=second_device.relay_data;
	
  //	n=buffer[10]-1;	   //ID
	//datatem=buffer[11]*256+buffer[12];

	switch(buffer[11]){
					case 0x52:		//删除继电器配置
					    if(!buffer[12]){
						for (m=0;m<datatrans;m++)
	                               {
																	 relaysetflag[m]=0;
	              //	for (k=1;k<3;k++){
								write_byte(0,0,366*m,0xEE);
	                            delay_ms(3);         //擦写存储变送器终端状态
		                               //}
	                               }
									write_byte(0,0,0xC053,0);           //变送器最大数
	                delay_ms(3);	
                  second_device.relay_data =0;							 
						               }

						else {
						for (m=0;m<buffer[12];m++)
	                    {
		             for (n=1;n<(datatrans+1);n++)      {
			                 if(buffer[13+m]==n){
												 relaysetflag[n-1]=0;
		                      // for (k=1;k<3;k++){
		                       //擦写存储变送器的地址
													 write_byte(0,0,366*(n-1),0xee);
													 delay_ms(3);
		                        //                }   
			                        //  break;     
                     						          }
										                           }
	                     }
						     }									
						break;
					
					case 0x51:		//覆盖继电器配置
						   for (m=0;m<buffer[12];m++)
	                    {           //判断配置了几种传感器探头
		                  // for (n=1;n<datatrans+1;n++)      {
			                      //if(buffer[13+i]==n){         //ID匹配
												n=buffer[13+i];
															relaysetflag[n-1]=0;
												write_byte(0,0,366*(n-1),0xe2);
												delay_ms(3);
										for (k=0;k<3;k++){				  //写终端配置
												write_byte(0,0,k+366*(n-1)+67,buffer[14+i+k]);
												delay_ms(3);
		                                                   }
			                              // break;     
                     						           //       }
										                      //    }
										        i+=4;                   //
	                     } 	
					        write_byte(0,0,0xC053,n);           //继电器最大数
	                delay_ms(3);	
                  second_device.relay_data =n;													
						break;
					
					default: 
						break;
	}	     
    Write_back( buffer, Len );
}
/**********************************************************
 *
 * @file	: Transset_Write_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 写变送器配置及返回信息
 *
 **********************************************************/
void Transset_Write_back(u8 *buffer, u8 Len)
{
u8 k,m,n;
u8 x;
u8 i=0;
u8 j=0;
u8 t=0;	
//	u16 temp;
	u8 datatrans;//变送器数量
	//u8 sdata[2];
	u16 datatem;
	datatrans=second_device.trans_data;
	//setDataflag[m]=0;
  //	n=buffer[10]-1;	   //ID
	//datatem=buffer[11]*256+buffer[12];

switch(buffer[11]){
					case 0x52:		//删除变送器配置
					    if(!buffer[12]){
						for (m=0;m<datatrans;m++)
	                               {
									setDataflag[m]=0;
                   transflag[m]=1;																	 
	              	for (k=1;k<3;k++){
								write_byte(0,1,708*m+k,0x00);
	                            delay_ms(3);         //擦写存储变送器的地址
		                               }
									write_byte(0,1,708*m,0xee);
	                            delay_ms(3);   
	                               }
									write_byte(0,0,0xC052,0);           //变送器最大数
	                delay_ms(3);	
                  second_device.trans_data =0;	
						               }

						else {
						for (m=0;m<buffer[12];m++)
	                    {
		             for (n=1;n<(datatrans+1);n++)      {
			                 if(buffer[13+m]==n){
												 setDataflag[n-1]=0;
												 transflag[n-1]=1;
		                       for (k=1;k<3;k++){
		                       //擦写存储变送器的地址
													 write_byte(0,1,708*(n-1)+k,0x00);
													 delay_ms(3);
		                                        } 
                            write_byte(0,1,708*(n-1),0xee);
	                            delay_ms(3);													 
			                          break;     
                     						          }
										                            }
	                     }
						}									
						break;
					
					case 0x51:		//覆盖变送器配置
						   for (m=0;m<buffer[12];m++)
	                    {           //判断配置了几种传感器探头
		                   //for (n=1;n<datatrans+1;n++)      {
			                     // if(buffer[13+i]==n){         //ID匹配
												   n=buffer[13+i];
															setDataflag[n-1]=0;
															transflag[n-1]=1;
														write_byte(0,1,708*(n-1),0xe2);
	                          delay_ms(3);
										for (k=0;k<2;k++){				  //写终端配置
												write_byte(0,1,k+708*(n-1)+1,buffer[14+i+k]);
												delay_ms(3);
		                                                   }
													datatem=buffer[14+i]*256+buffer[15+i];				  
												    for(x=0;x<4;x++) { 
                                       if((datatem&0x8000)==0x8000){           //判断传感器探头类型  写入相应类型的数据
																		 for (k=0;k<2;k++){
																			 write_byte(0,1,k+708*(n-1)+3+2*x,buffer[16+i+2*t+k]);
																			 delay_ms(3);
		                                                            }
																	t++;           
																}
                                       datatem=datatem<<1; 
                                              }    
			                              // break;     
                     						      //            }
										                      //    }
						     datatem=buffer[14+i]*256+buffer[15+i];//第一个变送器配置状态
							for(x=0;x<4;x++) { 
                            if((datatem&0x8000)==0x8000){j++;} 
                            datatem=datatem<<1; 
                                         } 
										        i+=2*j+3;                   //判断配置了几种传感器探头
	                     }
									write_byte(0,0,0xC052,n);           //变送器最大数
	                delay_ms(3);	
                  second_device.trans_data =n;											 
						break;
					
					default: 
						break;
	}	     
    Write_back( buffer, Len );
}

/**********************************************************
 *
 * @file	: Transcon_Write_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 写变送器控制门限及返回信息
 *
 **********************************************************/
void Transcon_Write_back(u8 *buffer, u8 Len)
{
u8 m,n= 0;
u16 k= 0;
u8 x,y= 0;
u8 b=0;
u8 j=0;	
//	u16 temp;
	u8 datatrans;//变送器数量
	u16 datatem;
		u8 sdata[2];
	u8 datatim;//变送器梯度配置
	datatrans=second_device.trans_data;
	//gateDataflag=0;
	//datatrans=buffer[11];

	switch(buffer[11]){
					case 0x52:		//删除变送器配置
					    if(!buffer[12]){
						for (m=0;m<datatrans;m++)
	                               {
									gateDataflag[m]=0;
	              	for (k=0;k<2;k++){
								write_byte(0,1,708*m+35+k,0x00);
	                            delay_ms(3);         //擦写存储变送器的地址
		                               }
	                               }
						               }
						else {
		             for (n=1;n<(datatrans+1);n++)      {
			                 if(buffer[12]==n){
												 gateDataflag[n-1]=0;
							     if(buffer[6]==0x0a){
								 sdata[0]= read_random(0,1,708*(n-1)+35);
								 delay_ms(3);
								 sdata[1]= read_random(0,1,708*(n-1)+36);
								 delay_ms(3);
								 sdata[0] &=(~buffer[13]);
								 sdata[1] &=(~buffer[14]);
								 for (k=0;k<2;k++){
		                       //擦写存储变送器的地址
													 write_byte(0,1,708*(n-1)+35+k,sdata[k]);
													 delay_ms(3);
		                                        }
								                    }
								 else {
		                       for (k=0;k<2;k++){
		                       //擦写存储变送器的地址
													 write_byte(0,1,708*(n-1)+35+k,0x00);
													 delay_ms(3);
		                                        }
									   }			   
			                          break;     
                     						          }
										                            }
						    }									
						break;
					
					case 0x51:		//覆盖变送器配置
		                   for (n=1;n<datatrans+1;n++)      {
			                      if(buffer[12]==n){         //ID匹配
															gateDataflag[n-1]=0;
								        sdata[0]= read_random(0,1,708*(n-1)+35);
								        delay_ms(3);
								        sdata[1]= read_random(0,1,708*(n-1)+36);
								        delay_ms(3);
										sdata[0] |=	buffer[13];
										sdata[1] |=	buffer[14];
										for (k=0;k<2;k++){				  //写终端配置
												write_byte(0,1,708*(n-1)+35+k,sdata[k]);
												delay_ms(3);
		                                                   }
													datatem=sdata[0]*256+sdata[1];				  
												    for(x=0;x<4;x++) { 
                                                      if((datatem&0x8000)==0x8000){
														datatim=buffer[18+j];	
														write_byte(0,1,28*x+708*(n-1)+37,buffer[15+j]);	 //继电器ID和通道
														delay_ms(3);
														write_byte(0,1,28*x+708*(n-1)+38,buffer[16+j]);
														delay_ms(3);
														write_byte(0,1,28*x+708*(n-1)+39,buffer[17+j]);
														delay_ms(3);
														write_byte(0,1,28*x+708*(n-1)+40,buffer[18+j]);  //梯度配置
														delay_ms(3);
														for(y=0;y<4;y++){
														  if((datatim&0x80)==0x80){
																for (k=0;k<6;k++){
															write_byte(0,1,k+708*(n-1)+41+28*x+6*y,buffer[19+j+k+6*y]);
															delay_ms(3);
														               }	
																	   
															             b++;           }
															datatim=datatim<<1;
														                } 
														                        } 
                            datatem=datatem<<1; 
														j+=b*6+4;
														b=0;
                                                                     }    
			                               break;     
                     						                  }
										                          }
						break;
					
					default: 
						break;
	}
	Write_back( buffer, Len );
}

/**********************************************************
 *
 * @file	: Alarm_Write_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 写变送器报警门限及返回信息
 *
 **********************************************************/
void Alarm_Write_back(u8 *buffer, u8 Len)
{
u8 m,k,n= 0;
u8 x,y= 0;
u8 sdata[2];	
	//u16 temp;
	u8 datatrans;//变送器数量
	u16 datatem;
	datatrans=second_device.trans_data;
	//alaDataflag=0;

	switch(buffer[11]){
					case 0x52:		//删除变送器报警
					    if(!buffer[12]){
						for (m=0;m<datatrans;m++)
	                               {
										alaDataflag[m]=0;							 
	              	for (k=0;k<2;k++){
								write_byte(0,1,708*m+485,0x00);
	                            delay_ms(3);         //擦写存储变送器的地址
		                               }
	                               }
						               }
						else {
							alaDataflag[buffer[12]-1]=0;
		                       for (k=0;k<2;k++){
		                       //擦写存储变送器报警配置的地址
										write_byte(0,1,708*(buffer[12]-1)+k+485,0x00);
										delay_ms(3);
		                                        }  
						     }									
						break;
					
					case 0x51:		//添加修改变送器报警
						alaDataflag[buffer[12]-1]=0;
										for (k=0;k<2;k++){
										     sdata[k]=read_random(0,1,708*(buffer[12]-1)+k+485);
											 delay_ms(3);
											 sdata[k] |=buffer[13+k];
											             }
										for (k=0;k<2;k++){				  //写报警配置
												write_byte(0,1,k+708*(buffer[12]-1)+485,sdata[k]);
												delay_ms(3);
		                                                   }
													datatem=buffer[13]*256+buffer[14];				  
										for(x=0;x<8;x++) { 
                                           if((datatem&0x8000)==0x8000){
		                                     for (k=0;k<6;k++){
	                                                 write_byte(0,1,708*n+487+k+6*x,buffer[15+6*y+k]);//相应配置对应变送器配置类型   读取存储的相应信息
	                                                 delay_ms(3);	    
	                                                          }
				                                                 y++;
				                                                        }
                                                      datatem=datatem<<1;
                                                          }
						break;
					
					default: 
						break;
	}
    Write_back( buffer, Len );
}


/**********************************************************
 *
 * @file	: Relayopen_Write_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 写继电器固定开启及返回信息（）
 *
 **********************************************************/
void Relayopengate_Write_back(u8 *buffer, u8 Len)
{
u8 m,n= 0;
u16 k= 0;
u8 j,x,y= 0;
u8 b=0;	
//	u16 temp;
	u8 datatrans;//变送器数量
	u16 datatem;
		u8 sdata[2];
	u8 datatim;//变送器梯度配置
	datatrans=second_device.relay_data;
	//gateDataflag=0;
	//datatrans=buffer[11];

	switch(buffer[11]){
					case 0x52:		//删除继电器固定任务
					    if(!buffer[12]){
						for (m=0;m<datatrans;m++)
	                               {
									relaygateDataflag[m]=0;
	              	for (k=0;k<2;k++){
								write_byte(0,0,366*m+108+k,0x00);
	                            delay_ms(3);         //擦写存储变送器的地址
		                               }
	                               }
						               }
						else {
		             for (n=1;n<(datatrans+1);n++)      {
			                 if(buffer[12]==n){
												 relaygateDataflag[n-1]=0;
								 for (k=0;k<2;k++){
		                       //擦写存储变送器的地址
													 write_byte(0,0,366*(n-1)+108+k,0x00);
													 delay_ms(3);
		                                        }
			                          break;     
                     						          }
										                            }
						    }									
						break;
					
					case 0x51:		//覆盖变送器配置
		                   for (n=1;n<datatrans+1;n++)      {
			                      if(buffer[12]==n){         //ID匹配
															relaygateDataflag[n-1]=0;
								        sdata[0]= read_random(0,0,366*(n-1)+108);
								        delay_ms(3);
								        sdata[1]= read_random(0,0,366*(n-1)+109);
								        delay_ms(3);
										sdata[0] |=	buffer[13];
										sdata[1] |=	buffer[14];
										for (k=0;k<2;k++){				  //写终端配置
												write_byte(0,0,366*(n-1)+108+k,sdata[k]);
												delay_ms(3);
		                                                   }
													datatem=buffer[13]*256+	buffer[14];				  
												    for(x=0;x<8;x++) { 
                                   if((datatem&0x8000)==0x8000){
														datatim=buffer[26+j];	
																				for (k=0;k<12;k++){
														write_byte(0,0,32*x+366*(n-1)+110+k,buffer[15+j+k]);	 //继电器ID和通道
														delay_ms(3);                   }
														for(y=0;y<4;y++){
														  if((datatim&0x80)==0x80){
																for (k=0;k<5;k++){
															write_byte(0,1,k+32*x+366*(n-1)+122+5*y,buffer[27+j]);
															delay_ms(3);
														               }	
															             b++;           }
															datatem=datatem<<1;
														                } 
														                        } 
                            datatem=datatem<<1; 
														j+=b*5+12;
														b=0;
                                                                     }    
			                               break;     
                     						                  }
										                          }
						break;
					
					default: 
						break;
	}
	Write_back( buffer, Len );
}


/**********************************************************
 *
 * @file	: Systime_Write_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 设置系统时间及返回信息
 *
 **********************************************************/
void Systime_Write_back(u8 *buffer, u8 Len)
{
	settmtime.tm_year = buffer[11];
	settmtime.tm_mon= buffer[12];
	settmtime.tm_mday = buffer[13];
	settmtime.tm_week= buffer[14];
	settmtime.tm_hour= buffer[15];
	settmtime.tm_min= buffer[16];
	settmtime.tm_sec= buffer[17];
	write_byte(0,0,0x0008,buffer[11]);
	delay_ms(5);
	write_byte(0,0,0x0009,buffer[12]);
	delay_ms(5);
	write_byte(0,0,0x000A,buffer[13]);
	delay_ms(5);
	write_byte(0,0,0x000B,buffer[14]);
	delay_ms(5);
	write_byte(0,0,0x000C,buffer[15]);
	delay_ms(5);
	write_byte(0,0,0x000D,buffer[16]);
	delay_ms(5);
	write_byte(0,0,0x000E,buffer[17]);
	delay_ms(5);
	//write_byte(0,0,0x007E,0x25);
	//delay_ms(5);
	Time_Set(&settmtime);
	
	Write_back( buffer, Len );
}

/**********************************************************
 *
 * @file	: Systemset_Write_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 写系统设置及返回信息
 *
 **********************************************************/
void Systemset_Write_back(u8 *buffer, u8 Len)
{
u8 x,y= 0;	
//	u16 temp;
//	u8 datatrans;//变送器数量
	u16 datatem;
	//alaDataflag=0;
	sysDataflag=0;
	datatem=buffer[11]*256+buffer[12];
	  write_byte(0,0,0xC000+16,buffer[11]);//相应配置对应数据配置   
	  delay_ms(3);
	    write_byte(0,0,0xC000+17,buffer[12]);//相应配置对应数据配置
	  delay_ms(3);
		for(x=0;x<16;x++) { 				  
         if((datatem&0x8000)==0x8000){
	  write_byte(0,0,0xC000+18+x,buffer[13+y]);//相应配置对应变送器配置类型   读取存储的相应信息
	  delay_ms(3);
				 y++;
				                           }
          datatem=datatem<<1;
                    }
	Write_back( buffer, Len );
}

/**********************************************************
 *
 * @file	: Relaycon_Write_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 写继电器手动控制及返回信息   注意释放和删除的区别  释放是释放该继电器的手动控制优先级   删除是恢复默认但不释放控制优先级  
 *
 **********************************************************/
void Relaycon_Write_back(u8 *buffer, u8 Len)
{

u8 y= 0;	
//	u16 temp;
	//u8 datatrans;//继电器数量
//	u8 datatem;
	u16 datatim;
//	datatrans=second_device.relay_data ;
	relayshouDataflag[buffer[11]-1]=0;
	datatim= buffer[12]*256+buffer[13];
	if(buffer[11]>second_device.relay_data){
	Comeback_err((u8 *)buffer, Len,RELAY_NOT_EXIST);}
	else{
                for(y=0;y<8;y++){
				if((datatim&0x8000)==0x8000){
					 shoudong_control[buffer[11]-1].shouflag[y]=1;
					                    //发送开启/关闭继电器通道指令
		       shoudong_control[buffer[11]-1].channeldata |=(1<<(15-y));
					//shoudong_control[buffer[11]-1].channeldata |=(1<<(15-y));
					write_byte(0,0,366*(buffer[11]-1)+11,shoudong_control[buffer[11]-1].channeldata/256);//相应通道
	                  delay_ms(3);
					write_byte(0,0,366*(buffer[11]-1)+12,shoudong_control[buffer[11]-1].channeldata%256);//相应通道
	                  delay_ms(3);
					shoudong_control[buffer[11]-1].type[y]=buffer[14];
				  time_get(Time_GetData);
					shoudong_control[buffer[11]-1].timedatah[y]=Time_GetData[4];
					shoudong_control[buffer[11]-1].timedatal[y]=buffer[15]*256+buffer[16]+Time_GetData[5];
					if(shoudong_control[buffer[11]-1].timedatal[y]>60){
					 shoudong_control[buffer[11]-1].timedatal[y]=shoudong_control[buffer[11]-1].timedatal[y]-60; 
					shoudong_control[buffer[11]-1].timedatah[y]=1+Time_GetData[4];
					}
									 write_byte(0,0,366*(buffer[11]-1)+15+y*3,buffer[14]);//相应通道的动作类型
	                  delay_ms(3);
									 write_byte(0,0,366*(buffer[11]-1)+14+y*3,shoudong_control[buffer[11]-1].timedatah[y]);//相应通道的动作时间
	                  delay_ms(3);
									 write_byte(0,0,366*(buffer[11]-1)+15+y*3,shoudong_control[buffer[11]-1].timedatal[y]);//相应通道的动作时间
	                  delay_ms(3);
					          break;
								             }
							 datatim=datatim<<1; 
											        }		
														}								
 	Write_back( buffer, Len );
}


/**********************************************************
 *
 * @file	: Salt_Write_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 写盐度补偿数据及返回信息
 *
 **********************************************************/
void Salt_Write_back(u8 *buffer, u8 Len)
{
    u8 k,n= 0;
	
	n=buffer[11]-1;	   //ID
	yanDataflag[n]=0;
	for (k=0;k<2;k++){
	  write_byte(0,1,708*n+583+k,buffer[12+k]);//相应配置对应变送器盐度补偿数据
	  delay_ms(3);	    
	                 }
	 // delay_ms(500);
		yanflag=1;
	//	OSTaskSuspend(HAND_TASK_PRIO);							 
	//	yandu_write(buffer[11],buffer[12],buffer[13]);
		//delay_ms(500);
	//	OSTaskResume(HAND_TASK_PRIO);
   // Write_back( buffer, Len );
}


/**********************************************************
 *
 * @file	: Transzhun_Write_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 设置变送器校准及返回信息
 *
 **********************************************************/
void Transzhun_Write_back(u8 *buffer, u8 Len)
{
	//u8 n= 0;
   // u8 x= 0;	
//	u16 temp;
//	u8 datatrans;//变送器数量
	u16 datatem;
	u8 addr,type;
//	datatrans=second_device.trans_data;
	jiaoDataflag=0;
   // n=buffer[11]-1;	   //ID
	 type=buffer[14];	   //ID
	datatem=buffer[12]*256+buffer[13];
		//for(x=0;x<8;x++) { 
       //  if((datatem&0x8000)==0x8000){
	zhunflag=1;
		   //	 addr=buffer[11];
		//		jiaozhun_send(addr,datatem,type);
//	delay_ms(300);
			//	break;
			//	                     }
      //    datatem=datatem<<1;
                    //}
}
/**********************************************************
 *
 * @file	: DeviceHardware_Write_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 读取设备硬件版本及返回信息
 *
 **********************************************************/
void DeviceHardware_Read_back(u8 *buffer, u8 Len)
{
u8 i= 0;
	
	u16 temp;

	uart4_buff.tx_len = 0x17;
	
	for (i=0;i<11;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//正常	
	
	uart4_buff.tx_data_buff[i++]=program_ver.ver_data1 ;        //硬件版本号
	uart4_buff.tx_data_buff[i++]=program_ver.ver_data2 ;

	uart4_buff.tx_data_buff[i++] = program_ver.ver_data3;		  
	uart4_buff.tx_data_buff[i++] = program_ver.ver_data4;
	for (i=15;i<21;i++)
	{
		uart4_buff.tx_data_buff[i] = program_ver.id_data[i-15];
	}
//	uart4_buff.tx_data_buff[i++] = (u8)((device_address>>8)&0x00ff);//读存储的设备序列号
//	uart4_buff.tx_data_buff[i++] = (u8)((device_address>>8)&0xff);

	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff); 		//CRC校验低位

	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}

/**********************************************************
 *
 * @file	: DeviceCon_Write_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 设置设备使能及返回信息
 *
 **********************************************************/
void DeviceCon_Write_back(u8 *buffer, u8 Len)
{

	enDataflag=0;

	switch(buffer[11]){
		 	case 0x11:
			   write_byte(0,0,0xc023,buffer[13]);
	           delay_ms(5);
				   	break;
			case 0x21:
			   write_byte(0,0,0x0c23,buffer[13]);
	           delay_ms(5);
				   	break;
			case 0x31:
			   write_byte(0,0,0xc023,buffer[13]);
	           delay_ms(5);
				   	break;
			default: 
						break;
	}
 Write_back( buffer, Len );
}


/**********************************************************
 *
 * @file	: Reset_Write_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 设置设备重启及返回信息
 *
 **********************************************************/
void Reset_Write_back(u8 *buffer, u8 Len)
{
	u8 i= 0;
	u16 temp;
	comDataflag=0;
	for (i=0;i<12;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[3] = 0xd0;		//正常	
	uart4_buff.tx_data_buff[11]=buffer[11]+1;
	uart4_buff.tx_data_buff[i++]=buffer[13];		//正常
	uart4_buff.tx_data_buff[i++]=buffer[14];		//正常
	uart4_buff.tx_data_buff[i++]=buffer[15];		//正常
	uart4_buff.tx_data_buff[i++]=0xd0;		//正常
	uart4_buff.tx_len =18;
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRC校验低位	
	
	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
	
	write_byte(0,0,0x0008,0x04);
	delay_ms(5);
	
	SoftReset();
	
}

/**********************************************************
 *
 * @file	: Intelligence_Write_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 写变送器智能控制及返回信息
 *
 **********************************************************/
void Intelligence_Write_back(u8 *buffer, u8 Len)
{
    u8 k,n= 0;
	
    n=buffer[11]-1;	   //ID
	inteDataflag[n]=0;
		   for (k=0;k<2;k++){
	  write_byte(0,1,708*n+590+k,buffer[12+k]);//智能控制
	  delay_ms(3);	    
	                        }
     Write_back( buffer, Len );
}

/**********************************************************
 *
 * @file	: ID_Write_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 写二级设备ID及返回信息
 *
 **********************************************************/
void SecondID_Write_back(u8 *buffer, u8 Len)
{
	switch(buffer[10])     { 
				case 0x21:    //变送器类型
							 write_byte(0,0,256+201*buffer[11],buffer[12]);	//擦写存储二级设备的新ID										
					     delay_ms(3);
					break;
				
				case 0x31:      //继电器类型
		           write_byte(0,1,483*buffer[11],buffer[12]);	//擦写存储二级设备的新ID										
					     delay_ms(3); 
					break;

					default: 
						break;
	                               } 			
     Write_back( buffer, Len );
}
/**********************************************************
 *
 * @file	: ID_Write_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 写二级设备ID及返回信息
 *
 **********************************************************/
void Secondver_Read_back(u8 *buffer, u8 Len)
{
seVerflag=1;
//Ver_Read(buffer[11],buffer[12]);
}

/**********************************************************
 *
 * @file	: SetmaxSecondID_Write_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 设置最大挂载数量及返回信息
 *
 **********************************************************/
void SetmaxSecondID_Write_back(u8 *buffer, u8 Len)
{
    u8 x,k;
	 u8 y=0;
	u16 datatem;
	capDataflag=0;

		datatem=buffer[11]*256+buffer[12];
    for (k=0;k<2;k++){
	  write_byte(0,0,0xC000+64+k,buffer[11+k]);//相应配置对应变送器配置类型   读取存储的相应信息
	  delay_ms(3);	    
	}
		for(x=0;x<16;x++) { 
         if((datatem&0x8000)==0x8000){
	  write_byte(0,0,0xC000+66+x,buffer[13+y]);//相应配置对应变送器配置类型   读取存储的相应信息
	  delay_ms(3);	
				 y++;      }
          datatem=datatem<<1;
                    }
		
   Write_back( buffer, Len );
}

 /**********************************************************
 *
 * @file	: ExistSecondID_Write_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 设置实际挂载数量及返回信息
 *
 **********************************************************/
void ExistSecondID_Write_back(u8 *buffer, u8 Len)
{
    u8 x= 0;
	sdDataflag=0;
		for(x=0;x<2;x++) { 
		           write_byte(0,0,0xC000+82+x,buffer[11+x]);	//擦写挂载设备数量								
					     delay_ms(3);
                      }
   Write_back( buffer, Len );
}


/**********************************************************
 *
 * @file	: Restoresetting_Write_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 恢复出厂设置及返回信息
 *
 **********************************************************/
void Restoresetting_Write_back(u8 *buffer, u8 Len)
{
	u8 k= 0;
	for(k=0;k<22;k++){
	write_byte(0,0,0x0102+k,0x35);	//擦写出厂设置的地址位							
	delay_ms(3);}
	Write_back( buffer, Len );
	write_byte(0,0,0xC022,0x03);
	delay_ms(3);
	SoftReset();	
}
				


/**********************************************************
 *
 * @file	: DeviceCon_Read_back()
 * @param	: u8 *buffer: 指向原始数据的u8数据类型指针
 *			  u8 Len: 原始数据长度
 * @retval	: None
 * @brief	: 读设备使能控制
 *
 **********************************************************/
void DeviceCon_Read_back(u8 *buffer, u8 Len)
{
u8 i= 0;
	
	u16 temp;
 
	uart4_buff.tx_len = 0x10;
	
	for (i=0;i<14;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//正常
	//if(buffer[10]==0x11){
	uart4_buff.tx_data_buff[13]=read_random(0,0,0xC000+35);   //读存储的设备使能状态
	delay_ms(3);
	//				    }
   // else {
//	uart4_buff.tx_data_buff[12]=read_random(0,0,35);
//	}
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff); 		//CRC校验低位
	
	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
	}


/*********************************************END OF FILE**********************/
