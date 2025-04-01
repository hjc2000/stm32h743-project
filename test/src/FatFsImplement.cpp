#include <algorithm>
#include <base/string/ToHexString.h>
#include <chrono>
#include <cstdint>
#include <diskio.h>
#include <ff.h>
#include <iostream>

namespace
{
	int const _buffer_size = 1024 * 1024;

	/// @brief 每个扇区的大小（单位：字节）
	int const _sector_size = 512;

	/// @brief 扇区数量。
	int const _sector_count = _buffer_size / _sector_size;

	/// @brief 定义一个静态数组作为磁盘存储空间
	uint8_t *_buffer = reinterpret_cast<uint8_t *>(0xC0000000);

} // namespace

extern "C"
{
	/// @brief 初始化磁盘驱动器
	/// @param pdrv
	/// @return
	DSTATUS disk_initialize(BYTE pdrv)
	{
		// 这里假设初始化总是成功的
		// 返回0表示成功
		return 0;
	}

	/// @brief 获取磁盘的状态
	/// @param pdrv
	/// @return
	DSTATUS disk_status(BYTE pdrv)
	{
		// 假设磁盘总是处于良好状态
		// 返回0表示正常
		return 0;
	}

	/// @brief 从磁盘读取指定数量的扇区到缓冲区
	/// @param pdrv
	/// @param buff
	/// @param sector
	/// @param count
	/// @return
	DRESULT disk_read(BYTE pdrv, BYTE *buff, LBA_t sector, UINT count)
	{
		std::copy(_buffer + sector * _sector_size,
				  _buffer + (sector + count) * _sector_size,
				  buff);

		return RES_OK;
	}

	DRESULT disk_write(BYTE pdrv, const BYTE *buff, LBA_t sector, UINT count)
	{
		std::copy(buff,
				  buff + count * _sector_size,
				  _buffer + sector * _sector_size);

		return RES_OK;
	}

	/// @brief 控制磁盘设备
	/// @param pdrv
	/// @param cmd
	/// @param buff
	/// @return
	DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff)
	{
		switch (cmd)
		{
		case CTRL_SYNC:
			{
				// 刷新缓存到磁盘
				// 对于模拟磁盘，这里不需要做任何事情
				break;
			}
		case GET_SECTOR_COUNT:
			{
				// 获取磁盘上的总扇区数
				*reinterpret_cast<DWORD *>(buff) = _sector_count;
				break;
			}
		case GET_SECTOR_SIZE:
			{
				// 获取每个扇区的大小
				*reinterpret_cast<WORD *>(buff) = _sector_size;
				break;
			}
		case GET_BLOCK_SIZE:
			{
				// 获取擦除块的大小（以扇区为单位）
				// 对于模拟磁盘，我们假设擦除块大小等于一个扇区
				*reinterpret_cast<DWORD *>(buff) = 1;
				break;
			}
		case CTRL_TRIM:
			{
				break;
			}
		default:
			{
				// 参数错误
				return RES_PARERR;
			}
		}

		return RES_OK;
	}

	/// @brief 使用 std::chrono 获取当前时间并转换为 FatFs 时间戳
	/// @param
	/// @return
	DWORD get_fattime(void)
	{
		// auto now = std::chrono::system_clock::now();
		// std::time_t now_c = std::chrono::system_clock::to_time_t(now);
		// struct tm *now_tm = std::localtime(&now_c);

		// // FatFs 时间戳格式：YYYYMMDD HHMMSS
		// DWORD fat_time = ((now_tm->tm_year + 1900 - 1980) << 25) |
		//                  ((now_tm->tm_mon + 1) << 21) |
		//                  (now_tm->tm_mday << 16) |
		//                  (now_tm->tm_hour << 11) |
		//                  (now_tm->tm_min << 5) |
		//                  (now_tm->tm_sec >> 1);

		// return fat_time;
		return 0;
	}
}
