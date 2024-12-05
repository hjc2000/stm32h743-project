#ifndef _MYIIC_H
#define _MYIIC_H

#include <stdint.h>

// IIC所有操作函数
void IIC_Init(void);                      // 初始化IIC的IO口
void IIC_Start(void);                     // 发送IIC开始信号
void IIC_Stop(void);                      // 发送IIC停止信号
void IIC_Send_Byte(uint8_t txd);          // IIC发送一个字节
uint8_t IIC_Read_Byte(unsigned char ack); // IIC读取一个字节

void IIC_Write_One_Byte(uint8_t daddr, uint8_t addr, uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr, uint8_t addr);
#endif
