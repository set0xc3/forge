#include "forge_base_types.h"
#include "forge_string.h"
#include "forge_logger.h"

#include "forge_renderer.h"

#include <Windows.h>
#include <GL/GL.h>

#include "forge_renderer_opengl.h"

#if FR_PLATFORM_LINUX
#include "forge_renderer_opengl_linux.c"
#elif FR_PLATFORM_WINDOWS
#include "forge_renderer_opengl_win32.c"
#else
#error missing platform detection
#endif

FR_API void
init(void *window_handle)
{
	gl_os_init(window_handle);
}

FR_API void 
begin(void *window_handle)
{
	gl_os_select_window(window_handle);
	
	glViewport(0, 0, 800, 600); 
	
	glClearColor(1.0f, 0.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

FR_API void 
submit(void *window_handle)
{
	gl_os_select_window(window_handle);
}

FR_API void 
end(void *window_handle)
{
	gl_os_finish(window_handle);
}