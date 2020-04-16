#ifndef __RTC_H
#define __RTC_H	 
#include "sys.h" 
#include "init.h"
 //////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//RTC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/5
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
//********************************************************************************
//�޸�˵��
//V1.1 20140726
//����:RTC_Get_Week����,���ڸ�����������Ϣ,�õ�������Ϣ.
////////////////////////////////////////////////////////////////////////////////// 
/*
#define FEBRUARY		2
#define STARTOFYEAR 	2000
#define SECDAY			86400L			 //һ���ж���s
#define leapyear(year)		((year) % 4 == 0)
#define days_in_year(a) 	(leapyear(a) ? 366 : 365)
#define days_in_month(a)	(month_days[(a) - 1])

static u8 month_days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
*/



	
u8 My_RTC_Init(void);						//RTC��ʼ��
ErrorStatus RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm);			//RTCʱ������
ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date,u8 week); 		//RTC��������
void RTC_Set_AlarmA(u8 week,u8 hour,u8 min,u8 sec);		//��������ʱ��(����������,24Сʱ��)
void RTC_Set_WakeUp(u32 wksel,u16 cnt);					//�����Ի��Ѷ�ʱ������
void RTC_Get_Time(u8 *hour,u8 *min,u8 *sec,u8 *ampm);	//��ȡRTCʱ��
void RTC_Get_Date(u8 *year,u8 *month,u8 *date,u8 *week);//��ȡRTC����
void Time_Set(struct rtc_time *tm);
void time_get(u8 *TimeData);

#endif

















