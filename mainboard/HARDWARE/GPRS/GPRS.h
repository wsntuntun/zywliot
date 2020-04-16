#ifndef __GPRS_H
#define	__GPRS_H
#include "sys.h" 

void UART4_Rx_err(void);				//UART4����Modbus֡֡ͷ(��ַ��)����
void mem_copy_byte(u8 *source, u8 *direct, u8 length);	//���鿽������
void USART2_Command(void);			//��λ�����͵�֡���ݽ�������Ӧ����
u16 CRC16(u8 *buffer, u8 usLen);			//CRC16У�麯��
void Comeback_err(u8 *buffer, u8 Len,u8 comend_ack);	//֡���ݴ��󷵻غ���
void ID_Read_back(u8 *buffer, u8 Len);	//���豸ID���غ���
void Transset_Read_back(u8 *buffer, u8 Len);	//������汾���غ���
void Transcon_Read_back(u8 *buffer, u8 Len);	//���豸�����ʷ��غ���
void Relayopen_Read_back(u8 *buffer, u8 Len);		//���豸IP��ַ���غ���
void Alarm_Read_back(u8 *buffer, u8 Len);//������Ũ�Ⱦ���ֵ�򱨾�ֵ���غ���
void Transadjust_Read_back(u8 *buffer, u8 Len);//���������ڷ��غ���

void Systemset_Read_back(u8 *buffer, u8 Len);	//�������¶�ʪ�ȷ��غ���
void Transdata_Read_back(u8 *buffer, u8 Len);			//���豸��ַ�뷵�غ���
void Write_back(u8 *buffer, u8 Len);			//д������֡������ȷ���غ���
void ID_Write_back(u8 *buffer, u8 Len);		//д�豸ID���غ���
void Transset_Write_back(u8 *buffer, u8 Len);	//д�豸�����ʷ��غ���
void Transcon_Write_back(u8 *buffer, u8 Len);				//д�豸IP��ַ���غ���
void Alarm_Write_back(u8 *buffer, u8 Len);		//д����Ũ�Ⱦ���ֵ�򱨾�ֵ���غ���
void Relayopen_Write_back(u8 *buffer, u8 Len);		//���̵����̶�ʱ�俪��������
void Transadjust_Write_back(u8 *buffer, u8 Len);	//д����궨ֵ���غ���
void Systime_Write_back(u8 *buffer, u8 Len);		//����ϵͳʱ�䷵�غ���
void Systemset_Write_back(u8 *buffer, u8 Len);		//д�豸��ַ�뷵�غ���
void Transzhun_Write_back(u8 *buffer, u8 Len);	 //�����豸ͨ�ŷ�ʽ���غ���

void Relaydata_Read_back(u8 *buffer, u8 Len);				//��ʷ���ݲ�ѯ����
void Relaycon_Write_back(u8 *buffer, u8 Len);
void Handshake_Read_back(u8 *buffer, u8 Len);		//���豸�Ƿ�������������
void DeviceHardware_Read_back(u8 *buffer, u8 Len);		//���豸��Ӳ���汾
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

void Salt_Read_back(u8 *buffer, u8 Len);   //��ȡ�ζȲ���
void Salt_Write_back(u8 *buffer, u8 Len);   //дȡ�ζȲ���
void Relayopengate_Read_back(u8 *buffer, u8 Len);//���̵����̶�ʱ�俪��������
void Relayopengate_Write_back(u8 *buffer, u8 Len);//д�̵����̶�ʱ�俪��������
void DeviceCon_Write_back(u8 *buffer, u8 Len);   //�豸ʹ�ܿ���
void Intelligence_Write_back(u8 *buffer, u8 Len);  //���ܿ���
void SecondID_Write_back(u8 *buffer, u8 Len);// �����豸ID����
void SetmaxSecondID_Write_back(u8 *buffer, u8 Len);// ��������������
void Restoresetting_Write_back(u8 *buffer, u8 Len);//�ָ���������
void Relaystate_Read_back(u8 *buffer, u8 Len);	
void DeviceCon_Read_back(u8 *buffer, u8 Len);	//���豸ʹ�ܷ��غ���
void Intelligence_Read_back(u8 *buffer, u8 Len);	//�����ܿ���
void SetmaxSecondID_Read_back(u8 *buffer, u8 Len);	//��֧������
void Reset_Write_back(u8 *buffer, u8 Len);	//��������
void ExistSecondID_Read_back(u8 *buffer, u8 Len);		  //��ʵ������
void ExistSecondID_Write_back(u8 *buffer, u8 Len);		  //дʵ������
void Transpump_Read_back(u8 *buffer, u8 Len);
void DeviceHardware_Write_back(u8 *buffer, u8 Len);
void Transpump_Write_back(u8 *buffer, u8 Len);
void PathwayName_Write_back(u8 *buffer, u8 Len);
void DeviceName_Write_back(u8 *buffer, u8 Len);
void Relayset_Write_back(u8 *buffer, u8 Len);



void SoftReset(void) ;
void GetLockCode(void);
#endif
