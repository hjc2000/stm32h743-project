#include "myiic.h"
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/gpio.h>
#include <bsp-interface/serial/GpioSoftwareIicHost.h>
#include <hal.h>

bsp::GpioSoftwareIicHost _iic_host{"iic", "PH4", "PH5"};

// IIC初始化
void IIC_Init(void)
{
    _iic_host.Open();
}

// 产生IIC起始信号
void IIC_Start(void)
{
    _iic_host.SendStartingSignal();
}

// 产生IIC停止信号
void IIC_Stop(void)
{
    _iic_host.SendStoppingSignal();
}

// IIC发送一个字节
// 返回从机有无应答
// 1，有应答
// 0，无应答
void IIC_Send_Byte(uint8_t txd)
{
    _iic_host.SendByte(txd);
}

// 读1个字节，ack=1时，发送ACK，ack=0，发送nACK
uint8_t IIC_Read_Byte(unsigned char ack)
{
    return _iic_host.ReceiveByte(!ack);
}
