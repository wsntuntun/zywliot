#ifndef  __dataprocess_H__
#define  __dataprocess_H__
#include "sys.h" 


void jiaozhun_send(u8 addr,u8 type);
void jiaozhun_Read_back(u8 prob1,u8 prob2,u8 type,u8 result);
void sence_send(u8 addr);
void sence_rec(void);

void find_max_min(u16 *buffer, u8 Len);

void relayzhitask(void);
void relayshoutask(void);
void relaygutask(void);
//void Ver_Read_back(u8 ver1,u8 ver2,u8 ver3,u8 ver4);
void Ver_Read(u8 addr1,u8 addr2);
void Ver_Read_back(u8 addr1,u8 addr2, u8 *verdata, u8 DataLen, u8 Datafirst);
void TransDset_Read_back(u8 result);
void RelayDset_Read_back(u8 result);

void pump_write(u8 addr1,u8 config1,u8 config2,u8 set1,u8 set2);
void pump_Read_back(void);

void yandu_write(u8 addr1,u8 set1,u8 set2);
void yandu_Read_back(u8 data1,u8 data2);
void yandu_read(u8 addr1);
void yandu_Write_back(void);

void TransDset_Write_back(u8 trnasID);
void relay_send(u8 addr); 
void trans_send(u8 addr,u8 probset1,u8 probset2);
void pump(void);
void relay(void);

#endif

