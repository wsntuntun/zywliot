#include <string.h>
#include <math.h>

#include "dataprocess.h"
#include "GPRS.h"
#include "delay.h"
#include "24cxx.h"
#include "rtc.h"
#include "init.h"
#include "timer.h"
#include "includes.h"
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
//********************************************************************************

/*Declare SFR associated with the ADC */ 



////////////////////////
/////// 函数声明 ///////
////////////////////////
void delay1_ms(unsigned int time);
extern struct uart_data_buff uart4_buff;
extern struct uart_data_buff uart1_buff;
extern struct Trans_control control_info[4];
extern struct data_alarm data_alarm_value[4];
extern struct Intelligent_control Intelligent_value[4];

//extern struct ungate_Fixedopen relay_ungate_data[80];
extern struct gate_Fixedopen relay_gate_data[4];
extern struct yandu_data device_yandu[4];
extern struct trans_query_date transID[4];
extern struct data_trans_result trans_query_data[4];
extern struct id_data device_id;
extern struct trans_alarm_enable  transAlarm_En[4];
extern struct trans_pump  transpump_En[4];
extern struct shou_control shoudong_control[4];
extern struct Trans_control control_info[4];
extern struct second_device_data second_device;
extern struct data_relay_result relay_result_data[4];
extern struct relay_query_date relay_date[4];

extern u8 Time_GetData[32];
extern struct rtc_time systmtime, settmtime,testtime;
extern  u16 Transdata[16];

extern u8 tim;
extern u8  controlflag;
extern u8 state[16];
extern u8  transflag[70],relayflag[70];
////////////////////////////////////////////////////////////////
///////////////////////  延时函数    ///////////////////////////
/////////////////////////////////////////////////////////////////
//// 1ms延时函数
void delay1_ms(unsigned int time)
{
	 unsigned int i,j;
	 for(i=0;i<time;i++)
	 for(j=0;j<123;j++);
}

/**********************************************************
 *
 * @file	: sence_send(u8 addr)
 * @param	: u8 addr: 指向原始数据的u8数据类型指针
 *	
 * @retval	: None
 * @brief	: 实时数据处理
 *
 **********************************************************/

void sence_send(u8 addr)
{
//u8 i= 0;
//	u8 k= 0;
	u16 temp;
	uart1_buff.tx_len = 0x08;
	uart1_buff.tx_data_buff[0] = addr;		//地址	
	uart1_buff.tx_data_buff[1] = 0x03;			
	uart1_buff.tx_data_buff[2]=0x00;
	uart1_buff.tx_data_buff[3]=0x00;
	uart1_buff.tx_data_buff[4]=0x00;
	if(addr<0x10){
	uart1_buff.tx_data_buff[5]=0x05;
	             }
	else 
		          {
	uart1_buff.tx_data_buff[5]=0x03;
	             }
	temp = CRC16((u8 *)uart1_buff.tx_data_buff, (uart1_buff.tx_len-2));
	uart1_buff.tx_data_buff[6] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
	uart1_buff.tx_data_buff[7] = (u8)(temp&0x00ff); 		//CRC校验低位
	
	USART3_SendTxData1( uart1_buff.tx_data_buff, uart1_buff.tx_len );
}
/**********************************************************
 *
 * @file	: trans_send(u8 addr,u8 probset1,u8 probset2)
 * @param	: u8 addr: 指向原始数据的u8数据类型指针
 *	
 * @retval	: None
 * @brief	: 变送器数据查询函数
 *
 **********************************************************/

void trans_send(u8 addr,u8 manudata,u8 type)
{
u16 temp;
	uart1_buff.tx_len = 0x08;
	uart1_buff.tx_data_buff[0] = addr+16*(manudata*2-4+type);		//正常	
	uart1_buff.tx_data_buff[1] = 0x03;		//正常	
	uart1_buff.tx_data_buff[2]=0x00;
	uart1_buff.tx_data_buff[3]=0x00;
	uart1_buff.tx_data_buff[4]=0x00;
	//manudata=addr/16+1;
	//probe=addr%16;
	switch(manudata){
		    case 0x01:   //自制
				          break;
				case 0x02:   //联祥膜法             
					 switch(addr){
				          case 0x01:   //DO
										uart1_buff.tx_data_buff[5]=0x05;
				             break;
				          case 0x02:   //PH 
										uart1_buff.tx_data_buff[5]=0x03;
									   break; 
									case 0x03:   //ORP
				             break;
				          case 0x04:   //叶绿素 
									   break; 
				          default: 
				             break;
					               }
				          break; 
				case 0x03:   //联祥荧光法
				      switch(addr){
				          case 0x01:   //DO
										uart1_buff.tx_data_buff[5]=0x05;
				             break;
				          case 0x02:   //PH 
										uart1_buff.tx_data_buff[5]=0x03;
									   break; 
									case 0x03:   //ORP
				             break;
				          case 0x04:   //叶绿素 
									   break; 
				          default: 
				             break;
					               }
				          break; 
				case 0x04:   //普贝斯膜法
				      switch(addr){
				          case 0x01:   //DO
										uart1_buff.tx_data_buff[5]=0x05;
				             break;
				          case 0x02:   //PH   
				            uart1_buff.tx_data_buff[5]=0x03;
									   break; 
									case 0x03:   //ORP
				             break;
				          case 0x04:   //叶绿素  
				             
									   break; 
				          default: 
				             break;
					               }
				          break; 
				case 0x05:   //普贝斯荧光法
				        switch(addr){
				          case 0x01:   //DO
										uart1_buff.tx_data_buff[5]=0x05;
				             break;
				          case 0x02:   //PH   
				             uart1_buff.tx_data_buff[5]=0x03;
									   break; 
									case 0x03:   //ORP
				             break;
				          case 0x04:   //叶绿素  
				             
									   break; 
				          default: 
				             break;
					               }
				          break; 
				case 0x06:   //科雷特膜法
				       switch(addr){
				          case 0x01:   //DO
										uart1_buff.tx_data_buff[5]=0x05;
				             break;
				          case 0x02:   //PH 
                     uart1_buff.tx_data_buff[5]=0x03;										
									   break; 
									case 0x03:   //ORP
				             break;
				          case 0x04:   //叶绿素  
				            
									   break; 
				          default: 
				             break;
					               }
				          break; 
				case 0x07:   //科雷特荧光法
				      switch(addr){
				          case 0x01:   //DO
										uart1_buff.tx_data_buff[5]=0x05;
				             break;
				          case 0x02:   //PH
										uart1_buff.tx_data_buff[5]=0x03;
									   break; 
									case 0x03:   //ORP
				             break;
				          case 0x04:   //叶绿素  
				            
									   break; 
				          default: 
				             break;
					               }
				          break; 
				default: 
				          break; 
					      }
	temp = CRC16((u8 *)uart1_buff.tx_data_buff, (uart1_buff.tx_len-2));
	uart1_buff.tx_data_buff[6] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
	uart1_buff.tx_data_buff[7] = (u8)(temp&0x00ff); 		//CRC校验低位
	
	USART3_SendTxData1( uart1_buff.tx_data_buff, uart1_buff.tx_len );
}
/**********************************************************
 *
 * @file	: relay_send(void)
 * @param	: u8 addr: 指向原始数据的u8数据类型指针
 *	
 * @retval	: None
 * @brief	: 继电器终端控制以及查询函数
 *
 **********************************************************/

void relay_send(u8 addr)
{
u8 i= 0;
u8 k= 0;
	u16 temp;
	u16 relayData=0;
	u16 setedflag =0;
	u16 tempdata1,tempdata2,tempdata3;
	tempdata1=relay_date[addr-1].data1[1]*256+relay_date[addr-1].data2[1];
	tempdata2=relay_date[addr-1].data1[2]*256+relay_date[addr-1].data2[2];
	tempdata3=relay_date[addr-1].data1[3]*256+relay_date[addr-1].data2[3];
	for(i=0;i<relay_result_data[i].relaydata;i++) {
	  if(shoudong_control[addr-1].shouflag[i]){
		  if(shoudong_control[addr-1].shouflag[i]==1){
					relayData|=(1<<(15-i)); 
				  tempdata3 |=(1<<(15-i)) ;
			                                          }
			else {relayData &=~(1<<(15-i));
					  tempdata3 &=~(1<<(15-i)) ;	}																		 
		                                      }
		else {if(shoudong_control[addr-1].guflag[i]){
			      if(shoudong_control[addr-1].guflag[i]==1){
					    relayData |=(1<<(15-i)); 
							tempdata2 |=(1<<(15-i)) ;
			                                          }
			      else {relayData &=~(1<<(15-i));
								  tempdata2 &=~(1<<(15-i)) ;	}	
		                                          }
		     else {if(shoudong_control[addr-1].zhiflag[i]){
		             if(shoudong_control[addr-1].zhiflag[i]==1){
					       relayData |=(1<<(15-i));
                 tempdata3 |=(1<<(15-i)) ;									 
			                                          }
			           else {relayData &=~(1<<(15-i));
											 tempdata3 &=~(1<<(15-i)) ;	}	
		                                                 }
				       else {relayData &=~(1<<(15-i));
							       tempdata1 &=~(1<<(15-i)) ;
							       tempdata2 &=~(1<<(15-i)) ;
								     tempdata3 &=~(1<<(15-i)) ;
							      }  
	            }
	       }
		                }
	relay_date[addr-1].data1[0]=relayData/256;         //最终状态
	relay_date[addr-1].data2[0]=relayData%256;	
	relay_date[addr-1].data1[1]=tempdata1/256;         //智能控制状态
  relay_date[addr-1].data2[1]=tempdata1%256;
	relay_date[addr-1].data1[2]=tempdata2/256;         //定时控制状态
  relay_date[addr-1].data2[2]=tempdata3%256;										
	relay_date[addr-1].data1[3]=tempdata3/256;         //手动控制状态
  relay_date[addr-1].data2[3]=tempdata3%256; 		
for(k=0;k<relay_result_data[i].relaydata;k++){       //通道12345678分别对应PD0134567 PB5
     if(k>1){                                //通道345678
			  if(k==7){                             //通道八
					 if((relayData&0x8000)==0x8000){
				       if ((PBin(5))==1){
									PBout(5)=0;               //开
										 delay_ms(5);
								                    }                   
                                        }
					else{
				 	     if ((PBin(5))==0){
									        PBout(5)=1;    //关
												 delay_ms(5);
								                    }
						  }
				            }
				else{
				 if((relayData&0x8000)==0x8000){
				       if ((PDin((k+1)))==1){
									//relay_result_data[0].data[4] &=~(1<<(15-k));
									PDout((k+1))=0;               //开
										 delay_ms(5);
								                    }                   
                                        }
					else{
				 	     if ((PDin((k+1)))==0){
									        PDout((k+1))=1;    //关
												 delay_ms(5);
								                    }
						  }
						}
				  setedflag |=(1<<(15-k));
						}
	    else{                                  //通道12
	        if((relayData&0x8000)==0x8000){
				       if ((PDin(k))==1){
									//relay_result_data[0].data[4] &=~(1<<(15-k));
									PDout(k)=0;               //开
										 delay_ms(5);
								                    }                   
                                        }
					else{
				 	     if ((PDin(k))==0){
									        PDout(k)=1;    //关
												 delay_ms(5);
								                }
						  }
				  setedflag |=(1<<(15-k));
		        }
          relayData=relayData<<1; 			 					
								                            }	
}
/**********************************************************
 *
 * @file	: jiaozhun_send(u8 addr,u8 type)
 * @param	: u8 addr: 指向原始数据的u8数据类型指针
 *	
 * @retval	: None
 * @brief	: 校准发送函数
 *
 **********************************************************/
void jiaozhun_send(u8 addr,u8 type)
{
	u16 temp;
	u8 probe;
	probe=addr%16;                  //探头类型（溶氧、PH....）
	uart1_buff.tx_len = 0x08;
	uart1_buff.tx_data_buff[0] = addr;		//
	uart1_buff.tx_data_buff[1] = 0x06;		//	
	uart1_buff.tx_data_buff[2]=0x00;
	uart1_buff.tx_data_buff[5]=type;      
	uart1_buff.tx_data_buff[4]=0x00;
	switch(probe){
				          case 0x01:   //DO
										 uart1_buff.tx_data_buff[3]=0x07;
				             break;
				          case 0x02:   //PH
										uart1_buff.tx_data_buff[3]=0x04;
									   break; 
									case 0x03:   //ORP
				             break;
				          case 0x04:   //叶绿素  
				            
									   break; 
				          default: 
				             break;
					     }
	temp = CRC16((u8 *)uart1_buff.tx_data_buff, (uart1_buff.tx_len-2));
	uart1_buff.tx_data_buff[6] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
	uart1_buff.tx_data_buff[7] = (u8)(temp&0x00ff); 		//CRC校验低位
	
	USART3_SendTxData1( uart1_buff.tx_data_buff, uart1_buff.tx_len );
							 delay_ms(10);
}
/**********************************************************
 *
 * @file	: jiaozhun_Read_back(u8 prob,u8 type,u8 result)
 * @param	: u8 addr: 指向原始数据的u8数据类型指针
 *	
 * @retval	: None
 * @brief	: 校准返回函数
 *
 **********************************************************/
 void jiaozhun_Read_back(u8 prob1,u8 prob2,u8 type,u8 result)
{
	  u16 temp;
		uart4_buff.tx_len = 18;
	  uart4_buff.tx_data_buff[0] = device_id.year_datah;
		uart4_buff.tx_data_buff[1] = device_id.year_datal;
		uart4_buff.tx_data_buff[2] = device_id.sn_datah;
		uart4_buff.tx_data_buff[3] = device_id.sn_datal;					// 	
		uart4_buff.tx_data_buff[4] =0x03;
		uart4_buff.tx_data_buff[5] =0x40 ;
		uart4_buff.tx_data_buff[6] = 0x0B;	 //长度
		uart4_buff.tx_data_buff[7] = 0x08; 	
		uart4_buff.tx_data_buff[8] = 0xD0;
		uart4_buff.tx_data_buff[9] = 0x00;
	  uart4_buff.tx_data_buff[10] = 0x01;
		uart4_buff.tx_data_buff[11] = prob1;
		uart4_buff.tx_data_buff[12] = prob2;
		uart4_buff.tx_data_buff[13] = type;
		uart4_buff.tx_data_buff[14] = 0x00;
		uart4_buff.tx_data_buff[15] = result;

		temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
		uart4_buff.tx_data_buff[16] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
		uart4_buff.tx_data_buff[17] = (u8)(temp&0x00ff); 		//CRC校验低位

	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}

/**********************************************************
 *
 * @file	: yandu_Read_back(u8 data1,u8 data2)
 * @param	: u8 addr: 指向原始数据的u8数据类型指针
 *	
 * @retval	: None
 * @brief	: 盐度读取返回函数
 *
 **********************************************************/
void yandu_Read_back(u8 data1,u8 data2)
{
	    u16 temp;
		uart4_buff.tx_len = 15;
	  uart4_buff.tx_data_buff[0] = device_id.year_datah;
		uart4_buff.tx_data_buff[1] = device_id.year_datal;
		uart4_buff.tx_data_buff[2] = device_id.sn_datah;
		uart4_buff.tx_data_buff[3] = device_id.sn_datal;					// 	
		uart4_buff.tx_data_buff[4] =0x03;
		uart4_buff.tx_data_buff[5] =0x40 ;
		uart4_buff.tx_data_buff[6] = 0x08;	 //长度
		uart4_buff.tx_data_buff[7] = 0x07; 	
		uart4_buff.tx_data_buff[8] = 0xD0;
		uart4_buff.tx_data_buff[9] = 0x00;
	    uart4_buff.tx_data_buff[10] = 0x01;
		uart4_buff.tx_data_buff[11] = data1;
		uart4_buff.tx_data_buff[12] = data2;

		temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
		uart4_buff.tx_data_buff[13] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
		uart4_buff.tx_data_buff[14] = (u8)(temp&0x00ff); 		//CRC校验低位

	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}
void yandu_Write_back(void)
{
	    u16 temp;
		uart4_buff.tx_len = 13;
	  uart4_buff.tx_data_buff[0] = device_id.year_datah;
		uart4_buff.tx_data_buff[1] = device_id.year_datal;
		uart4_buff.tx_data_buff[2] = device_id.sn_datah;
		uart4_buff.tx_data_buff[3] = device_id.sn_datal;					// 	
		uart4_buff.tx_data_buff[4] =0x16;
		uart4_buff.tx_data_buff[5] =0x40 ;
		uart4_buff.tx_data_buff[6] = 0x08;	 //长度
		uart4_buff.tx_data_buff[7] = 0x07; 	
		uart4_buff.tx_data_buff[8] = 0xD0;
		uart4_buff.tx_data_buff[9] = 0x00;
	    uart4_buff.tx_data_buff[10] = 0x01;
		temp = CRC16((u8 *)uart4_buff.tx_data_buff, (uart4_buff.tx_len-2));
		uart4_buff.tx_data_buff[11] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
		uart4_buff.tx_data_buff[12] = (u8)(temp&0x00ff); 		//CRC校验低位

	USART1_SendTxData1((u8 *)uart4_buff.tx_data_buff, uart4_buff.tx_len);
}
 void yandu_write(u8 addr1,u8 set1,u8 set2)
{
	  u16 temp;
		uart1_buff.tx_len = 8;
		uart1_buff.tx_data_buff[0] = addr1;
		uart1_buff.tx_data_buff[1] =0x06;
		uart1_buff.tx_data_buff[2] =0x00 ;
		uart1_buff.tx_data_buff[3] = 0x0a;	 //长度
		uart1_buff.tx_data_buff[4] = set1;	 //长度
		uart1_buff.tx_data_buff[5] = set2;
		temp = CRC16((u8 *)uart1_buff.tx_data_buff, (uart1_buff.tx_len-2));
		uart1_buff.tx_data_buff[6] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
		uart1_buff.tx_data_buff[7] = (u8)(temp&0x00ff); 		//CRC校验低位

	USART1_SendTxData1((u8 *)uart1_buff.tx_data_buff, uart1_buff.tx_len);
}
 void yandu_read(u8 addr1)
{
	  u16 temp;
		uart1_buff.tx_len = 8;
		uart1_buff.tx_data_buff[0] = addr1;
		uart1_buff.tx_data_buff[1] =0x03;
		uart1_buff.tx_data_buff[2] =0x00 ;
		uart1_buff.tx_data_buff[3] = 0x0a;	 //长度
		uart1_buff.tx_data_buff[4] =0x00;	 //长度
		uart1_buff.tx_data_buff[5] = 0x01;
		temp = CRC16((u8 *)uart1_buff.tx_data_buff, (uart1_buff.tx_len-2));
		uart1_buff.tx_data_buff[6] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
		uart1_buff.tx_data_buff[7] = (u8)(temp&0x00ff); 		//CRC校验低位

	USART3_SendTxData1((u8 *)uart1_buff.tx_data_buff, uart1_buff.tx_len);
}

void pump(void){
	if(PEin(14)==0){if(transpump_En[0].dataflag[0]&0x4000){
		                  transpump_En[0].dataflag[2] &=~(1<<14);
		                                                     }
		                 else transpump_En[0].dataflag[2] |=(1<<14);
		               transpump_En[0].dataflag[1] |=(1<<14);}
	  else {if(transpump_En[0].dataflag[0]&0x4000){
		                  transpump_En[0].dataflag[2] |=(1<<14);
		                                             }
		        else transpump_En[0].dataflag[2] &=~(1<<14);
			     transpump_En[0].dataflag[1]&=~(1<<14);
	        }	
              }
void relay(void){         //输出检测PC0PC3PC2PC1PA0PA5PA4PA1对应通道1-8
	u8 i=0;
	u16 temp;
	temp=relay_date[0].data[0];
	  if(PAin(0)==0)relay_result_data[0].data[2] |=(1<<11);
	  else {
	        relay_result_data[0].data[2] &=~(1<<11);
	    if(temp&0x0800){relay_result_data[0].data[3] |=(1<<11);}
			    else {relay_result_data[0].data[3] &=~(1<<11);}
					//	    //关	
	       }
		 OSTimeDlyHMSM(0,0,0,1);
	  if(PAin(1)==0)relay_result_data[0].data[2] |=(1<<8);
	  else {
	        relay_result_data[0].data[2] &=~(1<<8);
			    if(temp&0x0100){relay_result_data[0].data[3] |=(1<<8);}
			    else {relay_result_data[0].data[3] &=~(1<<8);}
			  //	  关	
	       }
		OSTimeDlyHMSM(0,0,0,1);
	  for(i = 9; i < 11; i++){
         if(PAin((i-5))==0){relay_result_data[0].data[2] |=(1<<i);
		          delay_ms(2); } 
	       else {
	            relay_result_data[0].data[2] &=~(1<<i);
					   if(temp&0x0200){relay_result_data[0].data[3] |=(1<<i);}
			       else {relay_result_data[0].data[3] &=~(1<<i);}
		//				   //关	
       	      //delay_ms(2);
				      }
				 temp=temp>>1;
	                         }
		OSTimeDlyHMSM(0,0,0,1);
	  if(PCin(0)==0)relay_result_data[0].data[2]|=(1<<15) ;
	  else {
			 relay_result_data[0].data[2] &=~(1<<15);
			if(temp&0x8000){relay_result_data[0].data[3] |=(1<<15);}
			else {relay_result_data[0].data[3] &=~(1<<15);}
					//	  //关	
	       }
		OSTimeDlyHMSM(0,0,0,1);
		temp=relay_date[0].data[0];
	  for(i = 12; i < 15; i++){
        if(PCin((i-11))==0)relay_result_data[0].data[2]|=(1<<i) ;
        else {
           relay_result_data[0].data[2] &=~(1<<i);
					 if(temp&0x1000){relay_result_data[0].data[3] |=(1<<i);}
			     else {relay_result_data[0].data[3] &=~(1<<i);}
		//			  //关	
              }
			temp=temp>>1;
	                          }
	    OSTimeDlyHMSM(0,0,0,1);
              }
/**********************************************************
 *
 * @file	: sence_rec(void)
 * @param	: u8 *GasData: 指向原始数据的u8数据类型指针
 *	
 * @retval	: None
 * @brief	: 传感数据处理
 *
 **********************************************************/
void sence_rec(void)
{
	u16 temp;
	u8 n=0;
	u8 i=0;
	u8 m=0;
	u8 Trans;
//	u8 flag[4];
	u16 datatem;
	u16 datapump;
	u8 crc_datah, crc_datal;
	u8 comend_Len, comend_Buff[100]; 	//Modbus帧数据BUFF
	

		comend_Len = uart1_buff.rx_len;
		mem_copy_byte((u8 *)uart1_buff.rx_data_buff, (u8 *)comend_Buff, comend_Len);//缓存数据

		//UART5RxFlag = 0x00;	//上一帧数据缓存完成，允许接收下一帧数据

		//帧设备ID数据解析
	    //Trans=comend_Buff[0]/0x10+1;
         //帧数据长度解析
	Trans=comend_Buff[0]%0x10;
		if (!((comend_Buff[1]==0x03)||(comend_Buff[1]==0x06)))	//如果数据长度不匹配，则丢弃本帧数据
		{      
			transID[Trans-1].state=TRANS_NOT_OPER;
			return ;
		}
	else {
			if(comend_Buff[1]==0x03){
		if (comend_Buff[2] != (comend_Len-5))	//如果数据长度不匹配，则丢弃本帧数据
		{
		switch(Trans){
		        case 0x01:
				      trans_query_data[Trans-1].decimal[0]=PROBE_ERROR_LEN;
						  trans_query_data[Trans-1].decimal[1]=PROBE_ERROR_LEN;
				          break;
				case 0x02:
				      trans_query_data[Trans-1].decimal[2]=PROBE_ERROR_LEN;
				          break; 
				default: 
				          break; 
					}
			return ;
		}                        }
			}
		//CRC校验帧数据	
		temp = CRC16((u8 *)comend_Buff, (comend_Len-2));
		crc_datah = (u8)((temp>>8)&0x00ff);
		crc_datal = (u8)(temp&0x00ff);
		
		if ((comend_Buff[comend_Len-2] != crc_datah)||(comend_Buff[comend_Len-1] != crc_datal))
		{	//CRC检验错误，则返回错误信息
		transID[Trans-1].state=TRANS_NOT_OPER;
		return ;	
		}
	switch(comend_Buff[1]){
		        case 0x03:
				switch(Trans){
		        case 0x01:  
						switch(comend_Buff[2]){
							case 0x02:
						   device_yandu[Trans-1].yandu_data1 =comend_Buff[3];
	             device_yandu[Trans-1].yandu_data2=comend_Buff[4];
						   yandu_Read_back(comend_Buff[3],comend_Buff[4]);
						  break;
					    case 0x0a:
				      transID[Trans-1].state=0xE1;
		               trans_query_data[Trans-1].integerh[0]=comend_Buff[3];
                   trans_query_data[Trans-1].integerl[0]=comend_Buff[4];
									 trans_query_data[Trans-1].integer[0] = trans_query_data[Trans-1].integerh[0]*256+trans_query_data[Trans-1].integerl[0]; 
			             trans_query_data[Trans-1].mvdatah[0]=0;
			             trans_query_data[Trans-1].mvdatal[0]=0; 
                   trans_query_data[Trans-1].decimal[0]=0;
									 trans_query_data[Trans-1].integerh[1]=comend_Buff[11];
                   trans_query_data[Trans-1].integerl[1]=comend_Buff[12];
									 trans_query_data[Trans-1].integer[1] = trans_query_data[Trans-1].integerh[1]*256+trans_query_data[Trans-1].integerl[1]; 
			             trans_query_data[Trans-1].mvdatah[1]=comend_Buff[7];
			             trans_query_data[Trans-1].mvdatal[1]=comend_Buff[8]; 
                   trans_query_data[Trans-1].decimal[1]=0;
									 for(n=0;n<2;n++) { 
									 if(trans_query_data[Trans-1].integer[n]<(data_alarm_value[Trans-1].integerh_temp1[n]*256
									   +data_alarm_value[Trans-1].integerl_temp1[n]+data_alarm_value[Trans-1].integerh_temp0[n]*256
									   +data_alarm_value[Trans-1].integerl_temp0[n])){
			                                  //超出报警门限
		                 trans_query_data[Trans-1].flag[0]=0x02;
		                    //开启报警	超下限
	                         	                                               }
         	         else {
				             if(trans_query_data[Trans-1].integer[n]<(data_alarm_value[Trans-1].integerh_temp2[n]*256
					           +data_alarm_value[Trans-1].integerl_temp2[n]-data_alarm_value[Trans-1].integerh_temp0[n]*256
					           -data_alarm_value[Trans-1].integerl_temp0[n])){
	                //超出报警门限
	            	     trans_query_data[Trans-1].flag[n]=0x01;
	                   //开启报警	 超上限
		                                                                     }
	                  else {		   //正常
				              trans_query_data[Trans-1].flag[n]=0;
					                }
              	        }
						                         }
							break;
             case 0x08:
						      //jiaozhun_Read_back(0x40,comend_Buff[9],comend_Buff[10]);
						     break;																		 
					    default: 
				         break;
								 }
					  break;
       case 0x02:     //PH
				    switch(comend_Buff[2]){
					     case 0x01:
						        break;
					     case 0x06:
				        trans_query_data[Trans-1].integerh[2] = comend_Buff[5];
              	trans_query_data[Trans-1].integerl[2] = comend_Buff[6];
				        trans_query_data[Trans-1].integer[2] = trans_query_data[Trans-1].integerh[2]*256+trans_query_data[Trans-1].integerl[2]; 
				        trans_query_data[Trans-1].mvdatah[2] = comend_Buff[7];
              	trans_query_data[Trans-1].mvdatal[2] = comend_Buff[8];
				        trans_query_data[Trans-1].decimal[2]=0;
							  if(trans_query_data[Trans-1].integer[2]<(data_alarm_value[Trans-1].integerh_temp1[2]*256
									   +data_alarm_value[Trans-1].integerl_temp1[2]+data_alarm_value[Trans-1].integerh_temp0[2]*256
									   +data_alarm_value[Trans-1].integerl_temp0[2])){
			                                  //超出报警门限
		                 trans_query_data[Trans-1].flag[2]=0x02;
		                    //开启报警	超下限
	                         	                                       }
         	         else {
				             if(trans_query_data[Trans-1].integer[2]<(data_alarm_value[Trans-1].integerh_temp2[2]*256
					           +data_alarm_value[Trans-1].integerl_temp2[2]-data_alarm_value[Trans-1].integerh_temp0[2]*256
					           -data_alarm_value[Trans-1].integerl_temp0[2])){
	                //超出报警门限
	            	     trans_query_data[Trans-1].flag[2]=0x01;
	                   //开启报警	 超上限
		                                                                     }
	                  else {		   //正常
				              trans_query_data[Trans-1].flag[2]=0;
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
				    break;
				case 0x06:
						switch(Trans){
		                  case 0x01:  
						switch(comend_Buff[3]){
				   	      case 0x07:
										if(comend_Buff[5]>10){ 
																  if(comend_Buff[5]==16){
																		for (i=0;i<6;i++){
		                                 uart1_buff.tx_data_buff[i] = comend_Buff[i];
	                                                   }
																		uart1_buff.tx_len=8;
																		uart1_buff.tx_data_buff[5]=19;
                                    temp = CRC16((u8 *)uart1_buff.tx_data_buff, (uart1_buff.tx_len-2));
		                                uart1_buff.tx_data_buff[6] = (u8)((temp>>8)&0x00ff);	//CRC校验高位
		                                uart1_buff.tx_data_buff[7] = (u8)(temp&0x00ff); 		//CRC校验低位
	                                  USART1_SendTxData1((u8 *)uart1_buff.tx_data_buff, uart1_buff.tx_len);																		
																	                      }
																	else {jiaozhun_Read_back(0x40,0,comend_Buff[5],0x01);}
						                                       }
											else{jiaozhun_Read_back(0x40,0,comend_Buff[5],0x01);}
						        break;
					        case 0x0A:
							      //yandu_Read_back(comend_Buff[6],comend_Buff[7]); 
									  yandu_Write_back();
							      break;
                  case 0x05:
							      //TransDset_Read_back(comend_Buff[6]); 
									  //transflag[comend_Buff[2]]=0;
							      break;	
							    default: 
				            break;			  }
						   break;

						   case 0x02:  
						switch(comend_Buff[3]){
							    case 0x01:
						       break;
								  case 0x04:
						       jiaozhun_Read_back(0x20,0,comend_Buff[5],0x01);
				   	       break;
					        case 0x06:
							     // RelayDset_Read_back(comend_Buff[6]); 
							      break;			  
							 default: 
				             break;			 }
						   break;

						   default: 
				             break;
						   }
					    break;   
			    default: 
				    break;}
				
	//Trans=comend_Buff[0]/0x10+1;
        
	  //数据状态
	//	Write_back( uart1_buff.tx_data_buff, uart1_buff.tx_len );	 
}

void relayshoutask(void)                //手动控制处理函数
{	u8 i,m=0;
	u16 temptask;
	//u16 tempdata;
	time_get(Time_GetData);
		for(i=0;i<second_device.relay_data;i++) {
			  if(relay_result_data[i].state!=0xee){
					temptask=shoudong_control[i].channeldata;
				//	tempdata=relay_date[i].data1[3]*256+relay_date[i].data2[3];
					  for(m=0;m<16;m++){                   //手动控制继电器
					       if((temptask&0x8000)==0x8000){
		                  if((Time_GetData[4]*60+Time_GetData[5])<(shoudong_control[i].timedatah[m]*60+shoudong_control[i].timedatal[m]))	{					 
		                                                //发送手动继电器通道开启命令
												 if(shoudong_control[i].type[m]){
													//tempdata |=(1<<(15-m)) ;
																								 
					               shoudong_control[i].shouflag[m]=1; }  //开
												 else {
					               shoudong_control[i].shouflag[m]=2; }   //关
											                           }
								                                      
					           else {shoudong_control[i].shouflag[m]=0;   //
					                 shoudong_control[i].channeldata &=~(1<<(15-m)) ;
											     //tempdata &=~(1<<(15-m)) ;
	                    shoudong_control[i].channeldatah=shoudong_control[i].channeldata/256;
						          shoudong_control[i].channeldatal=shoudong_control[i].channeldata%256;
						          write_byte(0,0,366*i+11,shoudong_control[i].channeldatah);
	                    delay_ms(3);
						          write_byte(0,0,366*i+12,shoudong_control[i].channeldatal);
	                    delay_ms(3);
					                 }		
								                             }
								            temptask=temptask<<1;
						                }
							//relay_date[i].data1[3]=tempdata/256;
              //relay_date[i].data1[3]=tempdata/256; 	
											}
												}
											}

void relaygutask(void)            //固定时间任务处理函数
{	u8 i,m,j,k=0;
	u16 temptask;
	u16 tempdata;
	//u16 data;
	u8 temptime;
	u8 transid,transtype;
	time_get(Time_GetData);
		for(i=0;i<second_device.relay_data;i++) {
			  if(relay_result_data[i].state!=0xee){
					                        //固定时间控制继电器
						temptask=relay_gate_data[i].gradient;
						  for(k = 0; k < 8; k++){      //固定任务号
	            if((temptask&0x8000)==0x8000){
								switch(relay_gate_data[i].relay_gateTask[k].gate_type){
		        case 0x01:       //有门限
				          transid=relay_gate_data[i].relay_gateTask[k].trans_ID;
						  for(m=0;m<16;m++){
						      tempdata =relay_gate_data[i].relay_gateTask[k].set_trans;
								  if((tempdata&0x8000)==0x8000){
										transtype=m;
										break;
									                             }
									 tempdata=tempdata<<1;
									             }
							for(m=0;m<16;m++){      
						           tempdata =relay_gate_data[i].relay_gateTask[k].relay_data;     //有固定任务的通道
									if((tempdata&0x8000)==0x8000){
												 temptime=relay_gate_data[i].relay_gateTask[k].set_time;
												for(j=0;j<4;j++){
													if((temptime&0x80)==0x80)  {
		                        if(((Time_GetData[4]*60+Time_GetData[5])>
									 (relay_gate_data[i].relay_gateTask[k].time_datah1[j]*60+relay_gate_data[i].relay_gateTask[k].time_datal1[j]))
								  &&((Time_GetData[4]*60+Time_GetData[5])<
									 (relay_gate_data[i].relay_gateTask[k].time_datah1[j]*60+relay_gate_data[i].relay_gateTask[k].time_datal1[j])))
								  {	
										if(relay_gate_data[i].relay_gateTask[k].control_type==1){
												if(trans_query_data[transid-1].integer[transtype]<
													(relay_gate_data[i].relay_gateTask[k].gate_data-relay_gate_data[i].relay_gateTask[k].watch_data)
												  ){
									          if	(!shoudong_control[i].shouflag[m]){									
		                           if(relay_gate_data[i].relay_gateTask[k].set_data[j]){
																 // data |=(1<<(15-m)) ;
					                        shoudong_control[i].guflag[m]=1; }  //开
												      else{
					                        shoudong_control[i].guflag[m]=2; }   //关
										             //记录该继电器通道有固定任务
										             break;                               }
										       else          {
													         shoudong_control[i].guflag[m]=0;
													                 }
													 }}
										  else{
											if(relay_gate_data[i].relay_gateTask[k].control_type==2){
												if(trans_query_data[transid-1].integer[transtype]>
													(relay_gate_data[i].relay_gateTask[k].gate_data+relay_gate_data[i].relay_gateTask[k].watch_data)
												  ){
									          if	(!shoudong_control[i].shouflag[m]){									
		                           if(relay_gate_data[i].relay_gateTask[k].set_data[j]){
					                        shoudong_control[i].guflag[m]=1; }  //开
												      else{
					                        shoudong_control[i].guflag[m]=2; }   //关
										             //记录该继电器通道有固定任务
										             break;                               }
										       else          {
													         shoudong_control[i].guflag[m]=0;
													                 }
													 }
												 }
											 }                                             } 
								     
									         else {
													 shoudong_control[i].guflag[m]=0;
													      }
									               }                     
													temptime=temptime<<1; }
											                  }     
									    tempdata=tempdata<<1;
									 } 
				          break;
				    case 0x02:       //无门限
				           for(m=0;m<16;m++){      
						           tempdata =relay_gate_data[i].relay_gateTask[k].relay_data;     //有固定任务的通道
									if((tempdata&0x8000)==0x8000){
												 temptime=relay_gate_data[i].relay_gateTask[k].set_time;
												for(j=0;j<4;j++){
													if((temptime&0x80)==0x80)  {
		                        if(((Time_GetData[4]*60+Time_GetData[5])>
									 (relay_gate_data[i].relay_gateTask[k].time_datah1[j]*60+relay_gate_data[i].relay_gateTask[k].time_datal1[j]))
								  &&((Time_GetData[4]*60+Time_GetData[5])<
									 (relay_gate_data[i].relay_gateTask[k].time_datah1[j]*60+relay_gate_data[i].relay_gateTask[k].time_datal1[j])))
								  {		
                     if	(!shoudong_control[i].shouflag[m]){									
		                      if(relay_gate_data[i].relay_gateTask[k].set_data[j]){
					               shoudong_control[i].guflag[m]=1;                     }  //开
												 else{
					               shoudong_control[i].guflag[m]=2; }   //关
										     //记录该继电器通道有固定任务
										       break;                         }
										 else {
													 shoudong_control[i].guflag[m]=0;
													}
					         } 
									         else {
													 shoudong_control[i].guflag[m]=0;
													      }
									                                     }
													temptime=temptime<<1;
											                  }     } 
									    tempdata=tempdata<<1;
									                  }                             
				          break; 
				    default: 
				          break;      
			                                                                 }
							                                                  }
							        temptask=temptask<<1;
						                          }									 
}         				
}
					}
void relayzhitask(void)          //智能控制处理函数
{	u8 i,j,m,n=0;
	u16 temptask,tempdata,tempEn;
	u8  tempgradient;
	u8 relayid,relayway=0;
		for(i=0;i<second_device.trans_data;i++) {
			  if(transID[i].state!=0xee){
					  for(m=0;m<16;m++){                   //智能控制探头 并判断是否使能
							    temptask=control_info[i].set_data; 
							    tempEn=Intelligent_value[i].flag_data;
					       if((tempEn&temptask&0x8000)==0x8000){
									 tempgradient=control_info[i].Transdata_control[m].gradient;
									 relayid=control_info[i].Transdata_control[m].relay_ID;   //继电器id
									 tempdata =control_info[i].Transdata_control[m].relay_data;//继电器通道
									 for(j=0;j<4;j++){
									   if((tempgradient&0x80)==0x80){
											  switch(control_info[i].Transdata_control[m].contype[j]){  //某梯度的控制类型
		                    case 0x01:     //小于下限执行
										        if(trans_query_data[i].integer[m]<control_info[i].Transdata_control[m].gate_data1[j]){
														  for(n=0;n<16;n++){            //继电器通道
								                 if((tempdata&0x8000)==0x8000){  //该通道无手动或是定时控制，则执行智能控制
																	 if(!(shoudong_control[relayid].shouflag[n]|shoudong_control[relayid].guflag[n])){
										               relayway++;
																	 if(relayway>control_info[i].Transdata_control[m].relays[j]){
																		 relayway=0;
																	 break;
																	 }
																	shoudong_control[relayid].zhiflag[n]=1;  //打开
									                             }              }
									                 tempdata=tempdata<<1;
																	 tempEn=tempEn<<1;
									             }	
														}
														break;
												 case 0x02:     //大于上限执行
										        if(trans_query_data[i].integer[m]>control_info[i].Transdata_control[m].gate_data2[j]){
														  for(n=0;n<16;n++){            //继电器通道
								                 if((tempdata&0x8000)==0x8000){//该通道无手动或是定时控制，则执行智能控制
																	 if(!(shoudong_control[relayid].shouflag[n]|shoudong_control[relayid].guflag[n])){
										               relayway++;
																	 if(relayway>control_info[i].Transdata_control[m].relays[j]){
																		 relayway=0;
																	 break;
																	 }
																	shoudong_control[relayid].zhiflag[n]=1; //打开
									                             }}
									                 tempdata=tempdata<<1;
									             }	
														}
														break;
													default: 
				                    break; 
												                                                      }
																														}
																					}
																				             }
																	}
																      }
																		          }
			}
void find_max_min(u16 *buffer, u8 Len)
{
 u8 i,j;
 u8 temp;
		for(i=0;i<Len;i++)
 {
 for(j=0;j<Len-1-i;j++)
 {
 if(buffer[j]<buffer[j+1])
 {
 temp=buffer[j];
 buffer[j]=buffer[j+1];
 buffer[j+1]=temp;}}}
}
