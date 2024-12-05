#ifndef _MYIIC_H
#define _MYIIC_H

#include <stdint.h>

// IIC���в�������
void IIC_Init(void);                      // ��ʼ��IIC��IO��
void IIC_Start(void);                     // ����IIC��ʼ�ź�
void IIC_Stop(void);                      // ����IICֹͣ�ź�
void IIC_Send_Byte(uint8_t txd);          // IIC����һ���ֽ�
uint8_t IIC_Read_Byte(unsigned char ack); // IIC��ȡһ���ֽ�

void IIC_Write_One_Byte(uint8_t daddr, uint8_t addr, uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr, uint8_t addr);
#endif
