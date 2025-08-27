// IWYU pragma: begin_keep

#include "base/Console.h"
#include "base/embedded/core.h"
#include "base/embedded/dma/MemoryDma.h"
#include "base/embedded/key/Key.h"
#include "base/embedded/key/KeyScanner.h"
#include "base/embedded/led/Led.h"
#include "base/embedded/led/LedBar.h"
#include "base/embedded/systick/systick.h"
#include "base/embedded/watch-dog/IndependentWatchDog.h"
#include "base/net/ethernet/EthernetFrameReader.h"
#include "base/net/profinet/dcp/DcpHelloRequestWriter.h"
#include "base/task/delay.h"
#include "base/task/task.h"
#include "base/test/TestBaseTimer.h"
#include "base/test/TestInputCaptureTimer.h"
#include "base/test/TestMemoryDma.h"
#include "base/test/TestPwmTimer.h"
#include "EthernetPort.h"
#include "ff.h"
#include "initialize.h"
#include "littlefs/LfsFlashPort.h"
#include "lwip-wrapper/NetifSlot.h"
#include "lwip-wrapper/NetifWrapper.h"
#include <chrono>
#include <memory>
#include <string>

// IWYU pragma: end_keep

/* #region 测试函数 */

void TestLittleFs()
{
	Lfs::LfsFlashPort port{DI_FlashCollection().Get("internal-flash")};

	// 初始化 LittleFS 文件系统
	lfs_t lfs;
	int res = lfs_format(&lfs, &port.Port()); // 格式化文件系统
	if (res)
	{
		base::console.WriteLine("format failed: " + std::to_string(res));
	}

	res = lfs_mount(&lfs, &port.Port()); // 挂载文件系统
	if (res)
	{
		base::console.WriteLine("mount failed: " + std::to_string(res));
	}

	// 创建文件并写入数据
	char const *filename = "example.txt";
	char const *str = "Hello, LittleFS!";
	lfs_file_t file;

	res = lfs_file_open(&lfs, &file, filename, LFS_O_WRONLY | LFS_O_CREAT | LFS_O_TRUNC);
	if (res)
	{
		base::console.WriteLine("open failed: " + std::to_string(res));
	}

	res = lfs_file_write(&lfs, &file, str, strlen(str));
	if (res < 0)
	{
		base::console.WriteLine("write failed: " + std::to_string(res));
	}

	lfs_file_close(&lfs, &file);

	// 读取文件内容
	res = lfs_file_open(&lfs, &file, filename, LFS_O_RDONLY);
	if (res)
	{
		base::console.WriteLine("open failed: " + std::to_string(res));
	}

	char buffer[128] = {0};
	res = lfs_file_read(&lfs, &file, buffer, sizeof(buffer) - 1);
	if (res < 0)
	{
		base::console.WriteLine("read failed: " + std::to_string(res));
	}

	base::console.WriteLine("have read: " + std::to_string(res));
	base::console.WriteLine(buffer);

	lfs_file_close(&lfs, &file);

	// 卸载文件系统
	lfs_unmount(&lfs);
}

inline void TestFatFs()
{
	FATFS fatfs{};
	BYTE work[FF_MAX_SS]; // 工作缓冲区
	FRESULT res{};

	// 初始化FatFs模块
	res = f_mount(&fatfs, "", 0); // 卸载任何已挂载的卷
	if (res != FR_OK)
	{
		base::console.WriteLine("f_mount error: " + std::to_string(res));
	}

	// 创建格式化参数结构体
	MKFS_PARM mkfs_parm{};
	mkfs_parm.fmt = FM_FAT; // 设置文件系统类型为FAT，并创建一个简单的FAT12/FAT16分区
	mkfs_parm.n_fat = 1;    // 设置FAT表的数量为1
	mkfs_parm.align = 1;    // 数据区域对齐为1扇区
	mkfs_parm.n_root = 0;   // 根目录条目数为0（自动选择）
	mkfs_parm.au_size = 0;  // 分配单元大小为自动选择

	// 格式化磁盘
	res = f_mkfs("", &mkfs_parm, work, sizeof(work));
	if (res != FR_OK)
	{
		base::console.WriteLine("f_mkfs error: " + std::to_string(res));
	}

	// 重新挂载文件系统
	res = f_mount(&fatfs, "", 1); // 挂载文件系统
	if (res != FR_OK)
	{
		base::console.WriteLine("f_mount error: " + std::to_string(res));
	}

	FIL file{};
	char const *filename = "example.txt";
	char const *str = "Hello, FATFS!";

	// 打开或创建文件
	res = f_open(&file, filename, FA_CREATE_ALWAYS | FA_WRITE);
	if (res == FR_OK)
	{
		// 写入字符串到文件
		UINT bytesWritten;
		res = f_write(&file, str, strlen(str), &bytesWritten);
		if (res != FR_OK || bytesWritten != strlen(str))
		{
			base::console.WriteLine("write failed: " + std::to_string(res));
		}
		else
		{
			base::console.WriteLine("write " +
									std::to_string(bytesWritten) +
									" bytes to the file successfully");
		}

		// 关闭文件
		f_close(&file);
	}
	else
	{
		base::console.WriteLine("open file failed: " + std::to_string(res));
	}

	// 重新打开文件以读取
	res = f_open(&file, filename, FA_READ);
	if (res == FR_OK)
	{
		// 假设文件内容不会超过127个字符
		char buffer[128]{};
		UINT bytesRead;

		// 从文件读取内容
		res = f_read(&file, buffer, sizeof(buffer) - 1, &bytesRead);
		if (res != FR_OK)
		{
			base::console.WriteLine("read file failed: " + std::to_string(res));
		}
		else
		{
			base::console.WriteLine("read file successfully, have read " +
									std::to_string(bytesRead) +
									" bytes");

			base::console.WriteLine(buffer);
		}

		// 关闭文件
		f_close(&file);
	}
	else
	{
		base::console.WriteLine("open file failed: " + std::to_string(res));
	}

	// 卸载文件系统
	f_mount(NULL, "", 0);
}

void TestDCP()
{
	std::shared_ptr<lwip::NetifWrapper> netif_wrapper{new lwip::NetifWrapper{"netif"}};
	lwip::NetifSlot::Instance().PlugIn(netif_wrapper);

	base::IPAddress ip_address{"192.168.1.30"};
	base::IPAddress netmask{"255.255.255.0"};
	base::IPAddress gateway{"192.168.1.1"};

	/// @brief 本网卡所使用的 MAC 地址。
	base::Mac mac{
		std::endian::big,
		base::Array<uint8_t, 6>{
			0xB8,
			0xAE,
			0x1D,
			0x00,
			0x04,
			0x00,
		},
	};

	bsp::EthernetPort port{};

	netif_wrapper->Open(&port,
						mac,
						ip_address,
						netmask,
						gateway,
						1500);

	port.ReceivingEhternetFrameEvent().Subscribe(
		[](base::ReadOnlySpan span)
		{
			std::shared_ptr<lwip::NetifWrapper> netif_wrapper = lwip::NetifSlot::Instance().Find("netif");
			base::ethernet::EthernetFrameReader frame{span};
			base::console.WriteLine("收到以太网帧：");
			base::console.WriteLine(frame);
			// EhternetInput(span);
		});

	base::console.WriteLine("MAC 地址：" + netif_wrapper->Mac().ToString());
	netif_wrapper->EnableDHCP();
	while (!netif_wrapper->HasGotAddressesByDHCP())
	{
		// break;
	}

	std::unique_ptr<uint8_t[]> buffer{new uint8_t[1500]{}};
	while (true)
	{
		base::Span buffer_span{buffer.get(), 1500};
		base::profinet::DcpHelloRequestWriter hello{buffer_span};
		hello.WriteSourceMac(mac);
		hello.WriteXid(1);
		hello.WriteNameOfStationBlock("rt-labs-dev");

		hello.WriteIPAddressInfomationBlock(false,
											netif_wrapper->IPAddress(),
											netif_wrapper->Gateway(),
											netif_wrapper->Netmask());

		hello.WriteIdBlock(0x0493, 0x0002);
		hello.WriteOemIdBlock(0xcafe, 0xee02);
		hello.WriteDeviceInitiativeBlock(true);

		port.Send(hello.SpanForSending());
		base::task::Delay(std::chrono::milliseconds{1000});
	}
}

void freertos_demo();
int p_net_sample_app_main();
void EhternetInput(base::ReadOnlySpan const &span);

/* #endregion */

///
/// @brief 起始任务。
///
///
void InitialTask()
{
	bsp::initialize_sdram_heap();
	bsp::initialize_iic_host();
	bsp::initialize_led();
	bsp::initialize_pcf8574();
	bsp::initialize_console();

	base::test::TestBaseTimer(6);
	base::test::TestPwmTimer(3, 4);
	base::test::TestInputCaptureTimer(5, 1);

	base::task::run("led",
					1,
					1024 * 10,
					[]()
					{
						base::independent_watch_dog::IndependentWatchDog watch_dog{1};
						watch_dog.Initialize(std::chrono::milliseconds{2000});

						base::led::led_bar[0].TurnOff();
						while (true)
						{
							watch_dog.Feed();
							base::led::led_bar[0].Toggle();
							base::console.WriteLine(base::systick::frequency());
							base::console.WriteLine(std::to_string(static_cast<std::chrono::nanoseconds>(base::systick::system_time_stamp()).count()) + "ns");
							base::console.WriteLine(std::to_string(static_cast<std::chrono::microseconds>(base::systick::system_time_stamp()).count()) + "us");
							base::console.WriteLine(std::to_string(static_cast<std::chrono::milliseconds>(base::systick::system_time_stamp()).count()) + "ms");
							base::console.WriteLine(std::to_string(static_cast<std::chrono::seconds>(base::systick::system_time_stamp()).count()) + "s");
							base::task::Delay(std::chrono::milliseconds{1000});
						}
					});

	base::task::run("test",
					1,
					1024 * 10,
					[]()
					{
						// TestFatFs();
						// freertos_demo();
						// p_net_sample_app_main();
						// TestLittleFs();
						// TestDCP();
						// TestUniversalTimer1();
						// bsp::TestFlash();
						// TestExtiKey();
						// bsp::TestSerial();
						// bsp::TestKeyScanner();
						// bsp::TestIndependentWatchDog();
						// base::test::TestMemoryDma1<4>(1);
					});

	base::task::run("key scanner",
					1,
					1024 * 10,
					[]()
					{
						base::key::KeyScanner scanner{
							base::key::Key{0},
							base::key::Key{1},
							base::key::Key{2},
						};

						while (true)
						{
							scanner.ScanKeys();
							if (scanner.HasKeyDownEvent(0))
							{
								base::led::led_bar[0].Toggle();
							}

							if (scanner.HasKeyDownEvent(1))
							{
								base::led::led_bar[0].Toggle();
							}

							if (scanner.HasKeyDownEvent(2))
							{
								base::led::led_bar[0].Toggle();
							}
						}
					});
}

int main(void)
{
	base::core::initialize();
	bsp::initialize_clock();

	///
	/// 不要在 main 函数中定义局部变量，然后创建任务的时候让 lambda 表达式通过引用的方式捕获，试图在
	/// 任务函数中访问这些局部变量。根据网上的一个说法：freertos 启动调度后会覆盖掉 main 函数的栈，
	/// 所以 main 函数的参数、局部变量全部会被破坏。
	///
	/// @li https://www.freertos.org/FreeRTOS_Support_Forum_Archive/February_2005/freertos_Reusing_main_s_stack_an_ISR_only_stack_1229327.html
	/// @li https://stackoverflow.com/questions/60060627/object-in-stack-of-main-function-is-overwritten-when-first-task-runs-freertos
	///
	/// 据说 main 函数被认为是只执行一次后就应该被删除的初始任务，所以它的栈理应被挪作他用。
	///

	base::task::run("initial",
					1,
					1024 * 4,
					[]()
					{
						InitialTask();
					});

	// 启动调度后 freertos 会使能 systick 中断，配置 systick 的重装载值和分频系数。
	base::task::start_scheduler();
}
