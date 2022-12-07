#include <forge_types.h>
#include <forge_logger.h>

#include "game.h"

FR_API void 
start(void)
{
	LOG_INFO("[GAME] start");
}

FR_API void 
update(f32 delta_time)
{
	local b8 is_one_frame = true;
	if (is_one_frame)
	{
		is_one_frame = false;
		LOG_INFO("[GAME] update");
	}
	
	//LOG_INFO("[GAME] delta_time: %f", delta_time);
}

FR_API void 
stop(void)
{
	LOG_INFO("[GAME] stop");
}