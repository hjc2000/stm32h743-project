#include "myiic.h"
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/gpio.h>
#include <bsp-interface/serial/GpioSoftwareIicHost.h>
#include <hal.h>

bsp::GpioSoftwareIicHost _iic_host{"iic", "PH4", "PH5"};

// IIC��ʼ��
void IIC_Init(void)
{
    _iic_host.Open();
}

// ����IIC��ʼ�ź�
void IIC_Start(void)
{
    _iic_host.SendStartingSignal();
}

// ����IICֹͣ�ź�
void IIC_Stop(void)
{
    _iic_host.SendStoppingSignal();
}

// IIC����һ���ֽ�
// ���شӻ�����Ӧ��
// 1����Ӧ��
// 0����Ӧ��
void IIC_Send_Byte(uint8_t txd)
{
    _iic_host.SendByte(txd);
}

// ��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
uint8_t IIC_Read_Byte(unsigned char ack)
{
    return _iic_host.ReceiveByte(!ack);
}
