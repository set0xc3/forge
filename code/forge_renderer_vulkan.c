#include "forge_base_types.h"
#include "forge_string.h"
#include "forge_logger.h"

#include "forge_os.h"

internal void 
vk_create_pipeline(String8 vs_path, String8 fs_path)
{
	FileInfo vs_dump = os_file_read(vs_path);
	FileInfo fs_dump = os_file_read(fs_path);
	
	LOG_DEBUG("vs_dump.size: %i", vs_dump.size);
	LOG_DEBUG("fs_dump.size: %i", fs_dump.size);
	
	MEMORY_FREE(vs_dump.data);
	MEMORY_FREE(fs_dump.data);
}

FR_API void
init(void *window_handle)
{
	vk_create_pipeline(str8_lit("D:\\dev\\forge\\assets\\shaders\\shader.vert.spv"),
					   str8_lit("D:\\dev\\forge\\assets\\shaders\\shader.frag.spv"));
}

FR_API void 
begin(void *window_handle)
{
}

FR_API void 
submit(void *window_handle)
{
}

FR_API void 
end(void *window_handle)
{
}