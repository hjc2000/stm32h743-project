#include<errno.h>
#include<unistd.h>

extern "C"
{
	/// <summary>
	///		stdio 底层会调用本函数。
	/// </summary>
	/// <param name="file">
	///		文件描述符
	///		* 对于标准输出，file = 1，对于错误输出，file = 2。
	/// </param>
	/// <param name="ptr"></param>
	/// <param name="len"></param>
	/// <returns></returns>
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
