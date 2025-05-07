#include "base/embedded/key/key_handle.h"
#include "base/string/define.h"
#include "Key0.h"
#include "Key1.h"
#include "Key2.h"
#include "key_handle.h" // IWYU pragma: keep
#include <stdexcept>

base::key::sp_key_handle base::key::open(int key_id)
{
	switch (key_id)
	{
	case 0:
		{
			return base::key::sp_key_handle{new bsp::Key0{}};
		}
	case 1:
		{
			return base::key::sp_key_handle{new bsp::Key1{}};
		}
	case 2:
		{
			return base::key::sp_key_handle{new bsp::Key2{}};
		}
	default:
		{
			throw std::invalid_argument{CODE_POS_STR + "非法按键 id."};
		}
	}
}

bool base::key::key_is_down(base::key::key_handle &h)
{
	return h.KeyIsDown();
}
