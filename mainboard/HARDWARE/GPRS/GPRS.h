#ifndef __GPRS_H
#define	__GPRS_H
#include "sys.h" 

void UART4_Rx_err(void);				//UART4接收Modbus帧帧头(地址码)函数
void mem_copy_byte(u8 *source, u8 *direct, u8 length);	//数组拷贝函数
void USART2_Command(void);			//上位机发送的帧数据解析及回应函数
u16 CRC16(u8 *buffer, u8 usLen);			//CRC16校验函数
void Comeback_err(u8 *buffer, u8 Len,u8 comend_ack);	//帧数据错误返回函数
void ID_Read_back(u8 *buffer, u8 Len);	//读设备ID返回函数
void Transset_Read_back(u8 *buffer, u8 Len);	//读软件版本返回函数
void Transcon_Read_back(u8 *buffer, u8 Len);	//读设备波特率返回函数
void Relayopen_Read_back(u8 *buffer, u8 Len);		//读设备IP地址返回函数
void Alarm_Read_back(u8 *buffer, u8 Len);//读气体浓度警告值或报警值返回函数
void Transadjust_Read_back(u8 *buffer, u8 Len);//读采样周期返回函数

void Systemset_Read_back(u8 *buffer, u8 Len);	//读环境温度湿度返回函数
void Transdata_Read_back(u8 *buffer, u8 Len);			//读设备地址码返回函数
void Write_back(u8 *buffer, u8 Len);			//写命令字帧数据正确返回函数
void ID_Write_back(u8 *buffer, u8 Len);		//写设备ID返回函数
void Transset_Write_back(u8 *buffer, u8 Len);	//写设备波特率返回函数
void Transcon_Write_back(u8 *buffer, u8 Len);				//写设备IP地址返回函数
void Alarm_Write_back(u8 *buffer, u8 Len);		//写气体浓度警告值或报警值返回函数
void Relayopen_Write_back(u8 *buffer, u8 Len);		//读继电器固定时间开启无门限
void Transadjust_Write_back(u8 *buffer, u8 Len);	//写气体标定值返回函数
void Systime_Write_back(u8 *buffer, u8 Len);		//设置系统时间返回函数
void Systemset_Write_back(u8 *buffer, u8 Len);		//写设备地址码返回函数
void Transzhun_Write_back(u8 *buffer, u8 Len);	 //设置设备通信方式返回函数

void Relaydata_Read_back(u8 *buffer, u8 Len);				//历史数据查询处理
void Relaycon_Write_back(u8 *buffer, u8 Len);
void Handshake_Read_back(u8 *buffer, u8 Len);		//读设备是否连接正常函数
void DeviceHardware_Read_back(u8 *buffer, u8 Len);		//读设备软硬件版本
void heartbeat(void);
void AllData_Read_back(u8 *buffer, u8 Len);

void Secondver_Read_back(u8 *buffer, u8 Len);


void SysEnable_Read_back(u8 *buffer, u8 Len);
void SysEnable_Write_back(u8 *buffer, u8 Len);
void TransAlarm_Read_back(u8 *buffer, u8 Len);
void TransAlarm_Write_back(u8 *buffer, u8 Len);
void RelayAlarm_Write_back(u8 *buffer, u8 Len);
void RelayAlarm_Read_back(u8 *buffer, u8 Len);
void Relayset_Read_back(u8 *buffer, u8 Len);
void RelayFixEnable_Read_back(u8 *buffer, u8 Len);
 void RelayFixEnable_Write_back(u8 *buffer, u8 Len);

void Salt_Read_back(u8 *buffer, u8 Len);   //读取盐度补偿
void Salt_Write_back(u8 *buffer, u8 Len);   //写取盐度补偿
void Relayopengate_Read_back(u8 *buffer, u8 Len);//读继电器固定时间开启有门限
void Relayopengate_Write_back(u8 *buffer, u8 Len);//写继电器固定时间开启有门限
void DeviceCon_Write_back(u8 *buffer, u8 Len);   //设备使能控制
void Intelligence_Write_back(u8 *buffer, u8 Len);  //智能控制
void SecondID_Write_back(u8 *buffer, u8 Len);// 二级设备ID设置
void SetmaxSecondID_Write_back(u8 *buffer, u8 Len);// 设置最大挂载数量
void Restoresetting_Write_back(u8 *buffer, u8 Len);//恢复出厂设置
void Relaystate_Read_back(u8 *buffer, u8 Len);	
void DeviceCon_Read_back(u8 *buffer, u8 Len);	//读设备使能返回函数
void Intelligence_Read_back(u8 *buffer, u8 Len);	//读智能控制
void SetmaxSecondID_Read_back(u8 *buffer, u8 Len);	//读支持容量
void Reset_Write_back(u8 *buffer, u8 Len);	//重启控制
void ExistSecondID_Read_back(u8 *buffer, u8 Len);		  //读实际容量
void ExistSecondID_Write_back(u8 *buffer, u8 Len);		  //写实际容量
void Transpump_Read_back(u8 *buffer, u8 Len);
void DeviceHardware_Write_back(u8 *buffer, u8 Len);
void Transpump_Write_back(u8 *buffer, u8 Len);
void PathwayName_Write_back(u8 *buffer, u8 Len);
void DeviceName_Write_back(u8 *buffer, u8 Len);
void Relayset_Write_back(u8 *buffer, u8 Len);



void SoftReset(void) ;
void GetLockCode(void);
#endif
