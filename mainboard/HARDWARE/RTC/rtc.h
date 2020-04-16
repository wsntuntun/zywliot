#ifndef __RTC_H
#define __RTC_H	 
#include "sys.h" 
#include "init.h"
 //////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//RTC 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/5
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
//********************************************************************************
//修改说明
//V1.1 20140726
//新增:RTC_Get_Week函数,用于根据年月日信息,得到星期信息.
////////////////////////////////////////////////////////////////////////////////// 
/*
#define FEBRUARY		2
#define STARTOFYEAR 	2000
#define SECDAY			86400L			 //一天有多少s
#define leapyear(year)		((year) % 4 == 0)
#define days_in_year(a) 	(leapyear(a) ? 366 : 365)
#define days_in_month(a)	(month_days[(a) - 1])

static u8 month_days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
*/



	
u8 My_RTC_Init(void);						//RTC初始化
ErrorStatus RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm);			//RTC时间设置
ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date,u8 week); 		//RTC日期设置
void RTC_Set_AlarmA(u8 week,u8 hour,u8 min,u8 sec);		//设置闹钟时间(按星期闹铃,24小时制)
void RTC_Set_WakeUp(u32 wksel,u16 cnt);					//周期性唤醒定时器设置
void RTC_Get_Time(u8 *hour,u8 *min,u8 *sec,u8 *ampm);	//获取RTC时间
void RTC_Get_Date(u8 *year,u8 *month,u8 *date,u8 *week);//获取RTC日期
void Time_Set(struct rtc_time *tm);
void time_get(u8 *TimeData);

#endif

















