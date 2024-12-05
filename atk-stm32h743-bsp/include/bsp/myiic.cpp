#include "myiic.h"
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/gpio.h>
#include <bsp-interface/serial/GpioSoftwareIicHost.h>
#include <hal.h>

bsp::GpioSoftwareIicHost _iic_host{"iic", "PH4", "PH5"};

// IO操作
#define IIC_SCL(n) (n ? HAL_GPIO_WritePin(GPIOH, GPIO_PIN_4, GPIO_PIN_SET) : HAL_GPIO_WritePin(GPIOH, GPIO_PIN_4, GPIO_PIN_RESET)) // SCL
#define IIC_SDA(n) (n ? HAL_GPIO_WritePin(GPIOH, GPIO_PIN_5, GPIO_PIN_SET) : HAL_GPIO_WritePin(GPIOH, GPIO_PIN_5, GPIO_PIN_RESET)) // SDA
#define READ_SDA HAL_GPIO_ReadPin(GPIOH, GPIO_PIN_5)                                                                               // 输入SDA

// IO方向设置
#define SDA_IN()                         \
    {                                    \
        GPIOH->MODER &= ~(3 << (5 * 2)); \
        GPIOH->MODER |= 0 << 5 * 2;      \
    } // PH5输入模式
#define SDA_OUT()                        \
    {                                    \
        GPIOH->MODER &= ~(3 << (5 * 2)); \
        GPIOH->MODER |= 1 << 5 * 2;      \
    } // PH5输出模式

// IIC初始化
void IIC_Init(void)
{
    _iic_host.Open();
}

// 产生IIC起始信号
void IIC_Start(void)
{
    SDA_OUT(); // sda线输出
    IIC_SDA(1);
    IIC_SCL(1);
    DI_Delayer().Delay(std::chrono::microseconds{4});
    IIC_SDA(0); // START:when CLK is high,DATA change form high to low
    DI_Delayer().Delay(std::chrono::microseconds{4});
    IIC_SCL(0); // 钳住I2C总线，准备发送或接收数据
}

// 产生IIC停止信号
void IIC_Stop(void)
{
    SDA_OUT(); // sda线输出
    IIC_SCL(0);
    IIC_SDA(0); // STOP:when CLK is high DATA change form low to high
    DI_Delayer().Delay(std::chrono::microseconds{4});
    IIC_SCL(1);
    IIC_SDA(1); // 发送I2C总线结束信号
    DI_Delayer().Delay(std::chrono::microseconds{4});
}

// 等待应答信号到来
// 返回值：1，接收应答失败
//         0，接收应答成功
uint8_t IIC_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    SDA_IN(); // SDA设置为输入
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

    IIC_SCL(0); // 时钟输出0
    return 0;
}

// 产生ACK应答
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

// 不产生ACK应答
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

// IIC发送一个字节
// 返回从机有无应答
// 1，有应答
// 0，无应答
void IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    SDA_OUT();
    IIC_SCL(0); // 拉低时钟开始数据传输
    for (t = 0; t < 8; t++)
    {
        IIC_SDA((txd & 0x80) >> 7);
        txd <<= 1;

        // 对TEA5767这三个延时都是必须的
        DI_Delayer().Delay(std::chrono::microseconds{2});
        IIC_SCL(1);
        DI_Delayer().Delay(std::chrono::microseconds{2});
        IIC_SCL(0);
        DI_Delayer().Delay(std::chrono::microseconds{2});
    }
}

// 读1个字节，ack=1时，发送ACK，ack=0，发送nACK
uint8_t IIC_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    SDA_IN(); // SDA设置为输入
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
        IIC_NAck(); // 发送nACK
    }
    else
    {
        IIC_Ack(); // 发送ACK
    }

    return receive;
}
