#include "init.h"
#include "24cxx.h"
#include "delay.h"
#include "rtc.h"
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
//extern u8 online;
extern u8 enFlag;
extern u8 ComFlag;
extern u32 CpuID[3]; 
extern u8  IDDataflag,verDataflag,comDataflag,enDataflag;
extern u8  sysDataflag,capDataflag,jzDataflag,sdDataflag;
extern u8  setDataflag[4],inteDataflag[4],yanDataflag[4],jiaoDataflag;
extern u8  alaDataflag[4],gateDataflag[4],transDataflag,relayshouDataflag[4];
extern u8  relaygateDataflag[4],relayalDataflag[4],relaysetflag[4],relaygateENflag[4];
extern u8  transalarmENflag[4],transpumpENflag[4];

extern struct uart_data_buff uart4_buff;
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
//extern struct rtc_time settmtime;
//extern struct rtc_time systmtime;
extern struct trans_alarm_enable  transAlarm_En[4];
extern struct trans_pump  transpump_En[4];
extern struct relay_alarm_enable  relayAlarm_En[4];
extern struct relay_fixgate_enable relay_gate_En[4];

extern RTC_TimeTypeDef RTC_TimeStruct;
extern RTC_DateTypeDef RTC_DateStruct;

//初始化PF9和PF10为输出口.并使能这两个口的时钟		    
//LED IO初始化
/*void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF时钟

  //GPIOF9,F10初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;//LED0和LED1对应IO口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_SetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);//GPIOF9,F10设置高，灯灭

}*/

void EepromWriteData_init(void)
{
	u8 i=0;
if(read_random(0,0,0xF000)!=0x25){						  //设备ID		   00000001
  delay_ms(3);
	write_byte(0,0,0xC000,0x00);
	delay_ms(3);
	write_byte(0,0,0xC001,0x00);
	delay_ms(3);
	write_byte(0,0,0xC002,0x00);
	delay_ms(3);
	write_byte(0,0,0xC003,0x01);
	delay_ms(3);
	write_byte(0,0,0xF000,0x25);
	delay_ms(3);

								  }
 if(read_random(0,0,0xF001)!=0x25){						  //硬件版本   0.1
  delay_ms(3);
	write_byte(0,0,0xC004,0x00);
	delay_ms(3);
	write_byte(0,0,0xC005,0x01);
	delay_ms(3);
	write_byte(0,0,0xC006,0x00);					 //软件版本0.1
	delay_ms(3);
	write_byte(0,0,0xC007,0x01);
	delay_ms(3);
	write_byte(0,0,0xC008,0x00);
	delay_ms(3);
	write_byte(0,0,0xC009,0x00);
	delay_ms(3);
	write_byte(0,0,0xC00A,0x00);					 
	delay_ms(3);
	write_byte(0,0,0xC00B,0x00);
	delay_ms(3);
	write_byte(0,0,0xC00C,0x00);					 //SN-000000000001
	delay_ms(3);
	write_byte(0,0,0xC00D,0x01);
	delay_ms(3);
	write_byte(0,0,0xF001,0x25);
	delay_ms(3);
								  }
if(read_random(0,0,0xF003)!=0x25){						  //连接原因状态位
  delay_ms(3);
	write_byte(0,0,0xC022,0x01);
	delay_ms(3);
	write_byte(0,0,0xF003,0x25);
	delay_ms(3);

								  }
if(read_random(0,0,0xF004)!=0x25){						  //设备使能位
  delay_ms(3);
  write_byte(0,0,0xC023,0x01);
	delay_ms(3);
	write_byte(0,0,0xF004,0x25);
	delay_ms(3);
								  }
if(read_random(0,0,0xF002)!=0x25){						  //系统设置数据
  delay_ms(3);
	write_byte(0,0,0xC010,0xF8);         //数据配置
	delay_ms(3);
	write_byte(0,0,0xC011,0x00);
	delay_ms(3);
	write_byte(0,0,0xC012,0x01);         //显示切换周期
	delay_ms(3);
	write_byte(0,0,0xC013,0x02);         //GPRS处理周期
	delay_ms(3);
	write_byte(0,0,0xC014,0x01);         //变送器处理周期
	delay_ms(3);
	write_byte(0,0,0xC015,0x01);         //继电器处理周期
	delay_ms(3);
	write_byte(0,0,0xC016,0x01);         //手动打开/关闭继电器默认时间
	delay_ms(3);
	write_byte(0,0,0xC017,0x01);         //处理周期
	delay_ms(3);
	write_byte(0,0,0xC018,0x01);         //处理周期
	delay_ms(3);
	write_byte(0,0,0xC019,0x01);         //处理周期
	delay_ms(3);
	write_byte(0,0,0xC01A,0x01);         //手动打开/关闭继电器默认时间
	delay_ms(3);
	write_byte(0,0,0xF002,0x25);
	delay_ms(3);
								  }
if(read_random(0,0,0xF006)!=0x25){						  //系统支持容量
    delay_ms(3);
	write_byte(0,0,0xC040,0xFF);         //数据配置
	delay_ms(3); 
		write_byte(0,0,0xC041,0x70);
	delay_ms(3);
		write_byte(0,0,0xC042,0x02);        //变送器最大个数 
	delay_ms(3);
		write_byte(0,0,0xC043,0x08);        //探头最大个数
	delay_ms(3);
		write_byte(0,0,0xC044,0x04);         //梯度最大组数
	delay_ms(3);
		write_byte(0,0,0xC045,0x08);        //最大校正时间组数
	delay_ms(3);
		write_byte(0,0,0xC046,0x01);        //继电器个数
	delay_ms(3);
		write_byte(0,0,0xC047,0x0A);         //通道最大个数
	delay_ms(3);
		write_byte(0,0,0xC048,0x04);        //无门限固定开启最大组数
	delay_ms(3);
		write_byte(0,0,0xC049,0x02);         //有门限固定开启最大组数
	delay_ms(3);
		write_byte(0,0,0xC04A,0x04);         //固定时间开启时间组数
	delay_ms(3);
	write_byte(0,0,0xF006,0x25);
	delay_ms(3);
								  }

	if(read_random(0,0,0xF007)!=0x25){						  //变送器配置个数和继电器配置个数
  delay_ms(3);
	write_byte(0,0,0xC052,0x00);    
	delay_ms(3);
	write_byte(0,0,0xC053,0x00);
	delay_ms(3);
	write_byte(0,0,0xF007,0x25);
	delay_ms(3);
								  }
for(i=0;i<4;i++){	
if(read_random(0,0,0xF009+i)!=0x25){						  //变送器状态
  delay_ms(3);
  write_byte(0,1,708*i,0xEE);          //ID
	delay_ms(3);
  write_byte(0,0,0xF009+i,0x25);
	delay_ms(3);                    }   
                  }
for(i=0;i<4;i++){	
if(read_random(0,0,0xF05A+i)!=0x25){						  //变送器控制门限
  delay_ms(3);
  write_byte(0,1,708*i+35,0x00);          //ID
	delay_ms(3);
	write_byte(0,1,708*i+36,0x00);          //ID
	delay_ms(3);
  write_byte(0,0,0xF05A+i,0x25);
	delay_ms(3);                    }   
                  }
for(i=0;i<4;i++){	
if(read_random(0,0,0xF0AB+i)!=0x25){						  //变送器报警门限
  delay_ms(3);
  write_byte(0,1,708*i+485,0x00);          //ID
	delay_ms(3);
	write_byte(0,1,708*i+486,0x00);          //ID
	delay_ms(3);
  write_byte(0,0,0xF0AB+i,0x25);
	delay_ms(3);                    }   
                  }	
for(i=0;i<4;i++){	
if(read_random(0,0,0xF14D+i)!=0x25){						  //变送器智能控制
  delay_ms(3);
  write_byte(0,1,708*i+590,0x00);          //ID
	delay_ms(3);
	write_byte(0,1,708*i+591,0x00);          //ID
	delay_ms(3);
  write_byte(0,0,0xF14D+i,0x25);
	delay_ms(3);                    }   
                  }	
for(i=0;i<4;i++){	
if(read_random(0,0,0xF1EF+i)!=0x25){						  //变送器报警使能
  delay_ms(3);
  write_byte(0,1,708*i+690,0x00);          //ID
	delay_ms(3);
	//write_byte(0,1,708*i+591,0x00);          //ID
	//delay_ms(3);
  write_byte(0,0,0xF1EF+i,0x25);
	delay_ms(3);                    }   
                  }	

for(i=0;i<4;i++){	
if(read_random(0,0,0xF240+i)!=0x25){						  //变送器水泵配置
  delay_ms(3);
  write_byte(0,1,708*i+696,0x00);          //ID
	delay_ms(3);
	write_byte(0,1,708*i+697,0x00);          //ID
	delay_ms(3);
  write_byte(0,0,0xF240+i,0x25);
	delay_ms(3);                    }   
                  }	
for(i=0;i<1;i++){	
if(read_random(0,0,0xF384+i)!=0x25){						  //继电器状态
  delay_ms(3);
  write_byte(0,0,366*i,0xEE);          //ID
	delay_ms(3);
  write_byte(0,0,0xF384+i,0x25);
	delay_ms(3);                    }   
                  }
for(i=0;i<1;i++){	
if(read_random(0,0,0xF2E2+i)!=0x25){						  //继电器报警使能
  delay_ms(3);
  write_byte(0,0,366*i+61,0x00);          //ID
	delay_ms(3);
	//write_byte(0,0,708*i+591,0x00);          //ID
	//delay_ms(3);
  write_byte(0,0,0xF2E2+i,0x25);
	delay_ms(3);                    }   
                  }	
for(i=0;i<1;i++){	
if(read_random(0,0,0xF384+i)!=0x25){						  //继电器配置
  delay_ms(3);
  write_byte(0,0,366*i+67,0x00);          //ID
	delay_ms(3);
	write_byte(0,0,366*i+68,0x00);          //ID
	delay_ms(3);
	write_byte(0,0,366*i+69,0x00);          //ID
	delay_ms(3);
  write_byte(0,0,0xF384+i,0x25);
	delay_ms(3);                    }   
                  }								
for(i=0;i<1;i++){	
if(read_random(0,0,0xF3D5+i)!=0x25){						  //继电器固定任务控制使能
  delay_ms(3);
  write_byte(0,0,366*i+102,0x00);          //ID
	delay_ms(3);
  write_byte(0,0,0xF3D5+i,0x25);
	delay_ms(3);                    }   
                  }	

for(i=0;i<1;i++){	
if(read_random(0,0,0xF426+i)!=0x25){						  //继电器固定任务
  delay_ms(3);
  write_byte(0,0,366*i+108,0x00);          //任务号
	delay_ms(3);
	write_byte(0,0,366*i+109,0x00);          
	delay_ms(3);
  write_byte(0,0,0xF426+i,0x25);
	delay_ms(3);                    }   
                  }	
if(read_random(0,0,0xF008)!=0x25){						  //系统时钟		  2017.01.01  00:00:00
	write_byte(0,0,0xf008,0x25);
	delay_ms(3);
	RTC_DateStruct.RTC_Year = 0x11;
	RTC_DateStruct.RTC_Month = 0x01;
	RTC_DateStruct.RTC_Date= 0x01;
	RTC_DateStruct.RTC_WeekDay= 0x01;
	RTC_TimeStruct.RTC_Hours= 0x00;
	RTC_TimeStruct.RTC_Minutes = 0x00;
	RTC_TimeStruct.RTC_Seconds= 0x00;
	//Time_Set(&settmtime);
	RTC_Set_Time(RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds,RTC_H12_AM);	//设置时间
	RTC_Set_Date(RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date,RTC_DateStruct.RTC_WeekDay);		//设置日期
								  }
}

void EepromReadData_init(void)
{  
ReadIDData_init();
ReadverData_init();
ReadcomData_init();
ReadenData_init();	
 ReadsysData_init();
 ReadcapData_init(); 
//ReadtransData_init();
ReadrelaysetData_init();	
 //ReadjzData_init(); 
	ReadsetData_init();
 ReadsdData_init();
 ReadinteData_init();	
 ReadyanData_init();
 ReadjiaoData_init();
 ReadalaData_init();
 //ReadgateData_init();	
 ReadrelayAlData_init();
 //ReadrelayungateData_init();
ReadrelaygateData_init();
ReadrelaygateEnData_init();
ReadrelayShouData_init();	
ReadtransalarmEN_init();
ReadtranspumpEN_init();	
}

	void ReadIDData_init(void){
		if(!IDDataflag){
	IDDataflag=1;
	device_id.year_datah = read_random(0,0,0xC000);		 //读设备ID
	delay_ms(3);
	device_id.year_datal = read_random(0,0,0xC001);
	delay_ms(3);
	device_id.sn_datah   = read_random(0,0,0xC002);
	delay_ms(3);
	device_id.sn_datal   = read_random(0,0,0xC003);
	delay_ms(3);}
                           }
	
  void ReadverData_init(void){
    u8 i=0;
		if(!verDataflag){
	verDataflag=1;
	program_ver.ver_data1 =read_random(0,0,0xC004);		   //读版本号
	delay_ms(3);
	program_ver.ver_data2 =read_random(0,0,0xC005);
	delay_ms(3);
	program_ver.ver_data3 =read_random(0,0,0xC006);
	delay_ms(3);
	program_ver.ver_data4 =read_random(0,0,0xC007);
	delay_ms(3);
	for(i=0;i<6;i++) {
	program_ver.id_data[i]=read_random(0,0,0xC008+i);
	delay_ms(3);	
	}
	}}
	
	void ReadcomData_init(void){
		if(!comDataflag){
	comDataflag=1;
	ComFlag = read_random(0,0,0xC022);      //读连接原因
	delay_ms(3);
	write_byte(0,0,0xC022,0x01);
	delay_ms(3);
	}}
	
	void ReadenData_init(void){
		if(!enDataflag){
	enDataflag=1;
	enFlag= read_random(0,0,0xC023);//读设备使能位
	delay_ms(3);}}
	
  void ReadsysData_init(void){
		if(!sysDataflag){
	sysDataflag=1;
	system_info.set_data1 =read_random(0,0,0xC010);			//读系统设置
	delay_ms(3);
	system_info.set_data2=read_random(0,0,0xC011);
	delay_ms(3);
	system_info.display_time=read_random(0,0,0xC012);
	delay_ms(3);
	system_info.displayTo_time=read_random(0,0,0xC013);
	delay_ms(3);
	system_info.GPRS_time=read_random(0,0,0xC014);
	delay_ms(3);
	system_info.trans_time=read_random(0,0,0xC015);	 
	delay_ms(3);
	system_info.relay_time=read_random(0,0,0xC016);
	delay_ms(3); 	
	system_info.hand_time=read_random(0,0,0xC017);
	delay_ms(3);
	system_info.Alarm_time=read_random(0,0,0xC018);
	delay_ms(3);
	system_info.AlarmTo_time=read_random(0,0,0xC019);
	delay_ms(3);
	system_info.GPRSTo_time=read_random(0,0,0xC01A);
	delay_ms(3);}}
	
	void ReadcapData_init(void){
		if(!capDataflag){
	capDataflag=1;
	capacity_info.set_data1 =read_random(0,0,0xC040);  //读系统支持容量
	delay_ms(3);
	capacity_info.set_data2 =read_random(0,0,0xC041);
	delay_ms(3);
	capacity_info.maxtrans_data =read_random(0,0,0xC042);
	delay_ms(3);
	capacity_info.maxsensor_data =read_random(0,0,0xC043);
	delay_ms(3);
	capacity_info.maxgradient_data =read_random(0,0,0xC044);
	delay_ms(3);
	capacity_info.maxcheck_data =read_random(0,0,0xC045);
	delay_ms(3);
	capacity_info.maxrelay_data =read_random(0,0,0xC046);
	delay_ms(3);
	capacity_info.maxgallery_data =read_random(0,0,0xC047);
	delay_ms(3);
	capacity_info.maxungate_data=read_random(0,0,0xC048);
	delay_ms(3);
	capacity_info.maxgate_data=read_random(0,0,0xC049);
	delay_ms(3);
	capacity_info.maxfixed_data=read_random(0,0,0xC04a);
	delay_ms(3);}}
	
/*	void ReadjzData_init(void){
		if(!jzDataflag){
	jzDataflag=1;
	jiaozhun_info.set =read_random(0,0,0x001c);  //读固定时间校正参数
	delay_ms(3);
	jiaozhun_info.id1_data =read_random(0,0,0x001d);
	delay_ms(3);
	jiaozhun_info.state1_data =read_random(0,0,0x001e);
	delay_ms(3);
	jiaozhun_info.time1_datah1 =read_random(0,0,0x001f);
	delay_ms(3);
	jiaozhun_info.time1_datal1 =read_random(0,0,0x0020);
	delay_ms(3);
	jiaozhun_info.time1_datah2 =read_random(0,0,0x0021);
	delay_ms(3);
	jiaozhun_info.time1_datal2 =read_random(0,0,0x0022);
	delay_ms(3);   }} */
		
	void ReadsdData_init(void){
		if(!sdDataflag){
	sdDataflag=1;
	second_device.trans_data =read_random(0,0,0xC052);   //读取配置的变送器个数
	delay_ms(3);
	second_device.relay_data =read_random(0,0,0xC053);   //读取配置的继电器个数
	delay_ms(3);}}
	
	void ReadsetData_init(void){                    //读变送器配置
		u8 i,m=0;
		u16 temp;
		for(i=0;i<4;i++){
		if(!setDataflag[i]){
	setDataflag[i]=1;
	transID[i].state =read_random(0,1,708*i);
	delay_ms(3);      
	     if(transID[i].state!=0xee){
		     transID[i].config_data1 =read_random(0,1,708*i+0x0001);
	       delay_ms(3);
	       transID[i].config_data2 =read_random(0,1,708*i+0x0002);
	       delay_ms(3); 
				 transID[i].config_data =transID[i].config_data1*256+transID[i].config_data2;	
				 temp=transID[i].config_data;
				 for(m=0;m<16;m++){
					 if((temp&0x8000)==0x8000){
						 transID[i].probe_data1[m]=read_random(0,1,708*i+0x0003+m*2);
						 delay_ms(3);
             transID[i].probe_data2[m]=read_random(0,1,708*i+0x0004+m*2);
						 delay_ms(3);						 
					                            }
					   temp=temp <<1;
				                    }
	                               }
	                  }
	                   }
	                          }
		void ReadrelaysetData_init(void){		  //继电器配置
					u8 i=0;
		for(i=0;i<1;i++){
		if(!relaysetflag[i]){
	relaysetflag[i]=1;
	relay_result_data[i].state =read_random(0,0,366*i);
	delay_ms(3);      
	     if(relay_result_data[i].state!=0xee){
				 relay_result_data[i].relaydata = read_random(0,0,366*i+67);
	delay_ms(3);
		relay_result_data[i].setData1 = read_random(0,0,366*i+68);			 
	delay_ms(3);relay_result_data[i].setData2 = read_random(0,0,366*i+69);
	delay_ms(3);
	relay_result_data[i].setData=relay_result_data[i].setData1*256+relay_result_data[i].setData2 ;

}}}}
	void ReadinteData_init(void){
				u8 n=0;
		for(n = 0; n < second_device.trans_data; n++){
		if(!inteDataflag[n]){
	inteDataflag[n]=1;
			if(transID[n].state!=0xee){
			
     Intelligent_value[n].flag_data1 =read_random(0,1,0x024e);  //读智能控制参数 590-591
	   delay_ms(3);
	   Intelligent_value[n].flag_data2 =read_random(0,1,0x024f);
	   delay_ms(3);  
	   Intelligent_value[n].flag_data =Intelligent_value[0].flag_data1*256+Intelligent_value[0].flag_data2;
	}}}}
	
	void ReadyanData_init(void){                           //读盐度值
		u8 n=0;
		for(n = 0; n < second_device.trans_data; n++){
		if(!yanDataflag[n]){
	yanDataflag[n]=1;
			if(transID[n].state!=0xee){
	device_yandu[n].yandu_data1 =read_random(0,1,708*n+0x0247);
	delay_ms(3);
	device_yandu[n].yandu_data2 =read_random(0,1,708*n+0x0248);
	delay_ms(3);}}
              }}
	
	void ReadjiaoData_init(void){
 		if(!jiaoDataflag){
	jiaoDataflag=1;
	device_jiaozhun[0].set_data1 =read_random(0,1,0x249);//读校正参数	585-589
	delay_ms(3);
	device_jiaozhun[0].set_data2 =read_random(0,1,0x024A);
	delay_ms(3);
	device_jiaozhun[0].type_data =read_random(0,1,0x024B);
	delay_ms(3);
	device_jiaozhun[0].para_data =read_random(0,1,0x024C);
	delay_ms(3);
	device_jiaozhun[0].result_data =read_random(0,1,0x024D);
	delay_ms(3);
	}}
		
	void ReadalaData_init(void){
		u8 i,n=0;
	for(n = 0; n < second_device.trans_data; n++){
		if(!alaDataflag[n]){
	alaDataflag[n]=1;
			if(transID[n].state!=0xee){
	data_alarm_value[n].set_data1 = read_random(0,1,708*n+0x01E5);   //读报警参数	   485-582
	delay_ms(3);data_alarm_value[n].set_data2 = read_random(0,1,708*n+0x01E6);
	delay_ms(3);
	data_alarm_value[n].set_data = data_alarm_value[0].set_data1*256+data_alarm_value[0].set_data2 ;
	for(i = 0; i < 16; i++){
	  if((data_alarm_value[n].set_data&0x8000)==0x8000)	{
    data_alarm_value[n].integerh_temp1[i] = read_random(0,1,708*n+487+6*i);
	delay_ms(3);data_alarm_value[n].integerl_temp1[i] = read_random(0,1,708*n+488+6*i);
	delay_ms(3);
	//data_alarm_value[0].integer_temp1[i] =data_alarm_value[0].integerl_temp1[i]*256+data_alarm_value[0].integerl_temp1[i];
	data_alarm_value[n].integerh_temp2[i] = read_random(0,1,708*n+489+6*i);
	delay_ms(3);data_alarm_value[n].integerl_temp2[i] = read_random(0,1,708*n+490+6*i);
	delay_ms(3);
	//data_alarm_value[0].integer_temp2[i] =data_alarm_value[0].integerl_temp2[i]*256+data_alarm_value[0].integerl_temp2[i];		
	data_alarm_value[n].integerh_temp0[i] = read_random(0,1,708*n+491+6*i);
	delay_ms(3);data_alarm_value[n].integerl_temp0[i] = read_random(0,1,708*n+492+6*i);
	delay_ms(3);
	//data_alarm_value[0].integer_temp0[i] =data_alarm_value[0].integerl_temp0[i]*256+data_alarm_value[0].integerl_temp0[i];
	}
	  data_alarm_value[n].set_data=data_alarm_value[n].set_data <<1;
	 }
	data_alarm_value[n].set_data = data_alarm_value[n].set_data1*256+data_alarm_value[n].set_data2 ;
	}}
}
	}


	void ReadgateData_init(void){					   //读控制门限
	u8 i,j,n=0;
	for(n = 0; n < second_device.trans_data; n++){
		if(!gateDataflag[n]){
	gateDataflag[n]=1;
			if(transID[n].state!=0xee){
	control_info[n].set_data1 = read_random(0,1,708*n+0x0023);   //读控制门限		  35-484
	delay_ms(3);
	control_info[n].set_data2 = read_random(0,1,708*n+0x0024);   //读控制门限		  35-484
	delay_ms(3);
	control_info[n].set_data = control_info[n].set_data1*256+control_info[n].set_data2 ;
	for(i = 0; i < 16; i++){
	  if((control_info[n].set_data&0x8000)==0x8000)	{
	 control_info[n].Transdata_control[i].relay_ID = read_random(0,1,708*n+37+28*i);
	delay_ms(3);control_info[n].Transdata_control[i].relay_datah = read_random(0,1,708*n+38+28*i);
	delay_ms(3);control_info[n].Transdata_control[i].relay_datal = read_random(0,1,708*n+39+28*i);
	control_info[n].Transdata_control[i].relay_data=control_info[n].Transdata_control[i].relay_data*256+control_info[n].Transdata_control[i].relay_data;
	delay_ms(3);
	control_info[n].Transdata_control[i].gradient = read_random(0,1,708*n+40+28*i);   //读控制门限		  35-484
	for(j = 0; j < 4; j++){
	   if((control_info[n].Transdata_control[i].gradient&0x80)==0x80)  {
	delay_ms(3);control_info[n].Transdata_control[i].gate_datah1[j] = read_random(0,1,708*n+41+28*i+6*j);
	delay_ms(3);control_info[n].Transdata_control[i].gate_datal1[j] = read_random(0,1,708*n+42+28*i+6*j);
	control_info[n].Transdata_control[i].gate_data1[j]=control_info[0].Transdata_control[i].gate_datah1[j]*256+control_info[0].Transdata_control[i].gate_datal1[j];
	delay_ms(3);control_info[n].Transdata_control[i].gate_datah2[j] = read_random(0,1,708*n+43+28*i+6*j);
	delay_ms(3);control_info[n].Transdata_control[i].gate_datal2[j] = read_random(0,1,708*n+44+28*i+6*j);
	control_info[n].Transdata_control[i].gate_data2[j]=control_info[n].Transdata_control[i].gate_datah2[j]*256+control_info[n].Transdata_control[i].gate_datal2[j];
	delay_ms(3);    
	control_info[n].Transdata_control[i].contype[j] = read_random(0,1,708*n+45+28*i+6*j);
	delay_ms(3);
	control_info[n].Transdata_control[i].relays[j] = read_random(0,1,708*n+46+28*i+6*j);
	delay_ms(3);
	}
	control_info[n].Transdata_control[i].gradient=control_info[n].Transdata_control[i].gradient <<1;
	}
	control_info[n].Transdata_control[i].gradient = read_random(0,1,708*n+40+28*i);
	 }
	  control_info[n].set_data=control_info[n].set_data <<1;
	 }
	 control_info[n].set_data = control_info[n].set_data1*256+control_info[n].set_data2 ;
}
			}
}
}
	
	void ReadtransData_init(void){						 //读传感数据
	u8 i=0;
	u16 set_data;
		if(!transDataflag){
	transDataflag=1;
	set_data=transID[0].config_data1*256+transID[0].config_data2;
	for(i = 0; i < 4; i++){
	  if((set_data&0x8000)==0x8000)	{
	trans_query_data[0].integerh[i] = read_random(0,1,594+6*i);
	delay_ms(3);trans_query_data[0].integerl[i] = read_random(0,1,595+6*i);
	delay_ms(3);
	//trans_query_data[0].integer[i] = trans_query_data[0].integerh[i]*256+trans_query_data[0].integerl[i];

	trans_query_data[0].mvdatah[i] = read_random(0,1,596+6*i);
	delay_ms(3);trans_query_data[0].mvdatal[i] = read_random(0,1,597+6*i);
	delay_ms(3);
	//trans_query_data[0].mvdata[i] = trans_query_data[0].mvdatah[i]*256+trans_query_data[0].mvdatal[i];

	trans_query_data[0].decimal[i] = read_random(0,1,598+6*i);
	delay_ms(3);trans_query_data[0].flag[i] = read_random(0,1,599+6*i);
	delay_ms(3);
	  }
	   set_data=set_data <<1;
	 }
	trans_query_data[0].flagdata =transID[0].config_data1*256+transID[0].config_data2;
	}}
		
	void ReadrelayAlData_init(void){			 //读继电器报警使能
  u8 n=0;
		for(n=0;n<70;n++){
		if(!relayalDataflag[n]){
	relayalDataflag[n]=1;
	     if(relay_result_data[n].state!=0xee){
  relayAlarm_En[n].set = read_random(0,0,366*n+61);			 
	delay_ms(3);relayAlarm_En[n].setflag = read_random(0,0,366*n+62);
	delay_ms(3);
	relayAlarm_En[n].datah= read_random(0,0,366*n+63);
	delay_ms(3);relayAlarm_En[n].datal= read_random(0,0,366*n+64);
	delay_ms(3);
	relayAlarm_En[n].data=relayAlarm_En[n].datah*256+relayAlarm_En[n].datal;
	relayAlarm_En[n].dataflagh = read_random(0,0,366*n+65);
	delay_ms(3);relayAlarm_En[n].dataflagl = read_random(0,0,366*n+66);
	delay_ms(3);
	relayAlarm_En[n].dataflag =relayAlarm_En[n].dataflagh*256+relayAlarm_En[n].dataflagl;
	
}}  }}
	
	void ReadrelaygateEnData_init(void){			 //读继电器固定任务使能
  u8 n=0;
		for(n=0;n<70;n++){
		if(!relaygateENflag[n]){
	relaygateENflag[n]=1;
	     if(relay_result_data[n].state!=0xee){
  relay_gate_En[n].set = read_random(0,0,366*n+102);			 
	delay_ms(3);relay_gate_En[n].setflag = read_random(0,0,366*n+103);
	delay_ms(3);
	relay_gate_En[n].datah= read_random(0,0,366*n+104);
	delay_ms(3);relay_gate_En[n].datal= read_random(0,0,366*n+105);
	delay_ms(3);
	
	relay_gate_En[n].dataflagh = read_random(0,0,366*n+106);
	delay_ms(3);relay_gate_En[n].dataflagl = read_random(0,0,366*n+107);
	delay_ms(3);
	
	
}}  }}

	void ReadrelayShouData_init(void){			 //读继电器手动数据
  u8 i,n=0;
		for(n=0;n<70;n++){
		if(!relayshouDataflag[n]){
	relayshouDataflag[n]=1;
	     if(relay_result_data[n].state!=0xee){
  shoudong_control[n].channeldatah = read_random(0,0,366*n+11);			 
	delay_ms(3);shoudong_control[n].channeldatal= read_random(0,0,366*n+12);
	delay_ms(3);
	shoudong_control[n].channeldata =shoudong_control[n].channeldatal+256*shoudong_control[n].channeldatah;
	for(i=0;i<8;i++){
		if((shoudong_control[n].channeldata&0x8000)==0x8000){
		shoudong_control[n].type[i]  = read_random(0,0,366*n+13+3*i);
	delay_ms(3);shoudong_control[n].timedatah[i] = read_random(0,0,366*n+14+3*i);
	delay_ms(3);
	shoudong_control[n].timedatal[i] = read_random(0,0,366*n+15+3*i);
			delay_ms(3);
	//shoudong_control[n].channelflag[i] =1; 
}
	shoudong_control[n].channeldata=shoudong_control[n].channeldata <<1;
	}
	shoudong_control[n].channeldata =shoudong_control[n].channeldatal+256*shoudong_control[n].channeldatah;
  }}}}

	void ReadrelaygateData_init(void){
		u8 i,j,n=0;
			for(n=0;n<70;n++){
		if(!relaygateDataflag[n]){
	relaygateDataflag[n]=1;
	     if(relay_result_data[n].state!=0xee){

	relay_gate_data[n].gradient1 = read_random(0,0,366*n+108);   //固定任务开启参数
	delay_ms(3);
	relay_gate_data[n].gradient2 = read_random(0,0,366*n+109);   //
	delay_ms(3);
	relay_gate_data[n].gradient = relay_gate_data[n].gradient1*256+relay_gate_data[n].gradient2; 

		for(i = 0; i < 8; i++){
	  if((relay_gate_data[n].gradient&0x8000)==0x8000)	{
	relay_gate_data[n].relay_gateTask[i].gate_type = read_random(0,0,366*n+110+32*i);
	delay_ms(3);
	relay_gate_data[n].relay_gateTask[i].relay_datah = read_random(0,0,366*n+111+32*i);
	delay_ms(3);relay_gate_data[n].relay_gateTask[i].relay_datal= read_random(0,1,366*n+112+32*i);
	delay_ms(3);
	relay_gate_data[n].relay_gateTask[i].relay_data=relay_gate_data[n].relay_gateTask[i].relay_datah*256+relay_gate_data[n].relay_gateTask[i].relay_datal;
	relay_gate_data[n].relay_gateTask[i].trans_ID = read_random(0,0,366*n+113+32*i);
	delay_ms(3);
	relay_gate_data[n].relay_gateTask[i].set_transh = read_random(0,0,366*n+114+32*i);
	delay_ms(3);relay_gate_data[n].relay_gateTask[i].set_transl= read_random(0,0,366*n+115+32*i);
	delay_ms(3);
	relay_gate_data[n].relay_gateTask[i].set_trans=relay_gate_data[n].relay_gateTask[i].set_transh*256+relay_gate_data[n].relay_gateTask[i].set_transl;

	relay_gate_data[n].relay_gateTask[i].gate_datah = read_random(0,0,366*n+116+32*i);
	delay_ms(3);relay_gate_data[n].relay_gateTask[i].gate_datal= read_random(0,0,366*n+117+32*i);
	delay_ms(3);
	relay_gate_data[n].relay_gateTask[i].gate_data=relay_gate_data[n].relay_gateTask[i].gate_datah*256+relay_gate_data[n].relay_gateTask[i].gate_datal;

	relay_gate_data[n].relay_gateTask[i].watch_datah = read_random(0,0,366*n+118+32*i);
	delay_ms(3);relay_gate_data[n].relay_gateTask[i].watch_datal= read_random(0,0,366*n+119+32*i);
	delay_ms(3);
	relay_gate_data[n].relay_gateTask[i].watch_data=relay_gate_data[n].relay_gateTask[i].watch_datah*256+relay_gate_data[n].relay_gateTask[i].watch_datal;
	relay_gate_data[n].relay_gateTask[i].control_type = read_random(0,0,366*n+120+32*i);
	delay_ms(3);relay_gate_data[n].relay_gateTask[i].set_time= read_random(0,0,366*n+121+32*i);
	delay_ms(3);

	for(j = 0; j < 4; j++){
	   if((relay_gate_data[n].relay_gateTask[i].set_time&0x80)==0x80)  {
	delay_ms(3);relay_gate_data[n].relay_gateTask[i].time_datah1[j] = read_random(0,0,366*n+122+32*i+5*j);
	delay_ms(3);relay_gate_data[n].relay_gateTask[i].time_datal1[j] = read_random(0,0,366*n+123+32*i+5*j);
	//relay_gate_data[0].relay_gateTask[i].time_data1[j]=relay_gate_data[0].relay_gateTask[i].time_datah1[j]*256+relay_gate_data[0].relay_gateTask[i].time_datal1[j];
	delay_ms(3);relay_gate_data[n].relay_gateTask[i].time_datah2[j] = read_random(0,0,366*n+124+32*i+5*j);
	delay_ms(3);relay_gate_data[n].relay_gateTask[i].time_datal2[j] = read_random(0,0,366*n+125+32*i+5*j);
	//relay_gate_data[0].relay_gateTask[i].time_data2[j]=relay_gate_data[0].relay_gateTask[i].time_datah2[j]*256+relay_gate_data[0].relay_gateTask[i].time_datal2[j];
	delay_ms(3);    
	relay_gate_data[n].relay_gateTask[i].set_data[j] = read_random(0,0,366*n+126+32*i+5*j);
	delay_ms(3);
	}
	relay_gate_data[n].relay_gateTask[i].set_time=relay_gate_data[n].relay_gateTask[i].set_time <<1;
	}
	relay_gate_data[n].relay_gateTask[i].set_time= read_random(0,0,366*n+121+32*i);
	delay_ms(3);
	 }
	  relay_gate_data[n].gradient=relay_gate_data[n].gradient <<1;
	 }
	relay_gate_data[n].gradient = relay_gate_data[n].gradient1*256+relay_gate_data[n].gradient2; 
	}}}}
	
	void ReadtransalarmEN_init(void){		  //变送器终端报警使能
	u8 n=0;
	for(n = 0; n < second_device.trans_data; n++){
		if(!transalarmENflag[n]){
	transalarmENflag[n]=1;
			if(transID[n].state!=0xee){
		transAlarm_En[n].set = read_random(0,1,708*n+690);			 
	delay_ms(3);transAlarm_En[n].setflag = read_random(0,1,708*n+691);
	delay_ms(3);
	transAlarm_En[n].datah= read_random(0,1,708*n+692);
	delay_ms(3);transAlarm_En[n].datal= read_random(0,0,708*n+693);
	delay_ms(3);
	transAlarm_En[n].data=transAlarm_En[n].datah*256+transAlarm_En[n].datal;
	transAlarm_En[n].dataflagh = read_random(0,1,708*n+694);
	delay_ms(3);transAlarm_En[n].dataflagl = read_random(0,1,708*n+695);
	delay_ms(3);
	transAlarm_En[n].dataflag =transAlarm_En[n].dataflagh*256+transAlarm_En[n].dataflagl;
	
}}  }}
	void ReadtranspumpEN_init(void){		  //水泵配置
		u8 n=0;
	for(n = 0; n < second_device.trans_data; n++){
		if(!transpumpENflag[n]){
	transpumpENflag[n]=1;
			if(transID[n].state!=0xee){
	transpump_En[n].datah= read_random(0,1,708*n+696);
	delay_ms(3);transpump_En[n].datal= read_random(0,1,708*n+697);
	delay_ms(3);
	transpump_En[n].data=transpump_En[n].datah*256+transpump_En[n].datal;
	transpump_En[n].dataflagh[0] = read_random(0,1,708*n+698);
	delay_ms(3);transpump_En[n].dataflagl[0] = read_random(0,1,708*n+699);
	delay_ms(3);
	transpump_En[n].dataflag[0] =transpump_En[n].dataflagh[0]*256+transpump_En[n].dataflagl[0];
	if((transpump_En[n].dataflag[0]&0x4000)==0x4000){
	   PBout(6)=0; 
	            }	
  else PBout(6)=1; 	
}}}}
