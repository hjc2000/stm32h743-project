#include "24cxx.h"
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/gpio.h>
#include <bsp-interface/di/iic.h>
#include <bsp-interface/serial/GpioSoftwareIicHost.h>
#include <hal.h>

bsp::IIicHost *_iic_host = nullptr;

// ��ʼ��IIC�ӿ�
void AT24CXX_Init()
{
    _iic_host = DI_IicHostCollection().Get("eerom_iic_host");
    _iic_host->Open();
}

// ��AT24CXXָ����ַ����һ������
// ReadAddr:��ʼ�����ĵ�ַ
// ����ֵ  :����������
uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr)
{
    uint8_t temp = 0;
    _iic_host->SendStartingSignal();
    if (EE_TYPE > AT24C16)
    {
        _iic_host->SendByte(0XA0);          // ����д����
        _iic_host->SendByte(ReadAddr >> 8); // ���͸ߵ�ַ
    }
    else
    {
        _iic_host->SendByte(0XA0 + ((ReadAddr / 256) << 1)); // ����������ַ0XA0,д����
    }

    _iic_host->SendByte(ReadAddr % 256); // ���͵͵�ַ
    _iic_host->SendStartingSignal();
    _iic_host->SendByte(0XA1); // �������ģʽ
    temp = _iic_host->ReceiveByte(1);
    _iic_host->SendStoppingSignal(); // ����һ��ֹͣ����
    return temp;
}

// ��AT24CXXָ����ַд��һ������
// WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ
// DataToWrite:Ҫд�������
void AT24CXX_WriteOneByte(uint16_t WriteAddr, uint8_t DataToWrite)
{
    _iic_host->SendStartingSignal();
    if (EE_TYPE > AT24C16)
    {
        _iic_host->SendByte(0XA0);           // ����д����
        _iic_host->SendByte(WriteAddr >> 8); // ���͸ߵ�ַ
    }
    else
    {
        _iic_host->SendByte(0XA0 + ((WriteAddr / 256) << 1)); // ����������ַ0XA0,д����
    }

    _iic_host->SendByte(WriteAddr % 256); // ���͵͵�ַ
    _iic_host->SendByte(DataToWrite);     // �����ֽ�
    _iic_host->SendStoppingSignal();      // ����һ��ֹͣ����
    DI_Delayer().Delay(std::chrono::milliseconds{10});
}

// ��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
// �ú�������д��16bit����32bit������.
// WriteAddr  :��ʼд��ĵ�ַ
// DataToWrite:���������׵�ַ
// Len        :Ҫд�����ݵĳ���2,4
void AT24CXX_WriteLenByte(uint16_t WriteAddr, uint32_t DataToWrite, uint8_t Len)
{
    uint8_t t;
    for (t = 0; t < Len; t++)
    {
        AT24CXX_WriteOneByte(WriteAddr + t, (DataToWrite >> (8 * t)) & 0xff);
    }
}

// ��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
// �ú������ڶ���16bit����32bit������.
// ReadAddr   :��ʼ�����ĵ�ַ
// ����ֵ     :����
// Len        :Ҫ�������ݵĳ���2,4
uint32_t AT24CXX_ReadLenByte(uint16_t ReadAddr, uint8_t Len)
{
    uint8_t t;
    uint32_t temp = 0;
    for (t = 0; t < Len; t++)
    {
        temp <<= 8;
        temp += AT24CXX_ReadOneByte(ReadAddr + Len - t - 1);
    }

    return temp;
}

// ���AT24CXX�Ƿ�����
// ��������24XX�����һ����ַ(255)���洢��־��.
// ���������24Cϵ��,�����ַҪ�޸�
// ����1:���ʧ��
// ����0:���ɹ�
uint8_t AT24CXX_Check()
{
    uint8_t temp;
    temp = AT24CXX_ReadOneByte(255); // ����ÿ�ο�����дAT24CXX
    if (temp == 0X55)
    {
        return 0;
    }
    else // �ų���һ�γ�ʼ�������
    {
        AT24CXX_WriteOneByte(255, 0X55);
        temp = AT24CXX_ReadOneByte(255);
        if (temp == 0X55)
        {
            return 0;
        }
    }

    return 1;
}

// ��AT24CXX�����ָ����ַ��ʼ����ָ������������
// ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
// pBuffer  :���������׵�ַ
// NumToRead:Ҫ�������ݵĸ���
void AT24CXX_Read(uint16_t ReadAddr, uint8_t *pBuffer, uint16_t NumToRead)
{
    while (NumToRead)
    {
        *pBuffer++ = AT24CXX_ReadOneByte(ReadAddr++);
        NumToRead--;
    }
}

// ��AT24CXX�����ָ����ַ��ʼд��ָ������������
// WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
// pBuffer   :���������׵�ַ
// NumToWrite:Ҫд�����ݵĸ���
void AT24CXX_Write(uint16_t WriteAddr, uint8_t *pBuffer, uint16_t NumToWrite)
{
    while (NumToWrite--)
    {
        AT24CXX_WriteOneByte(WriteAddr, *pBuffer);
        WriteAddr++;
        pBuffer++;
    }
}
