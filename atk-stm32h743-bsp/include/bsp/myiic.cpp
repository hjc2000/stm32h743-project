#include "myiic.h"
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/gpio.h>
#include <bsp-interface/serial/GpioSoftwareIicHost.h>
#include <hal.h>

bsp::GpioSoftwareIicHost _iic_host{"iic", "PH4", "PH5"};

// IO����
#define IIC_SCL(n) (n ? HAL_GPIO_WritePin(GPIOH, GPIO_PIN_4, GPIO_PIN_SET) : HAL_GPIO_WritePin(GPIOH, GPIO_PIN_4, GPIO_PIN_RESET)) // SCL
#define IIC_SDA(n) (n ? HAL_GPIO_WritePin(GPIOH, GPIO_PIN_5, GPIO_PIN_SET) : HAL_GPIO_WritePin(GPIOH, GPIO_PIN_5, GPIO_PIN_RESET)) // SDA
#define READ_SDA HAL_GPIO_ReadPin(GPIOH, GPIO_PIN_5)                                                                               // ����SDA

// IO��������
#define SDA_IN()                         \
    {                                    \
        GPIOH->MODER &= ~(3 << (5 * 2)); \
        GPIOH->MODER |= 0 << 5 * 2;      \
    } // PH5����ģʽ
#define SDA_OUT()                        \
    {                                    \
        GPIOH->MODER &= ~(3 << (5 * 2)); \
        GPIOH->MODER |= 1 << 5 * 2;      \
    } // PH5���ģʽ

// IIC��ʼ��
void IIC_Init(void)
{
    _iic_host.Open();
}

// ����IIC��ʼ�ź�
void IIC_Start(void)
{
    SDA_OUT(); // sda�����
    IIC_SDA(1);
    IIC_SCL(1);
    DI_Delayer().Delay(std::chrono::microseconds{4});
    IIC_SDA(0); // START:when CLK is high,DATA change form high to low
    DI_Delayer().Delay(std::chrono::microseconds{4});
    IIC_SCL(0); // ǯסI2C���ߣ�׼�����ͻ��������
}

// ����IICֹͣ�ź�
void IIC_Stop(void)
{
    SDA_OUT(); // sda�����
    IIC_SCL(0);
    IIC_SDA(0); // STOP:when CLK is high DATA change form low to high
    DI_Delayer().Delay(std::chrono::microseconds{4});
    IIC_SCL(1);
    IIC_SDA(1); // ����I2C���߽����ź�
    DI_Delayer().Delay(std::chrono::microseconds{4});
}

// �ȴ�Ӧ���źŵ���
// ����ֵ��1������Ӧ��ʧ��
//         0������Ӧ��ɹ�
uint8_t IIC_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    SDA_IN(); // SDA����Ϊ����
    IIC_SDA(1);
    DI_Delayer().Delay(std::chrono::microseconds{1});
    IIC_SCL(1);
    DI_Delayer().Delay(std::chrono::microseconds{1});
    while (READ_SDA)
    {
        ucErrTime++;
        if (ucErrTime > 250)
        {
            IIC_Stop();
            return 1;
        }
    }

    IIC_SCL(0); // ʱ�����0
    return 0;
}

// ����ACKӦ��
void IIC_Ack(void)
{
    IIC_SCL(0);
    SDA_OUT();
    IIC_SDA(0);
    DI_Delayer().Delay(std::chrono::microseconds{2});
    IIC_SCL(1);
    DI_Delayer().Delay(std::chrono::microseconds{2});
    IIC_SCL(0);
}

// ������ACKӦ��
void IIC_NAck(void)
{
    IIC_SCL(0);
    SDA_OUT();
    IIC_SDA(1);
    DI_Delayer().Delay(std::chrono::microseconds{2});
    IIC_SCL(1);
    DI_Delayer().Delay(std::chrono::microseconds{2});
    IIC_SCL(0);
}

// IIC����һ���ֽ�
// ���شӻ�����Ӧ��
// 1����Ӧ��
// 0����Ӧ��
void IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    SDA_OUT();
    IIC_SCL(0); // ����ʱ�ӿ�ʼ���ݴ���
    for (t = 0; t < 8; t++)
    {
        IIC_SDA((txd & 0x80) >> 7);
        txd <<= 1;

        // ��TEA5767��������ʱ���Ǳ����
        DI_Delayer().Delay(std::chrono::microseconds{2});
        IIC_SCL(1);
        DI_Delayer().Delay(std::chrono::microseconds{2});
        IIC_SCL(0);
        DI_Delayer().Delay(std::chrono::microseconds{2});
    }
}

// ��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
uint8_t IIC_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    SDA_IN(); // SDA����Ϊ����
    for (i = 0; i < 8; i++)
    {
        IIC_SCL(0);
        DI_Delayer().Delay(std::chrono::microseconds{2});
        IIC_SCL(1);
        receive <<= 1;
        if (READ_SDA)
        {
            receive++;
        }

        DI_Delayer().Delay(std::chrono::microseconds{1});
    }

    if (!ack)
    {
        IIC_NAck(); // ����nACK
    }
    else
    {
        IIC_Ack(); // ����ACK
    }

    return receive;
}
