#include <errno.h>
#include <unistd.h>

extern "C"
{
	/// @brief stdio 底层会调用本函数。
	///
	/// @param file 文件描述符
	/// @note 对于标准输出，file = 1，对于错误输出，file = 2。
	///
	/// @param ptr
	/// @param len
	/// @return
	int _write(int file, char *ptr, int len)
	{
		try
		{
			return len;
		}
		catch (...)
		{
			return EIO;
		}
	}
}
