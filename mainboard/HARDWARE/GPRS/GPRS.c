#include "GPRS.h"
#include "dataprocess.h"
#include "delay.h"
#include "24cxx.h"
#include "rtc.h"
#include "timer.h"
#include "init.h"
#include "includes.h"
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
extern u8 StartFlag;				//��ʼ��־
extern u8 StopFlag;				//ֹͣ��־

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

extern u8 setflag;			//���������ñ�־
extern u8 openfixflag ;			//�̵����̶��������������ޣ����ñ�־
extern u8 openunfixflag;			//�̵����̶��������������ޣ����ñ�־
extern u8 fixedflag ;			//�̶�ʱ��У�����ñ�־

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
//CRC16�������λ�ֽڱ�
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

//CRC16�������λ�ֽڱ�
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
{  __set_FAULTMASK(1);      // �ر������ж�
 NVIC_SystemReset();// ��λ
 
}
  
void GetLockCode(void)  
{  
//��ȡCPUΨһID  
CpuID[0]=*(vu32*)(0x1ffff7e8);  
CpuID[1]=*(vu32*)(0x1ffff7ec);  
CpuID[2]=*(vu32*)(0x1ffff7f0);  
//�����㷨,�ܼ򵥵ļ����㷨  
Lock_Code=(CpuID[0]>>1)+(CpuID[1]>>2)+(CpuID[2]>>3);  
} 

/**********************************************************
 *
 * @file	: mem_copy_byte()
 * @param	: u8 *source: ԭʼ����
 *			  u8 *direct: Ŀ������
 *       	  u8 length: Ҫ���������ݳ���
 * @retval	: None
 * @brief	: ��ԭʼ���鿽��length�����ȵ����ݵ�Ŀ��������
 *
 **********************************************************/
void mem_copy_byte(u8 *source, u8 *direct, u8 length)//�����ַ�������
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
 * @brief	: ��λ�����͵�֡���ݽ�������Ӧ
 *
 **********************************************************/
void USART2_Command(void)
{
	u16 temp;
	u8 crc_datah, crc_datal;
	u8 comend_Len, comend_Buff[100]; 	//Modbus֡����BUFF

	//if (UART5RxFlag==1)		 	//Modbus֡һ֡���ݽ�����ɣ����������ֽ���
	{
		comend_Len = uart4_buff.rx_len;
		mem_copy_byte((u8 *)uart4_buff.rx_data_buff, (u8 *)comend_Buff, comend_Len);//��������

		//UART5RxFlag = 0x00;	//��һ֡���ݻ�����ɣ����������һ֡����

		//֡�豸ID���ݽ���
		if ((comend_Buff[0] !=device_id.year_datah)||(comend_Buff[1] !=device_id.year_datal)||
		(comend_Buff[2] !=device_id.sn_datah)||(comend_Buff[3] !=device_id.sn_datal))
		//if (comend_Buff[0] != device_address)	//������Ǳ����豸��ַ�룬������֡����
		{
			Comeback_err((u8 *)comend_Buff, comend_Len,GPRS_ERROR_ID);
			return ;
		}
    //֡���ݳ��Ƚ���
		if (comend_Buff[6] != (comend_Len-7))	//������ݳ��Ȳ�ƥ�䣬������֡����
		{
			Comeback_err((u8 *)comend_Buff, comend_Len,GPRS_ERROR_LENGTH);
			return ;
		}
		//	if ((!online)&&(comend_Buff[7]!=0x0E))	//��������ߣ�������֡����
		//{
		//	return ;
		//}
			if ((!enFlag)&&(comend_Buff[7]!=0x0E)&&(comend_Buff[7]!=0x10))	//������ã�������֡����
		{
		   Comeback_err((u8 *)comend_Buff, comend_Len,GPRS_ERROR_Disable);
			return ;
		}
		//CRCУ��֡����	
		temp = CRC16((u8 *)comend_Buff, (comend_Len-2));
		crc_datah = (u8)((temp>>8)&0x00ff);
		crc_datal = (u8)(temp&0x00ff);
		
		if ((comend_Buff[comend_Len-2] != crc_datah)||(comend_Buff[comend_Len-1] != crc_datal))
		{	//CRC��������򷵻ش�����Ϣ
			Comeback_err((u8 *)comend_Buff, comend_Len,GPRS_ERROR_CRC);
		return ;	
		}
	 
		//�����ּ�飬�������ִ����򷵻ش�����Ϣ
		if ((comend_Buff[7]<0x01)||(comend_Buff[7]>0x20))
		{
			Comeback_err((u8 *)comend_Buff, comend_Len,GPRS_ERROR_CMD);
			return ;	
		}	 
		switch(comend_Buff[4]){
				case 0x03:		//��������
				switch(comend_Buff[7])
				{
					case 0x01:		//���豸ID
					  ID_Read_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x02:		//���豸���к�
						DeviceHardware_Read_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x03:		//������������
					  Transset_Read_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x05:		//����������	
					  Alarm_Read_back((u8 *)comend_Buff, comend_Len);
						break;
					
					case 0x07:		//���ζȲ���
					  Salt_Read_back((u8 *)comend_Buff, comend_Len);
						break;
						 
					case 0x0D:		//��ϵͳ����
					  Systemset_Read_back((u8 *)comend_Buff, comend_Len);
						break;

		 			case 0x0E:		//��������
						Handshake_Read_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x10:		//���豸ʹ��
					  DeviceCon_Read_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x11:		//��ȡ����������
						SetmaxSecondID_Read_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x13:		//������������
					  Transdata_Read_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x15:		//��ȡʵ�ʹ�������
						ExistSecondID_Read_back((u8 *)comend_Buff, comend_Len);
						break;
					
					case 0x19:		//������������ʹ��
						TransAlarm_Read_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x1E:		//��������ˮ��
						Transpump_Read_back((u8 *)comend_Buff, comend_Len);
						break;
					case 0x04:		//����������������
					  Transcon_Read_back((u8 *)comend_Buff, comend_Len);
						break;

					//case 0x05:		//���̵����̶������������ޣ�
					//  Relayopen_Read_back((u8 *)comend_Buff, comend_Len);
					//	break;

				//	case 0x06:		//���������̶�ʱ��У��
					//  Transadjust_Read_back((u8 *)comend_Buff, comend_Len);
				//		break;

					
					case 0x12:		//���̵����������
					  Relaydata_Read_back((u8 *)comend_Buff, comend_Len);
					  break;
					
					//case 0x5A:		//��ȫ������
					//	AllData_Read_back((u8 *)comend_Buff, comend_Len);
					//	break;
					
					
					case 0x06:		//���̵����̶���������
					  Relayopengate_Read_back((u8 *)comend_Buff, comend_Len);
						break;
					
					case 0x0C:		//���̵����ն�״̬
					  Relaystate_Read_back((u8 *)comend_Buff, comend_Len);
						break;
					
					case 0x16:		//�����豸�汾
						Secondver_Read_back((u8 *)comend_Buff, comend_Len);
						break;
					
					case 0x0f:		//�����ܿ���
					  Intelligence_Read_back((u8 *)comend_Buff, comend_Len);
						break;
					
					case 0x1a:		//���̵�������ʹ��
						RelayAlarm_Read_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x1b:		//���̵�������
					  Relayset_Read_back((u8 *)comend_Buff, comend_Len);
						break;
					
					case 0x1c:		//���̵����̶�ʱ���������ʹ��
						RelayFixEnable_Read_back((u8 *)comend_Buff, comend_Len);
						break;
			//	case 0x14:		//��ϵͳʹ��
					//	SysEnable_Read_back((u8 *)comend_Buff, comend_Len);
					//	break;
					default: 
						break;
					}
						break;
						//д������
				  case 0x16:		//д������
				switch(comend_Buff[7]){

					case 0x01:		//д�豸ID
						ID_Write_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x02:		//д�豸���к�
						DeviceHardware_Write_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x03:		//д����������
						Transset_Write_back((u8 *)comend_Buff, comend_Len);
						break;
					
					case 0x05:		//д��������������
						Alarm_Write_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x07:        //д�ζȲ�������
				        Salt_Write_back((u8 *)comend_Buff, comend_Len);
					    break;

					case 0x08:		//���ñ�����У׼
						Transzhun_Write_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x0a:		//����ϵͳʱ��
						Systime_Write_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x0D:		//дϵͳ����
						Systemset_Write_back((u8 *)comend_Buff, comend_Len);
						break; 
						 
					case 0x10:		//�豸ʹ�ܿ���
					  DeviceCon_Write_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x11:		//��������������
						SetmaxSecondID_Write_back((u8 *)comend_Buff, comend_Len);
						break;
								
					case 0x14:		//�ָ���������
						Restoresetting_Write_back((u8 *)comend_Buff, comend_Len);
						break;
					
					case 0x15:		//���ù�������
						ExistSecondID_Write_back((u8 *)comend_Buff, comend_Len);
						break;
					
					case 0x19:		//д����������ʹ��
						TransAlarm_Write_back((u8 *)comend_Buff, comend_Len);
						break;
					
					case 0x1D:		//д�豸����
						DeviceName_Write_back((u8 *)comend_Buff, comend_Len);
						break;
						
					case 0x1E:		//д������ˮ�ÿ���
						Transpump_Write_back((u8 *)comend_Buff, comend_Len);
						break;

				   	case 0x1F:		//дͨ������
						PathwayName_Write_back((u8 *)comend_Buff, comend_Len);
						break;
					 case 0x04:		//д��������������
						Transcon_Write_back((u8 *)comend_Buff, comend_Len);
						break;
				//	case 0x06:		//д�̵����̶������������ޣ�
				//		Relayopen_Write_back((u8 *)comend_Buff, comend_Len);
				//		break;
				//	case 0x08:		//д������У��ʱ��
					//	Transadjust_Write_back((u8 *)comend_Buff, comend_Len);
					//	break;
					case 0x0B:		//д�̵����ֶ�����
						 Relaycon_Write_back((u8 *)comend_Buff, comend_Len);
						break;

					case 0x06:		//д�̵����̶���������
					  Relayopengate_Write_back((u8 *)comend_Buff, comend_Len);
						break;
					
				//	case 0x10:		//��������
					//	Reset_Write_back((u8 *)comend_Buff, comend_Len);
					//	break;
					
					case 0x0f:		//���ܿ��ƿ���
						Intelligence_Write_back((u8 *)comend_Buff, comend_Len);
						break;
					
					case 0x1B:		//д�̵�������
					  Relayset_Write_back((u8 *)comend_Buff, comend_Len);
						break;
				//	case 0x12:		//����ID����
				//		SecondID_Write_back((u8 *)comend_Buff, comend_Len);
				//		break;
							
				//	case 0x13:		//����ϵͳʹ��
				//		SysEnable_Write_back((u8 *)comend_Buff, comend_Len);
				//		break;
				
					case 0x1A:		//д�̵�������ʹ��
						RelayAlarm_Write_back((u8 *)comend_Buff, comend_Len);
						break;
					case 0x1c:		//д�̵����̶�ʱ���������ʹ��
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
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: ���̵����̶�ʱ���������ʹ�ܵķ�����Ϣ
 *
 **********************************************************/
 void RelayFixEnable_Read_back(u8 *buffer, u8 Len)
{
	u8 i,k,n= 0;
	
	u16 temp;
	//u8 datatem;//����������
	uart4_buff.tx_len =20;
//	datatem=second_device.trans_data;
	for (i=0;i<11;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//����
	n=buffer[11]-1;
		              for (k=0;k<6;k++){
		                uart4_buff.tx_data_buff[i++] = read_random(0,0,(366*n+102+k));//��Ӧ���ö�Ӧ���������ܿ���   ��ȡ�洢����Ӧ������Ϣ
	                  delay_ms(3);
		                               }
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRCУ���λ
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRCУ���λ
	
USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}
/**********************************************************
 *
 * @file	: Systemset_Write_back()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: д�̵����̶�ʱ���������ʹ�ܼ�������Ϣ
 *
 **********************************************************/
void RelayFixEnable_Write_back(u8 *buffer, u8 Len)
{
u8 n= 0;
u8 x= 0;	
//	u16 temp;
//	u8 datatrans;//����������
//	u16 datatem;
	//alaDataflag=0;  
	n=buffer[11]-1;
	relaygateENflag[n]=0;
		for(x=0;x<6;x++) { 
	  write_byte(0,0,366*n+102+x,buffer[12+x]);//��Ӧ���ö�Ӧ��������������   ��ȡ�洢����Ӧ��Ϣ
	  delay_ms(3);
                    }
	Write_back( buffer, Len );
} 
		/**********************************************************
 *
 * @file	: Relayset_Read_back()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: ���̵������õķ�����Ϣ
 *
 **********************************************************/
 void Relayset_Read_back(u8 *buffer, u8 Len)
{
	u8 i,k,n= 0;
	u8 j= 0;
	u16 temp;
	//u16 datatem;
//	u8 datatem;//����������
	//uart4_buff.tx_len =15;

	for (i=0;i<11;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//����
	if(buffer[11]>second_device.relay_data){
	Comeback_err((u8 *)buffer, Len,RELAY_NOT_EXIST);}
	else {
		if(buffer[11]){
			uart4_buff.tx_data_buff[i++] = buffer[11];
	for (j=0;j<buffer[11];j++){
	 n=buffer[12+j]-1;	   //ID
		uart4_buff.tx_data_buff[i++] = buffer[12+j];
    for (k=0;k<3;k++){
	  uart4_buff.tx_data_buff[i++] = read_random(0,0,(366*n+k+67));//��Ӧ���ö�Ӧ��������������   ��ȡ�洢����Ӧ��Ϣ
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
	  uart4_buff.tx_data_buff[i++] = read_random(0,0,(366*n+k+67));//��Ӧ���ö�Ӧ��������������   ��ȡ�洢����Ӧ��Ϣ
	  delay_ms(3);	    
	                     }
	                                        }
	   uart4_buff.tx_len =4*second_device.relay_data+14;
	   uart4_buff.tx_data_buff[6]=4*second_device.relay_data+7;
	      }
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRCУ���λ
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRCУ���λ
	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);}
}
	/**********************************************************
 *
 * @file	: Intelligence_Read_back()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: ���̵����ն˱���ʹ�ܵķ�����Ϣ
 *
 **********************************************************/
 void RelayAlarm_Read_back(u8 *buffer, u8 Len)
{
	u8 i,k,n= 0;
	
	u16 temp;
//	u8 datatem;//����������
	uart4_buff.tx_len =20;
//	datatem=second_device.trans_data;
	for (i=0;i<11;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//����
	n=buffer[11]-1;
	if(buffer[11]>second_device.relay_data){
	Comeback_err((u8 *)buffer, Len,TRANS_NOT_EXIST);}
	else {
		              for (k=0;k<6;k++){
		                uart4_buff.tx_data_buff[i++] = read_random(0,0,(366*n+61+k));//��Ӧ���ö�Ӧ���������ܿ���   ��ȡ�洢����Ӧ������Ϣ
	                  delay_ms(3);
		                  }
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRCУ���λ
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRCУ���λ
	
	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}
	}
/**********************************************************
 *
 * @file	: Systemset_Write_back()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: д�̵����ն˱���ʹ�ܼ�������Ϣ
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
	  write_byte(0,0,366*n+61+x,buffer[12+x]);//��Ӧ���ö�Ӧ��������������   ��ȡ�洢����Ӧ��Ϣ
	  delay_ms(3);
                    }
	Write_back( buffer, Len );
} 

/**********************************************************
 *
 * @file	: TransAlarm_Read_back()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: ���������ն˱���ʹ�ܵķ�����Ϣ
 *
 **********************************************************/
 void TransAlarm_Read_back(u8 *buffer, u8 Len)
{
	u8 i,k,n= 0;
	
	u16 temp;
//	u8 datatem;//����������
	uart4_buff.tx_len =20;
//	datatem=second_device.trans_data;
	for (i=0;i<12;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//����
	uart4_buff.tx_data_buff[6] = 0x0D;		//����
	if(buffer[11]>second_device.trans_data){
	Comeback_err((u8 *)buffer, Len,TRANS_NOT_EXIST);}
	else{
	n=buffer[11]-1;
		              for (k=0;k<6;k++){
		                uart4_buff.tx_data_buff[i++] = read_random(0,1,(708*n+690+k));//��Ӧ���ö�Ӧ���������ܿ���   ��ȡ�洢����Ӧ������Ϣ
	                  delay_ms(3);
		                  }
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRCУ���λ
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRCУ���λ
	
	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}
   }
   /**********************************************************
 *
 * @file	: Transpump_Read_back()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: ��������ˮ�õķ�����Ϣ
 *
 **********************************************************/
 void Transpump_Read_back(u8 *buffer, u8 Len)
{
	u8 i,k,n= 0;
	
	u16 temp;
//	u8 datatem;//����������
	//uart4_buff.tx_len =26;
//	datatem=second_device.trans_data;
	for (i=0;i<12;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//����
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
		                //uart4_buff.tx_data_buff[i++] = read_random(0,1,(708*n+698+k));//��Ӧ���ö�Ӧ���������ܿ���   ��ȡ�洢����Ӧ������Ϣ
	                  uart4_buff.tx_data_buff[i++] =transpump_En[n].dataflagh[k];
										uart4_buff.tx_data_buff[i++] =transpump_En[n].dataflagl[k];
										//delay_ms(3);
									                  }
		                      }
	else {uart4_buff.tx_len =16;
        uart4_buff.tx_data_buff[6] =9;	}						
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRCУ���λ
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRCУ���λ
	
	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}
   }
/**********************************************************
 *
 * @file	: Systemset_Write_back()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: д�������ն˱���ʹ�ܼ�������Ϣ
 *
 **********************************************************/
void TransAlarm_Write_back(u8 *buffer, u8 Len)
{
u8 n= 0;
u8 x= 0;	
	//u16 temp;
	//u8 datatrans;//����������
	//u16 datatem;
	//alaDataflag=0;
	n=buffer[11]-1;
	transalarmENflag[n]=0;
		for(x=0;x<6;x++) { 
	  write_byte(0,1,708*n+691+x,buffer[12+x]);//��Ӧ���ö�Ӧ��������������   ��ȡ�洢����Ӧ��Ϣ
	  delay_ms(3);
                    }
	Write_back( buffer, Len );
} 

/**********************************************************
 *
 * @file	: DeviceName_Write_back()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: д�豸���Ƽ�������Ϣ
 *
 **********************************************************/
void DeviceName_Write_back(u8 *buffer, u8 Len)
{
u8 k,m,n;
//u8 y,x;
//u8 i,j=0;
u8 t=0;	
//	u16 temp;
	u8 datatrans;//����������
	u8 datarelay;
//	u8 sdata[2];
//	u16 datatem;
	datatrans=second_device.trans_data;
	datarelay=second_device.relay_data;
	//setDataflag=0;
  //	n=buffer[10]-1;	   //ID
	//datatem=buffer[11]*256+buffer[12];

	switch(buffer[11]){
					case 0x52:		//ɾ������������
						switch(buffer[13]){
						case 0x11:
						    for (k=0;k<20;k++){
								write_byte(0,0,32000+90+k,0x00);
	                            delay_ms(3);         //��д�洢�������ĵ�ַ
		                               }
						    break;
						case 0x21:
						if(!buffer[12]){
						   for (m=0;m<datatrans;m++)
	                               {
	              	        for (k=0;k<10;k++){
								write_byte(0,0,32000+10*m+110+k,0x00);
	                            delay_ms(3);         //��д�洢�������ĵ�ַ
		                                       }
	                               }
									   }
						else{
						 for (m=0;m<buffer[12];m++)
	                    {
		                   for (n=1;n<(datatrans+1);n++)      {
			                 if(buffer[14+m]==n){
		                       for (k=0;k<10;k++){
		                       //��д�洢�������ĵ�ַ
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
	                            delay_ms(3);         //��д�洢�������ĵ�ַ
		                                      }
	                               }
								       }
						else {
							 for (m=0;m<buffer[12];m++)
	                    {
		                   for (n=1;n<(datarelay+1);n++)      {
			                 if(buffer[14+m]==n){
		                       for (k=0;k<10;k++){
		                       //��д�洢�������ĵ�ַ
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
					
					case 0x51:		//����޸ı���������
						  switch(buffer[13]){
						case 0x11:
						    for (k=0;k<buffer[14]+1;k++){
								write_byte(0,0,90+k,buffer[14+k]);
	                            delay_ms(3);         //��д�洢�������ĵ�ַ
		                               }
						    break;
						case 0x21:
						 for (m=0;m<buffer[12];m++)
	                    {
		                   for (n=1;n<(datatrans+1);n++)      {
			                 if(buffer[14+t]==n){
		                       for (k=0;k<buffer[15+t]+1;k++){
		                       //��д�洢�������ĵ�ַ
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
		                       //��д�洢�������ĵ�ַ
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
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: дͨ�����Ƽ�������Ϣ
 *
 **********************************************************/
void PathwayName_Write_back(u8 *buffer, u8 Len)
{
u8 k,m,n;
u8 x;
u8 i,j=0;
u8 t=0;	
//	u16 temp;
	u8 datatrans;//����������
	u8 sdata[2];
	u16 datatem;
	datatrans=second_device.relay_data;
	//setDataflag=0;
  //	n=buffer[10]-1;	   //ID
	//datatem=buffer[11]*256+buffer[12];

	switch(buffer[11]){
					case 0x52:		//ɾ������������
					    if(!buffer[12]){
						for (m=0;m<datatrans;m++)
	                               {
	              	for (k=1;k<3;k++){
								write_byte(0,1,708*m+k,0x00);
	                            delay_ms(3);         //��д�洢�������ĵ�ַ
		                               }
	                               }
						               }

						else {
						for (m=0;m<buffer[12];m++)
	                    {
		             for (n=1;n<(datatrans+1);n++)      {
			                 if(buffer[13+m]==n){
		                       for (k=1;k<3;k++){
		                       //��д�洢�������ĵ�ַ
													 write_byte(0,1,708*m+k,0x00);
													 delay_ms(3);
		                                        }   
			                          break;     
                     						          }
										                            }
	                     }
						}									
						break;
					
					case 0x50:		//����޸ı���������
						  for (m=0;m<buffer[12];m++)
	                    {           //�ж������˼��ִ�����̽ͷ
		                   for (n=1;n<81;n++)      {
			                      if(buffer[13+i]==n){         //IDƥ��
										for (k=0;k<2;k++){
										     sdata[k]=read_random(0,1,708*(n-1)+k);
											 delay_ms(3);
											 sdata[k] |=buffer[14+i+k];
											             }
										for (k=0;k<2;k++){				  //д�ն�����
												write_byte(0,1,k+701*(n-1)+1,sdata[k]);
												delay_ms(3);
		                                                   }
													datatem=buffer[14+i]*256+buffer[15+i];				  
												    for(x=0;x<4;x++) { 
                                       if((datatem&0x8000)==0x8000){           //�жϴ�����̽ͷ����  д����Ӧ���͵�����
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
						     datatem=buffer[14+i]*256+buffer[15+i];//��һ������������״̬
							for(x=0;x<4;x++) { 
                            if((datatem&0x8000)==0x8000){j++;} 
                            datatem=datatem<<1; 
                                         } 
										        i+=2*j+3;                   //�ж������˼��ִ�����̽ͷ

	                     } 				
						break;
					
					case 0x51:		//���Ǳ���������
						   for (m=0;m<buffer[12];m++)
	                    {           //�ж������˼��ִ�����̽ͷ
		                   for (n=1;n<datatrans+1;n++)      {
			                      if(buffer[13+i]==n){         //IDƥ��
										for (k=0;k<2;k++){				  //д�ն�����
												write_byte(0,1,k+701*(n-1)+1,buffer[14+i+k]);
												delay_ms(3);
		                                                   }
													datatem=buffer[14+i]*256+buffer[15+i];				  
												    for(x=0;x<4;x++) { 
                                       if((datatem&0x8000)==0x8000){           //�жϴ�����̽ͷ����  д����Ӧ���͵�����
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
						     datatem=buffer[14+i]*256+buffer[15+i];//��һ������������״̬
							for(x=0;x<4;x++) { 
                            if((datatem&0x8000)==0x8000){j++;} 
                            datatem=datatem<<1; 
                                         } 
										        i+=2*j+3;                   //�ж������˼��ִ�����̽ͷ

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
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: д������ˮ�ÿ��Ƽ�������Ϣ
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
	  write_byte(0,1,708*n+696+x,buffer[12+x]);//��Ӧ���ö�Ӧ��������������   ��ȡ�洢����Ӧ��Ϣ
	  delay_ms(3);
                    }
		temp=buffer[14]*256+buffer[15];
	//pump_write(buffer[11],buffer[12],buffer[13],buffer[14],buffer[15]);
	if((temp&0x4000)==0x4000){
	       PBout(6)=0;                  //��ˮ��
	                          }
	else PBout(6)=1; 
	Write_back( buffer, Len );
} 
/**********************************************************
 *
 * @file	: Intelligence_Read_back()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: ��ϵͳ����ʹ�ܵķ�����Ϣ
 *
 **********************************************************/
 void SysEnable_Read_back(u8 *buffer, u8 Len)
{
	u8 i,k= 0;
	
	u16 temp;
//	u8 datatem;//����������
	uart4_buff.tx_len =40;
//	datatem=second_device.trans_data;
	for (i=0;i<10;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//����
		              for (k=0;k<28;k++){
		                uart4_buff.tx_data_buff[i++] = read_random(0,0,(36+k));//��Ӧ���ö�Ӧ���������ܿ���   ��ȡ�洢����Ӧ������Ϣ
	                  delay_ms(3);
		                  }
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRCУ���λ
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRCУ���λ
	
	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}
/**********************************************************
 *
 * @file	: Systemset_Write_back()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: дϵͳ����ʹ�ܼ�������Ϣ
 *
 **********************************************************/
void SysEnable_Write_back(u8 *buffer, u8 Len)
{
u8 x= 0;	
//	u16 temp;
//	u8 datatrans;//����������
//	u16 datatem;
	//alaDataflag=0;
		for(x=0;x<28;x++) { 
	  write_byte(0,0,36+x,buffer[10+x]);//��Ӧ���ö�Ӧ��������������   ��ȡ�洢����Ӧ��Ϣ
	  delay_ms(3);
                    }
	Write_back( buffer, Len );
} 
/**********************************************************
 *
 * @file	: Intelligence_Read_back()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: �����������ܿ��Ƶķ�����Ϣ
 *
 **********************************************************/
void Intelligence_Read_back(u8 *buffer, u8 Len)
{
	u8 i,k= 0;
	
	u16 temp;
	u8 datatem;//����������
	uart4_buff.tx_len =16;
	datatem=second_device.trans_data;
	for (i=0;i<12;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//����
	datatem=buffer[11]-1;	
		              for (k=0;k<2;k++){
		                uart4_buff.tx_data_buff[i++] = read_random(0,1,(708*datatem+590+k));//��Ӧ���ö�Ӧ���������ܿ���   ��ȡ�洢����Ӧ������Ϣ
	                  delay_ms(3);
		                  }
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRCУ���λ
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRCУ���λ
	
	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}


/**********************************************************
 *
 * @file	: SetmaxSecondID_Read_back()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: ��������������������Ϣ
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
	  uart4_buff.tx_data_buff[i++] = read_random(0,0,0xC000+66+x);//��Ӧ���ö�Ӧ��������������   ��ȡ�洢����Ӧ��Ϣ
	  delay_ms(3);	    
				 y++;                    }
          datatem=datatem<<1;
                    }
	uart4_buff.tx_data_buff[8] = 0xd0;		//����
	uart4_buff.tx_len = 15+y;	
  uart4_buff.tx_data_buff[6] = 8+y;		//����										
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRCУ���λ
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff); 		//CRCУ���λ
	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}
/**********************************************************
 *
 * @file	: ExistSecondID_Read_back()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: ��ʵ�ʹ���������������Ϣ
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
	uart4_buff.tx_data_buff[8] = 0xd0;		//����
		for(x=0;x<2;x++) { 
						uart4_buff.tx_data_buff[i++] = read_random(0,0,0xC000+x+82);	//��ȡ��Ӧ�Ĺ��ز���							
					       delay_ms(3);
                      }							
	temp = CRC16( (u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRCУ���λ
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff); 		//CRCУ���λ
	
	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}
/**********************************************************
 *
 * @file	: CRC16()
 * @param	: u8 *buffer: ָ��Ҫ����CRC16У�����ݵ�ָ��
 *			  u8 usLen: Ҫ����CRC16У������ݳ���
 * @retval	: CRC16У��Ľ��
 * @brief	: ��usLen�����ȵ����ݽ���CRC16У��
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
	usInx = ucCRCHi ^ buffer[i++]; /* ����CRC */
    ucCRCHi = ucCRCLo ^ aucCRCHi[usInx] ;
    ucCRCLo = aucCRCLo[usInx] ;
	}

	return (u16)(ucCRCHi << 8 | ucCRCLo);
}

/**********************************************************
 *
 * @file	: Comeback_err()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: ֡���ݴ��󣬷��ش�����Ϣ
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
	uart4_buff.tx_data_buff[4] = buffer[4] + 0x80;			//������
	uart4_buff.tx_data_buff[6] = 5;		//����
	uart4_buff.tx_data_buff[8] = comend_ack;					//Ԥ��1
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRCУ���λ
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRCУ���λ
    USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}

/**********************************************************
 *
 * @file	: Relaydata_Read_back()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: ���̵������״̬���ݵķ�����Ϣ
 *
 **********************************************************/
void Relaydata_Read_back(u8 *buffer, u8 Len)
{
		u8 i,m,n= 0;
	u8 x,j,y= 0;
	u16 temp;
	u16 datatem;//����������
	u8 relays=0;
	u8 datatrans;//����������
	u8 temdata1,temdata2;
	datatrans=second_device.relay_data;
	for (i=0;i<11;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//����  
	temp=0;
	if(buffer[11]){
	    uart4_buff.tx_data_buff[i++] =buffer[11];
		temp++;
		for (y=0;y<buffer[11];y++)
	{
	    n=buffer[12+y]-1;
		
		uart4_buff.tx_data_buff[i++] =buffer[12+y];
		temp++;
		uart4_buff.tx_data_buff[i++] = relay_result_data[n].state;   //�̵����ն�״̬
		temp++;
		if(relay_result_data[n].state!=0xee){
		for(j=0;j<2;j++){
		uart4_buff.tx_data_buff[i++] = read_random(0,0,(366*n+j+68));   //�̵����������
		temp++;
		delay_ms(3);}
	    datatem=uart4_buff.tx_data_buff[i-2]*256+uart4_buff.tx_data_buff[i-1];   //��ȡ���������õĵ�ַ����
		//datatim=trans_query_data[n].flagdata;
		for(x=0;x<16;x++) { 
         if((datatem&0x8000)==0x8000){//�ж������˼��ּ�⹦��
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
		uart4_buff.tx_data_buff[i++] = relay_result_data[m].state;   //�̵����ն�״̬
		temp++;
		temdata1=relay_result_data[n].setData1;
		
	    temdata2=relay_result_data[n].setData2;   //��ȡ�̵������õĵ�ַ����

		 uart4_buff.tx_data_buff[i++] = temdata1;
		 temp++;
		 uart4_buff.tx_data_buff[i++] = temdata2;
		 temp++;
		datatem=temdata1*256+temdata2;
		for(x=0;x<16;x++) { 
         if((datatem&0x8000)==0x8000){//�ж������˼��ּ�⹦��
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
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRCУ���λ
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRCУ���λ

USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
 
	for(n=0;n<datatrans;n++){
		if(relay_result_data[n].state!=0xee){		
	datatem=relay_result_data[n].setData1*256+relay_result_data[n].setData2;		  //�洢��������
			write_byte(0,0,(366*n),relay_result_data[n].state);        
	      delay_ms(3);
	   for(x=0;x<16;x++) { 
         if((datatem&0x8000)==0x8000){//�ж������˼��ּ�⹦��
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
		uart4_buff.tx_data_buff[i++] = read_random(0,1,(708*m+j));   //������
		temp++;
		delay_ms(3);}
		temdata1=read_random(0,1,(708*m+1));
		delay_ms(3);
	    temdata2=read_random(0,1,(708*m+2));   //��ȡ���������õĵ�ַ����
		delay_ms(3);
		datatem=temdata1*256+temdata2;
		uart4_buff.tx_data_buff[i++] = trans_query_data[m].flagdata/256;  //������̽ͷ״̬λ
		temp++;
		uart4_buff.tx_data_buff[i++] = trans_query_data[m].flagdata%256;  
		temp++;
	//	datatim=trans_query_data[m].flagdata;
		for(x=0;x<4;x++) { 
         if((datatem&0x8000)==0x8000){//�ж������˼��ִ�����̽ͷ
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
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: ���̵����ն�״̬�ķ�����Ϣ
 *
 **********************************************************/
void Relaystate_Read_back(u8 *buffer, u8 Len)
{
		u8 i,m,n= 0;
	u8 x,j,y= 0;
	u16 temp;
	u16 datatem;//����������
	u8 relays=0;
	u8 datatrans;//����������
	u8 temdata1,temdata2;
	datatrans=second_device.relay_data;
	for (i=0;i<11;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//����  
	temp=0;
	if(buffer[11]){
	    uart4_buff.tx_data_buff[i++] =buffer[11];
		temp++;
		for (y=0;y<buffer[11];y++)
	{
	    n=buffer[12+y]-1;
		uart4_buff.tx_data_buff[i++] =buffer[12+y];
		temp++;
		uart4_buff.tx_data_buff[i++] = relay_result_data[n].state;   //������
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
		uart4_buff.tx_data_buff[i++] = relay_result_data[m].state;   //������
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
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRCУ���λ
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRCУ���λ

USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);

	  //�洢����
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
		uart4_buff.tx_data_buff[i++] = read_random(0,1,(708*m+j));   //������
		temp++;
		delay_ms(3);}
		temdata1=read_random(0,1,(708*m+1));
		delay_ms(3);
	    temdata2=read_random(0,1,(708*m+2));   //��ȡ���������õĵ�ַ����
		delay_ms(3);
		datatem=temdata1*256+temdata2;
		uart4_buff.tx_data_buff[i++] = trans_query_data[m].flagdata/256;  //������̽ͷ״̬λ
		temp++;
		uart4_buff.tx_data_buff[i++] = trans_query_data[m].flagdata%256;  
		temp++;
	//	datatim=trans_query_data[m].flagdata;
		for(x=0;x<4;x++) { 
         if((datatem&0x8000)==0x8000){//�ж������˼��ִ�����̽ͷ
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
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: �����������õķ�����Ϣ
 *
 **********************************************************/
void Transset_Read_back(u8 *buffer, u8 Len)
{
	u8 i,k,n,j= 0;
	u8 x,y= 0;
	u8 m= 0;
	u16 temp;
	u16 datatem;//����������
//	u8 transdata=second_device.trans_data;
	
	for (i=0;i<12;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//����
	if(buffer[11]>second_device.trans_data){
	Comeback_err((u8 *)buffer, Len,TRANS_NOT_EXIST);}
	else {
	for (j=0;j<buffer[11];j++){
	n=buffer[12+j]-1;	   //ID
		uart4_buff.tx_data_buff[i++] = buffer[12+j];
		m++;
    for (k=1;k<3;k++){
	  uart4_buff.tx_data_buff[i++] = read_random(0,1,(708*n+k));//��Ӧ���ö�Ӧ��������������   ��ȡ�洢����Ӧ��Ϣ
	  delay_ms(3);
   m++;	    
	}
	  datatem=uart4_buff.tx_data_buff[14]+uart4_buff.tx_data_buff[13]*256;
		for(x=0;x<8;x++) { 
         if((datatem&0x8000)==0x8000){
		   for (k=0;k<2;k++){
	  uart4_buff.tx_data_buff[i++] = read_random(0,1,(708*n+3+k+2*x));//��Ӧ���ö�Ӧ��������������   ��ȡ�洢����Ӧ��Ϣ
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
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRCУ���λ
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRCУ���λ
	
	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}
 }
/**********************************************************
 *
 * @file	: Transcon_Read_back()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: ���������������޵ķ�����Ϣ
 *
 **********************************************************/
void Transcon_Read_back(u8 *buffer, u8 Len)
{
	u8 i,k,n= 0;
	u8 x,y= 0;
	u16 temp;
	u16 datatem;//����������
	u8 datatrans;//����������
//	u8 datarelay;//�̵�������
	u8 datatim;//�������ݶ�����
	//datarelay=second_device.trans_data;
	for (i=0;i<12;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//����
	temp=0;
	n=buffer[11]-1;
	datatrans=read_random(0,1,(708*n+35));//�������ն�����
				         delay_ms(3); 
	datatem=datatrans*256+read_random(0,1,(708*n+36));//�������ն�����
				         delay_ms(3); 
		             uart4_buff.tx_data_buff[i++]=datatrans;//				   
		             temp++; 
		             uart4_buff.tx_data_buff[i++]=datatem%256;
		             temp++;
					 
				          for(x=0;x<4;x++) { 
									if((datatem&0x8000)==0x8000){
										uart4_buff.tx_data_buff[i++] = read_random(0,1,(708*n+37+28*x));//��ȡ�������������޵����� �̵���ID
					          delay_ms(3);
					          temp++;
					          uart4_buff.tx_data_buff[i++] = read_random(0,1,(708*n+38+28*x));//��ȡ�������������޵����� �̵���ͨ��
					          delay_ms(3);
					          temp++;
					          uart4_buff.tx_data_buff[i++] = read_random(0,1,(708*n+39+28*x));//��ȡ�������������޵����� �̵���ͨ��
					          delay_ms(3);
					          temp++;
										datatim=read_random(0,1,(708*n+40+28*x));	//�洢���ݶ�����
										delay_ms(3);
										uart4_buff.tx_data_buff[i++] =datatim;
										temp++;
										for(y=0;y<2;y++){
											if((datatim&0x80)==0x80){
												for (k=0;k<6;k++){
												uart4_buff.tx_data_buff[i++] = read_random(0,1,(708*n+41+28*x+6*y+k));//��ȡ�������������޵�����
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
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRCУ���λ
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRCУ���λ
	temp = 0;
USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}

/**********************************************************
 *
 * @file	: Alarm_Read_back()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: ���������޵ķ�����Ϣ
 *
 **********************************************************/
void Alarm_Read_back(u8 *buffer, u8 Len)
{
	u8 i,k,n= 0;
	u8 x= 0;
	u16 temp;
	u16 datatem;//����������
	u8 datatrans;//����������
		u8 datatim;//����������
	//datatrans=second_device.trans_data;
	for (i=0;i<12;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//����
	temp=1;
		if(buffer[11]>second_device.trans_data){
	Comeback_err((u8 *)buffer, Len,TRANS_NOT_EXIST);}
	  else {
	n=buffer[11]-1;
	datatrans=read_random(0,1,(708*n+485));//��������������
				         delay_ms(3); 
	datatim=read_random(0,1,(708*n+486));
	delay_ms(3);
	datatem=datatrans*256+datatim;//��������������
				         
		             uart4_buff.tx_data_buff[i++]=datatrans;//				   
		             temp++; 
		             uart4_buff.tx_data_buff[i++]=datatim;
		             temp++;
				for(x=0;x<4;x++) { 
                            if((datatem&0x8000)==0x8000){
															for (k=0;k<6;k++){
															uart4_buff.tx_data_buff[i++] = read_random(0,1,(708*n+487+6*x+k));//��ȡ��������Ӧ̽ͷ�ı���ֵ
															delay_ms(3);
																	temp++;
		                                            } 
													                      }
                            datatem=datatem<<1; 
                           }
			               
	uart4_buff.tx_len =temp+13;
	uart4_buff.tx_data_buff[6]=temp+6;
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRCУ���λ
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRCУ���λ
	
	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}
	}
/**********************************************************
 *
 * @file	: Relayopen_Read_back()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: ���̵����̶�ʱ�俪������ķ�����Ϣ�������ޣ�
 *
 **********************************************************/
void Relayopen_Read_back(u8 *buffer, u8 Len)
{
	u8 i,m,k,n= 0;
	u8 x,y= 0;
	u16 temp;
	u8 datatem;//�̶�ʱ�俪������
	u8 datatim;//�̶�ʱ�俪������
	u8 datatrans;//����������
	datatrans=second_device.relay_data;
	for (i=0;i<10;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[3] = 0xd0;		//����
	temp=1;	
	if(!buffer[12]){             //��ȡȫ��
	uart4_buff.tx_data_buff[i++]=temp/256;
	uart4_buff.tx_data_buff[i++]=temp%256;
	uart4_buff.tx_data_buff[i++]=datatem;
	for (m=1;m<(datatrans+1);m++)
	{
		 		datatem=read_random(0,1,(483*m+11-483));//��������
				delay_ms(3);
		    uart4_buff.tx_data_buff[i++]=read_random(0,1,(483*m-483));//�̵����ն�ID
				delay_ms(3); 
		    temp++; 
		    uart4_buff.tx_data_buff[i++]=read_random(0,1,(483*m+1-483));//�̵����ն�״̬
				delay_ms(3);
		    temp++; 
		    uart4_buff.tx_data_buff[i++]=datatem;
		    temp++;
				for(x=0;x<4;x++) { 
                            if((datatem&0x80)==0x80){
														datatim=read_random(0,1,(483*m+14+19*x-483));	//�洢��ʱ������
														delay_ms(3);
		                        uart4_buff.tx_data_buff[i++]=read_random(0,1,(483*m+12+19*x-483));//�̵���ͨ��
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
															uart4_buff.tx_data_buff[i++] = read_random(0,1,(483*m+15+19*x+4*y+k-483));//��ȡ��������Ӧ̽ͷ�ı���ֵ
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
							            datatem=read_random(0,1,(483*n+11-483));//��������
				delay_ms(3);
		    uart4_buff.tx_data_buff[i++]=read_random(0,1,(483*n-483));//�̵����ն�ID
				delay_ms(3); 
		    temp++; 
		    uart4_buff.tx_data_buff[i++]=read_random(0,1,(483*n+1-483));//�̵����ն�״̬
				delay_ms(3);
		    temp++; 
		    uart4_buff.tx_data_buff[i++]=datatem;
		    temp++;
								for(x=0;x<4;x++) { 
                            if((datatem&0x80)==0x80){
														datatim=read_random(0,1,(483*n+14+19*x-483));	//�洢��ʱ������
														delay_ms(3);
		                        uart4_buff.tx_data_buff[i++]=read_random(0,1,(483*n+12+19*x-483));//�̵���ͨ��
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
															uart4_buff.tx_data_buff[i++] = read_random(0,1,(483*n+15+19*x+4*y+k-483));//��ȡ��������Ӧ̽ͷ�ı���ֵ
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
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRCУ���λ
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRCУ���λ
	
	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}

/**********************************************************
 *
 * @file	: Relayopen_Read_back()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: ���̵����̶�ʱ�俪������ķ�����Ϣ����
 *
 **********************************************************/
void Relayopengate_Read_back(u8 *buffer, u8 Len)
{
	u8 i,k,n= 0;
	u8 x,y,z= 0;
	u16 temp;
	u16 datatem;//����������
	u8 datatrans;//����������
	//u8 datarelay;//�̵�������
	u8 datatim;//�������ݶ�����
	//datarelay=second_device.trans_data;
	for (i=0;i<12;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//����
	temp=1;
	n=buffer[11]-1;
	datatrans=read_random(0,0,(366*n+108));//�̵����̶���������
				         delay_ms(3); 
	datatem=datatrans*256+read_random(0,0,(366*n+109));//�̵����̶���������
				         delay_ms(3); 
		             uart4_buff.tx_data_buff[i++]=datatrans;//				   
		             temp++; 
		             uart4_buff.tx_data_buff[i++]=datatem%256;
		             temp++;
					 
				          for(x=0;x<8;x++) { 
									if((datatem&0x8000)==0x8000){
											   for(y=0;y<11;y++) {
					          uart4_buff.tx_data_buff[i++] = read_random(0,0,(366*n+110+32*x+y));//��ȡ�������������޵����� �̵���ͨ��
					          delay_ms(3);
					          temp++;                }
										datatim=read_random(0,0,(366*n+121+32*x));	//�洢���ݶ�����
										delay_ms(3);
										uart4_buff.tx_data_buff[i++] =datatim;
										temp++;
										for(z=0;z<4;z++){
											if((datatim&0x80)==0x80){
												for (k=0;k<5;k++){
												uart4_buff.tx_data_buff[i++] = read_random(0,1,(366*n+122+32*x+5*z+k));//��ȡ�������������޵�����
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
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRCУ���λ
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRCУ���λ
	temp = 0;
USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}
/**********************************************************
 *
 * @file	: Transadjust_Read_back()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: ���������̶�ʱ��У���ķ�����Ϣ
 *
 **********************************************************/

/**********************************************************
 *
 * @file	: AllData_Read_back()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: ���������ݵķ�����Ϣ
 *
 **********************************************************/
void AllData_Read_back(u8 *buffer, u8 Len)
{
	u8 i,m,k= 0;
	
	u16 temp;
	u8 datatrans;//�̶�ʱ������
	u8 datareley;
	
	for (i=0;i<10;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[3] = 0xd0;		//����
	
	temp=9*(datatrans+datareley)+2;	
	uart4_buff.tx_data_buff[i++]=temp/256;
	uart4_buff.tx_data_buff[i++]=temp%256;
	uart4_buff.tx_data_buff[i++]=datatrans;
	for (m=0;m<datatrans;m++)
	{
		uart4_buff.tx_data_buff[i++] = m+1;   //������
		for (k=0;k<16;k++){
		uart4_buff.tx_data_buff[i++] = buffer[k];//����������   ��ȡ�洢����Ӧ������Ϣ
		                  }
	}

	uart4_buff.tx_data_buff[i++]=datareley;
	for (m=0;m<datareley;m++)
	{
		uart4_buff.tx_data_buff[i++] = m+1;   //�̵���
		              for (k=0;k<4;k++){
		              uart4_buff.tx_data_buff[i++] = buffer[k];//��Ӧ�̵�������
		                               }   
	}

	uart4_buff.tx_len =uart4_buff.tx_data_buff[10]*256+uart4_buff.tx_data_buff[11]+14;
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRCУ���λ
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRCУ���λ

	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}

/**********************************************************
 *
 * @file	: ID_Read_back()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: ���豸ID
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
	uart4_buff.tx_data_buff[8] = 0xd0;		//����
	uart4_buff.tx_data_buff[i++] =device_id.year_datah;		  //���洢���豸ID
	uart4_buff.tx_data_buff[i++] =device_id.year_datal;
	uart4_buff.tx_data_buff[i++] =device_id.sn_datah;
	uart4_buff.tx_data_buff[i++] =device_id.sn_datal;

	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRCУ���λ
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff); 		//CRCУ���λ
	
	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
	}
/**********************************************************
 *
 * @file	: Systemset_Read_back()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: ��ϵͳ���õķ�����Ϣ
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
	uart4_buff.tx_data_buff[8] = 0xd0;		//���� 
	 datatem=buffer[11]*256+buffer[12];
		for(x=0;x<16;x++) { 				  
         if((datatem&0x8000)==0x8000){
	  uart4_buff.tx_data_buff[i++] = read_random(0,0,0xC000+18+x);	//ϵͳ����								
		delay_ms(3);
				 y++;
				                           }
          datatem=datatem<<1;
                    }
	uart4_buff.tx_data_buff[6] = y+8;
	uart4_buff.tx_len = 15+y;
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRCУ���λ
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff); 		//CRCУ���λ

USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}

/**********************************************************
 *
 * @file	: Transdata_Read_back()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: �����������ݵķ�����Ϣ
 *
 **********************************************************/
void Transdata_Read_back(u8 *buffer, u8 Len)
{
	u8 i,m,n= 0;
	u8 x,j,y= 0;
	u16 temp;
	u16 datatem;//����������
	u8 trans=0;
	u8 datatrans;//����������
	u8 temdata1,temdata2;
	datatrans=second_device.trans_data;
	for (i=0;i<11;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//����  
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
		uart4_buff.tx_data_buff[i++] = transID[n].state;   //������
		temp++;
		if(transID[n].state!=0xee){
		uart4_buff.tx_data_buff[i++] = transID[n].config_data1;   //������
		temp++;
		uart4_buff.tx_data_buff[i++] = transID[n].config_data2;   //������
		temp++;
	  datatem=transID[n].config_data1*256+transID[n].config_data2;   //��ȡ���������õĵ�ַ����
	//	delay_ms(3);
		uart4_buff.tx_data_buff[i++] = trans_query_data[n].flagdata/256;  //������̽ͷ״̬λ
		temp++;
		uart4_buff.tx_data_buff[i++] = trans_query_data[n].flagdata%256;  
		temp++;
		//datatim=trans_query_data[n].flagdata;
		for(x=0;x<4;x++) { 
         if((datatem&0x8000)==0x8000){//�ж������˼��ִ�����̽ͷ
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
 		uart4_buff.tx_data_buff[i++] = transID[m].state;   //������
		temp++;
		uart4_buff.tx_data_buff[i++] = transID[m].config_data1;   //������
		temp++;
		uart4_buff.tx_data_buff[i++] = transID[m].config_data2;   //������
		temp++;
		temdata1=transID[m].config_data1;
		delay_ms(3);
	  temdata2=transID[m].config_data2;   //��ȡ���������õĵ�ַ����
		delay_ms(3);
		datatem=temdata1*256+temdata2;
		uart4_buff.tx_data_buff[i++] = trans_query_data[m].flagdata/256;  //������̽ͷ״̬λ
		temp++;
		uart4_buff.tx_data_buff[i++] = trans_query_data[m].flagdata%256;  
		temp++;
	//	datatim=trans_query_data[m].flagdata;
		for(x=0;x<4;x++) { 
         if((datatem&0x8000)==0x8000){//�ж������˼��ִ�����̽ͷ
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
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRCУ���λ
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRCУ���λ

USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);

  for(m=0;m<datatrans;m++){
		 if(transID[m].state!=0xee){
	datatem=transID[m].config_data1*256+transID[m].config_data2;		  //�洢��������
	for(x=0;x<4;x++) { 
         if((datatem&0x8000)==0x8000){//�ж������˼��ִ�����̽ͷ
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
            if(!(trans_query_data[n].decimal[x]|trans_query_data[n].flag[x])){  //��λ����ѯ2�����϶����쳣�����������ϱ�
				 Alarmdata[n]++;
				 if(Alarmdata[n]==2){
				 	  Alarmdata[n]=0;
					  uart4_buff.tx_data_buff[11] +=1;
					  uart4_buff.tx_data_buff[i++] =n;
					 temp++;
					  for(j=0;j<3;j++){
		uart4_buff.tx_data_buff[i++] = read_random(0,1,(708*m+j));   //������
		temp++;
		delay_ms(3);}
		temdata1=read_random(0,1,(708*m+1));
		delay_ms(3);
	    temdata2=read_random(0,1,(708*m+2));   //��ȡ���������õĵ�ַ����
		delay_ms(3);
		datatem=temdata1*256+temdata2;
		uart4_buff.tx_data_buff[i++] = trans_query_data[m].flagdata/256;  //������̽ͷ״̬λ
		temp++;
		uart4_buff.tx_data_buff[i++] = trans_query_data[m].flagdata%256;  
		temp++;
	//	datatim=trans_query_data[m].flagdata;
		for(x=0;x<4;x++) { 
         if((datatem&0x8000)==0x8000){//�ж������˼��ִ�����̽ͷ
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
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: ���豸�Ƿ����������ķ�����Ϣ
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
	uart4_buff.tx_data_buff[6] = 0x06;		//����
		temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
		uart4_buff.tx_data_buff[11] = (u8)((temp>>8)&0x00ff);	//CRCУ���λ
		uart4_buff.tx_data_buff[12] = (u8)(temp&0x00ff); 		//CRCУ���λ

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
		uart4_buff.tx_data_buff[7] = 0x0E; 	//����
		uart4_buff.tx_data_buff[8] = 0x00;
		uart4_buff.tx_data_buff[9] = 0x00;
	  uart4_buff.tx_data_buff[10] = 0x01;
	  uart4_buff.tx_data_buff[11] = ComFlag;


		temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
		uart4_buff.tx_data_buff[12] = (u8)((temp>>8)&0x00ff);	//CRCУ���λ
		uart4_buff.tx_data_buff[13] = (u8)(temp&0x00ff); 		//CRCУ���λ

	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);

}

/**********************************************************
 *
 * @file	: Salt_Read_back()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: ���ζȲ����ķ�����Ϣ
 *
 **********************************************************/
void Salt_Read_back(u8 *buffer, u8 Len)
{
	u8 i,n= 0;
	u16 temp;
//	u8 datatem;//����������
//	datatem=second_device.trans_data;
	uart4_buff.tx_len =16;

	for (i=0;i<12;i++)
	{
		uart4_buff.tx_data_buff[i] = buffer[i];
	}
	uart4_buff.tx_data_buff[8] = 0xd0;		//����
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
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: д������֡������ȷ�ķ�����Ϣ
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
	uart4_buff.tx_data_buff[6] = uart4_buff.tx_len - 7; 	//����
	uart4_buff.tx_data_buff[8] = 0xd0; 	//
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRCУ���λ
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff); 		//CRCУ���λ


	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
	
}

/**********************************************************
 *
 * @file	: ID_Write_back()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: д�豸ID��������Ϣ
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
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: д�̼��汾��������Ϣ
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
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: д�̵������ü�������Ϣ
 *
 **********************************************************/
void Relayset_Write_back(u8 *buffer, u8 Len)
{
u8 k,m;
u8 n=0;
u8 i=0;	
//	u16 temp;
	u8 datatrans;//����������
	//u8 sdata[2];
	//u16 datatem;
	datatrans=second_device.relay_data;
	
  //	n=buffer[10]-1;	   //ID
	//datatem=buffer[11]*256+buffer[12];

	switch(buffer[11]){
					case 0x52:		//ɾ���̵�������
					    if(!buffer[12]){
						for (m=0;m<datatrans;m++)
	                               {
																	 relaysetflag[m]=0;
	              //	for (k=1;k<3;k++){
								write_byte(0,0,366*m,0xEE);
	                            delay_ms(3);         //��д�洢�������ն�״̬
		                               //}
	                               }
									write_byte(0,0,0xC053,0);           //�����������
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
		                       //��д�洢�������ĵ�ַ
													 write_byte(0,0,366*(n-1),0xee);
													 delay_ms(3);
		                        //                }   
			                        //  break;     
                     						          }
										                           }
	                     }
						     }									
						break;
					
					case 0x51:		//���Ǽ̵�������
						   for (m=0;m<buffer[12];m++)
	                    {           //�ж������˼��ִ�����̽ͷ
		                  // for (n=1;n<datatrans+1;n++)      {
			                      //if(buffer[13+i]==n){         //IDƥ��
												n=buffer[13+i];
															relaysetflag[n-1]=0;
												write_byte(0,0,366*(n-1),0xe2);
												delay_ms(3);
										for (k=0;k<3;k++){				  //д�ն�����
												write_byte(0,0,k+366*(n-1)+67,buffer[14+i+k]);
												delay_ms(3);
		                                                   }
			                              // break;     
                     						           //       }
										                      //    }
										        i+=4;                   //
	                     } 	
					        write_byte(0,0,0xC053,n);           //�̵��������
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
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: д���������ü�������Ϣ
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
	u8 datatrans;//����������
	//u8 sdata[2];
	u16 datatem;
	datatrans=second_device.trans_data;
	//setDataflag[m]=0;
  //	n=buffer[10]-1;	   //ID
	//datatem=buffer[11]*256+buffer[12];

switch(buffer[11]){
					case 0x52:		//ɾ������������
					    if(!buffer[12]){
						for (m=0;m<datatrans;m++)
	                               {
									setDataflag[m]=0;
                   transflag[m]=1;																	 
	              	for (k=1;k<3;k++){
								write_byte(0,1,708*m+k,0x00);
	                            delay_ms(3);         //��д�洢�������ĵ�ַ
		                               }
									write_byte(0,1,708*m,0xee);
	                            delay_ms(3);   
	                               }
									write_byte(0,0,0xC052,0);           //�����������
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
		                       //��д�洢�������ĵ�ַ
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
					
					case 0x51:		//���Ǳ���������
						   for (m=0;m<buffer[12];m++)
	                    {           //�ж������˼��ִ�����̽ͷ
		                   //for (n=1;n<datatrans+1;n++)      {
			                     // if(buffer[13+i]==n){         //IDƥ��
												   n=buffer[13+i];
															setDataflag[n-1]=0;
															transflag[n-1]=1;
														write_byte(0,1,708*(n-1),0xe2);
	                          delay_ms(3);
										for (k=0;k<2;k++){				  //д�ն�����
												write_byte(0,1,k+708*(n-1)+1,buffer[14+i+k]);
												delay_ms(3);
		                                                   }
													datatem=buffer[14+i]*256+buffer[15+i];				  
												    for(x=0;x<4;x++) { 
                                       if((datatem&0x8000)==0x8000){           //�жϴ�����̽ͷ����  д����Ӧ���͵�����
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
						     datatem=buffer[14+i]*256+buffer[15+i];//��һ������������״̬
							for(x=0;x<4;x++) { 
                            if((datatem&0x8000)==0x8000){j++;} 
                            datatem=datatem<<1; 
                                         } 
										        i+=2*j+3;                   //�ж������˼��ִ�����̽ͷ
	                     }
									write_byte(0,0,0xC052,n);           //�����������
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
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: д�������������޼�������Ϣ
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
	u8 datatrans;//����������
	u16 datatem;
		u8 sdata[2];
	u8 datatim;//�������ݶ�����
	datatrans=second_device.trans_data;
	//gateDataflag=0;
	//datatrans=buffer[11];

	switch(buffer[11]){
					case 0x52:		//ɾ������������
					    if(!buffer[12]){
						for (m=0;m<datatrans;m++)
	                               {
									gateDataflag[m]=0;
	              	for (k=0;k<2;k++){
								write_byte(0,1,708*m+35+k,0x00);
	                            delay_ms(3);         //��д�洢�������ĵ�ַ
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
		                       //��д�洢�������ĵ�ַ
													 write_byte(0,1,708*(n-1)+35+k,sdata[k]);
													 delay_ms(3);
		                                        }
								                    }
								 else {
		                       for (k=0;k<2;k++){
		                       //��д�洢�������ĵ�ַ
													 write_byte(0,1,708*(n-1)+35+k,0x00);
													 delay_ms(3);
		                                        }
									   }			   
			                          break;     
                     						          }
										                            }
						    }									
						break;
					
					case 0x51:		//���Ǳ���������
		                   for (n=1;n<datatrans+1;n++)      {
			                      if(buffer[12]==n){         //IDƥ��
															gateDataflag[n-1]=0;
								        sdata[0]= read_random(0,1,708*(n-1)+35);
								        delay_ms(3);
								        sdata[1]= read_random(0,1,708*(n-1)+36);
								        delay_ms(3);
										sdata[0] |=	buffer[13];
										sdata[1] |=	buffer[14];
										for (k=0;k<2;k++){				  //д�ն�����
												write_byte(0,1,708*(n-1)+35+k,sdata[k]);
												delay_ms(3);
		                                                   }
													datatem=sdata[0]*256+sdata[1];				  
												    for(x=0;x<4;x++) { 
                                                      if((datatem&0x8000)==0x8000){
														datatim=buffer[18+j];	
														write_byte(0,1,28*x+708*(n-1)+37,buffer[15+j]);	 //�̵���ID��ͨ��
														delay_ms(3);
														write_byte(0,1,28*x+708*(n-1)+38,buffer[16+j]);
														delay_ms(3);
														write_byte(0,1,28*x+708*(n-1)+39,buffer[17+j]);
														delay_ms(3);
														write_byte(0,1,28*x+708*(n-1)+40,buffer[18+j]);  //�ݶ�����
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
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: д�������������޼�������Ϣ
 *
 **********************************************************/
void Alarm_Write_back(u8 *buffer, u8 Len)
{
u8 m,k,n= 0;
u8 x,y= 0;
u8 sdata[2];	
	//u16 temp;
	u8 datatrans;//����������
	u16 datatem;
	datatrans=second_device.trans_data;
	//alaDataflag=0;

	switch(buffer[11]){
					case 0x52:		//ɾ������������
					    if(!buffer[12]){
						for (m=0;m<datatrans;m++)
	                               {
										alaDataflag[m]=0;							 
	              	for (k=0;k<2;k++){
								write_byte(0,1,708*m+485,0x00);
	                            delay_ms(3);         //��д�洢�������ĵ�ַ
		                               }
	                               }
						               }
						else {
							alaDataflag[buffer[12]-1]=0;
		                       for (k=0;k<2;k++){
		                       //��д�洢�������������õĵ�ַ
										write_byte(0,1,708*(buffer[12]-1)+k+485,0x00);
										delay_ms(3);
		                                        }  
						     }									
						break;
					
					case 0x51:		//����޸ı���������
						alaDataflag[buffer[12]-1]=0;
										for (k=0;k<2;k++){
										     sdata[k]=read_random(0,1,708*(buffer[12]-1)+k+485);
											 delay_ms(3);
											 sdata[k] |=buffer[13+k];
											             }
										for (k=0;k<2;k++){				  //д��������
												write_byte(0,1,k+708*(buffer[12]-1)+485,sdata[k]);
												delay_ms(3);
		                                                   }
													datatem=buffer[13]*256+buffer[14];				  
										for(x=0;x<8;x++) { 
                                           if((datatem&0x8000)==0x8000){
		                                     for (k=0;k<6;k++){
	                                                 write_byte(0,1,708*n+487+k+6*x,buffer[15+6*y+k]);//��Ӧ���ö�Ӧ��������������   ��ȡ�洢����Ӧ��Ϣ
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
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: д�̵����̶�������������Ϣ����
 *
 **********************************************************/
void Relayopengate_Write_back(u8 *buffer, u8 Len)
{
u8 m,n= 0;
u16 k= 0;
u8 j,x,y= 0;
u8 b=0;	
//	u16 temp;
	u8 datatrans;//����������
	u16 datatem;
		u8 sdata[2];
	u8 datatim;//�������ݶ�����
	datatrans=second_device.relay_data;
	//gateDataflag=0;
	//datatrans=buffer[11];

	switch(buffer[11]){
					case 0x52:		//ɾ���̵����̶�����
					    if(!buffer[12]){
						for (m=0;m<datatrans;m++)
	                               {
									relaygateDataflag[m]=0;
	              	for (k=0;k<2;k++){
								write_byte(0,0,366*m+108+k,0x00);
	                            delay_ms(3);         //��д�洢�������ĵ�ַ
		                               }
	                               }
						               }
						else {
		             for (n=1;n<(datatrans+1);n++)      {
			                 if(buffer[12]==n){
												 relaygateDataflag[n-1]=0;
								 for (k=0;k<2;k++){
		                       //��д�洢�������ĵ�ַ
													 write_byte(0,0,366*(n-1)+108+k,0x00);
													 delay_ms(3);
		                                        }
			                          break;     
                     						          }
										                            }
						    }									
						break;
					
					case 0x51:		//���Ǳ���������
		                   for (n=1;n<datatrans+1;n++)      {
			                      if(buffer[12]==n){         //IDƥ��
															relaygateDataflag[n-1]=0;
								        sdata[0]= read_random(0,0,366*(n-1)+108);
								        delay_ms(3);
								        sdata[1]= read_random(0,0,366*(n-1)+109);
								        delay_ms(3);
										sdata[0] |=	buffer[13];
										sdata[1] |=	buffer[14];
										for (k=0;k<2;k++){				  //д�ն�����
												write_byte(0,0,366*(n-1)+108+k,sdata[k]);
												delay_ms(3);
		                                                   }
													datatem=buffer[13]*256+	buffer[14];				  
												    for(x=0;x<8;x++) { 
                                   if((datatem&0x8000)==0x8000){
														datatim=buffer[26+j];	
																				for (k=0;k<12;k++){
														write_byte(0,0,32*x+366*(n-1)+110+k,buffer[15+j+k]);	 //�̵���ID��ͨ��
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
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: ����ϵͳʱ�估������Ϣ
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
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: дϵͳ���ü�������Ϣ
 *
 **********************************************************/
void Systemset_Write_back(u8 *buffer, u8 Len)
{
u8 x,y= 0;	
//	u16 temp;
//	u8 datatrans;//����������
	u16 datatem;
	//alaDataflag=0;
	sysDataflag=0;
	datatem=buffer[11]*256+buffer[12];
	  write_byte(0,0,0xC000+16,buffer[11]);//��Ӧ���ö�Ӧ��������   
	  delay_ms(3);
	    write_byte(0,0,0xC000+17,buffer[12]);//��Ӧ���ö�Ӧ��������
	  delay_ms(3);
		for(x=0;x<16;x++) { 				  
         if((datatem&0x8000)==0x8000){
	  write_byte(0,0,0xC000+18+x,buffer[13+y]);//��Ӧ���ö�Ӧ��������������   ��ȡ�洢����Ӧ��Ϣ
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
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: д�̵����ֶ����Ƽ�������Ϣ   ע���ͷź�ɾ��������  �ͷ����ͷŸü̵������ֶ��������ȼ�   ɾ���ǻָ�Ĭ�ϵ����ͷſ������ȼ�  
 *
 **********************************************************/
void Relaycon_Write_back(u8 *buffer, u8 Len)
{

u8 y= 0;	
//	u16 temp;
	//u8 datatrans;//�̵�������
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
					                    //���Ϳ���/�رռ̵���ͨ��ָ��
		       shoudong_control[buffer[11]-1].channeldata |=(1<<(15-y));
					//shoudong_control[buffer[11]-1].channeldata |=(1<<(15-y));
					write_byte(0,0,366*(buffer[11]-1)+11,shoudong_control[buffer[11]-1].channeldata/256);//��Ӧͨ��
	                  delay_ms(3);
					write_byte(0,0,366*(buffer[11]-1)+12,shoudong_control[buffer[11]-1].channeldata%256);//��Ӧͨ��
	                  delay_ms(3);
					shoudong_control[buffer[11]-1].type[y]=buffer[14];
				  time_get(Time_GetData);
					shoudong_control[buffer[11]-1].timedatah[y]=Time_GetData[4];
					shoudong_control[buffer[11]-1].timedatal[y]=buffer[15]*256+buffer[16]+Time_GetData[5];
					if(shoudong_control[buffer[11]-1].timedatal[y]>60){
					 shoudong_control[buffer[11]-1].timedatal[y]=shoudong_control[buffer[11]-1].timedatal[y]-60; 
					shoudong_control[buffer[11]-1].timedatah[y]=1+Time_GetData[4];
					}
									 write_byte(0,0,366*(buffer[11]-1)+15+y*3,buffer[14]);//��Ӧͨ���Ķ�������
	                  delay_ms(3);
									 write_byte(0,0,366*(buffer[11]-1)+14+y*3,shoudong_control[buffer[11]-1].timedatah[y]);//��Ӧͨ���Ķ���ʱ��
	                  delay_ms(3);
									 write_byte(0,0,366*(buffer[11]-1)+15+y*3,shoudong_control[buffer[11]-1].timedatal[y]);//��Ӧͨ���Ķ���ʱ��
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
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: д�ζȲ������ݼ�������Ϣ
 *
 **********************************************************/
void Salt_Write_back(u8 *buffer, u8 Len)
{
    u8 k,n= 0;
	
	n=buffer[11]-1;	   //ID
	yanDataflag[n]=0;
	for (k=0;k<2;k++){
	  write_byte(0,1,708*n+583+k,buffer[12+k]);//��Ӧ���ö�Ӧ�������ζȲ�������
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
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: ���ñ�����У׼��������Ϣ
 *
 **********************************************************/
void Transzhun_Write_back(u8 *buffer, u8 Len)
{
	//u8 n= 0;
   // u8 x= 0;	
//	u16 temp;
//	u8 datatrans;//����������
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
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: ��ȡ�豸Ӳ���汾��������Ϣ
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
	uart4_buff.tx_data_buff[8] = 0xd0;		//����	
	
	uart4_buff.tx_data_buff[i++]=program_ver.ver_data1 ;        //Ӳ���汾��
	uart4_buff.tx_data_buff[i++]=program_ver.ver_data2 ;

	uart4_buff.tx_data_buff[i++] = program_ver.ver_data3;		  
	uart4_buff.tx_data_buff[i++] = program_ver.ver_data4;
	for (i=15;i<21;i++)
	{
		uart4_buff.tx_data_buff[i] = program_ver.id_data[i-15];
	}
//	uart4_buff.tx_data_buff[i++] = (u8)((device_address>>8)&0x00ff);//���洢���豸���к�
//	uart4_buff.tx_data_buff[i++] = (u8)((device_address>>8)&0xff);

	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRCУ���λ
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff); 		//CRCУ���λ

	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}

/**********************************************************
 *
 * @file	: DeviceCon_Write_back()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: �����豸ʹ�ܼ�������Ϣ
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
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: �����豸������������Ϣ
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
	uart4_buff.tx_data_buff[3] = 0xd0;		//����	
	uart4_buff.tx_data_buff[11]=buffer[11]+1;
	uart4_buff.tx_data_buff[i++]=buffer[13];		//����
	uart4_buff.tx_data_buff[i++]=buffer[14];		//����
	uart4_buff.tx_data_buff[i++]=buffer[15];		//����
	uart4_buff.tx_data_buff[i++]=0xd0;		//����
	uart4_buff.tx_len =18;
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRCУ���λ
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff);			//CRCУ���λ	
	
	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
	
	write_byte(0,0,0x0008,0x04);
	delay_ms(5);
	
	SoftReset();
	
}

/**********************************************************
 *
 * @file	: Intelligence_Write_back()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: д���������ܿ��Ƽ�������Ϣ
 *
 **********************************************************/
void Intelligence_Write_back(u8 *buffer, u8 Len)
{
    u8 k,n= 0;
	
    n=buffer[11]-1;	   //ID
	inteDataflag[n]=0;
		   for (k=0;k<2;k++){
	  write_byte(0,1,708*n+590+k,buffer[12+k]);//���ܿ���
	  delay_ms(3);	    
	                        }
     Write_back( buffer, Len );
}

/**********************************************************
 *
 * @file	: ID_Write_back()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: д�����豸ID��������Ϣ
 *
 **********************************************************/
void SecondID_Write_back(u8 *buffer, u8 Len)
{
	switch(buffer[10])     { 
				case 0x21:    //����������
							 write_byte(0,0,256+201*buffer[11],buffer[12]);	//��д�洢�����豸����ID										
					     delay_ms(3);
					break;
				
				case 0x31:      //�̵�������
		           write_byte(0,1,483*buffer[11],buffer[12]);	//��д�洢�����豸����ID										
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
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: д�����豸ID��������Ϣ
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
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: ����������������������Ϣ
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
	  write_byte(0,0,0xC000+64+k,buffer[11+k]);//��Ӧ���ö�Ӧ��������������   ��ȡ�洢����Ӧ��Ϣ
	  delay_ms(3);	    
	}
		for(x=0;x<16;x++) { 
         if((datatem&0x8000)==0x8000){
	  write_byte(0,0,0xC000+66+x,buffer[13+y]);//��Ӧ���ö�Ӧ��������������   ��ȡ�洢����Ӧ��Ϣ
	  delay_ms(3);	
				 y++;      }
          datatem=datatem<<1;
                    }
		
   Write_back( buffer, Len );
}

 /**********************************************************
 *
 * @file	: ExistSecondID_Write_back()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: ����ʵ�ʹ���������������Ϣ
 *
 **********************************************************/
void ExistSecondID_Write_back(u8 *buffer, u8 Len)
{
    u8 x= 0;
	sdDataflag=0;
		for(x=0;x<2;x++) { 
		           write_byte(0,0,0xC000+82+x,buffer[11+x]);	//��д�����豸����								
					     delay_ms(3);
                      }
   Write_back( buffer, Len );
}


/**********************************************************
 *
 * @file	: Restoresetting_Write_back()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: �ָ��������ü�������Ϣ
 *
 **********************************************************/
void Restoresetting_Write_back(u8 *buffer, u8 Len)
{
	u8 k= 0;
	for(k=0;k<22;k++){
	write_byte(0,0,0x0102+k,0x35);	//��д�������õĵ�ַλ							
	delay_ms(3);}
	Write_back( buffer, Len );
	write_byte(0,0,0xC022,0x03);
	delay_ms(3);
	SoftReset();	
}
				


/**********************************************************
 *
 * @file	: DeviceCon_Read_back()
 * @param	: u8 *buffer: ָ��ԭʼ���ݵ�u8��������ָ��
 *			  u8 Len: ԭʼ���ݳ���
 * @retval	: None
 * @brief	: ���豸ʹ�ܿ���
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
	uart4_buff.tx_data_buff[8] = 0xd0;		//����
	//if(buffer[10]==0x11){
	uart4_buff.tx_data_buff[13]=read_random(0,0,0xC000+35);   //���洢���豸ʹ��״̬
	delay_ms(3);
	//				    }
   // else {
//	uart4_buff.tx_data_buff[12]=read_random(0,0,35);
//	}
	temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
	uart4_buff.tx_data_buff[i++] = (u8)((temp>>8)&0x00ff);	//CRCУ���λ
	uart4_buff.tx_data_buff[i] = (u8)(temp&0x00ff); 		//CRCУ���λ
	
	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
	}


/*********************************************END OF FILE**********************/
