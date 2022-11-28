#include "forge_base_types.h"
#include "forge_array.h"
#include "forge_string.h"
#include "forge_logger.h"

void start(void)
{
	LOG_DEBUG("-------- Start --------");
	
	//u64 array_capasity = f_array_capacity(array_ptr);
	//for(u64 i = 0; i < array_capasity; i += 1)
	{
		//Event *event = os_memory_reserve(sizeof(Event));
		//f_array_push(array_ptr, event, sizeof(Event));
	}
}

void update(f32 delta_time)
{
	local b8 is_first_frame = true;
	if (is_first_frame)
	{
		is_first_frame = false;
		LOG_DEBUG("-------- Update --------");
	}
}

void stop()
{
	LOG_DEBUG("-------- Stop --------");
}