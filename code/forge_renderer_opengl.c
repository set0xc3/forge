#include "forge_base_types.h"
#include "forge_string.h"
#include "forge_logger.h"
#include "forge_os.h"

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

internal u32 
gl_create_shader(const char *vs_path, const char *fs_path)
{
    u32 result = 0;
    
	FileInfo vs_string = os_file_read(str8_lit(vs_path));
    FileInfo fs_string = os_file_read(str8_lit(fs_path));
    
#if 0
    // Create and compile vertex shader
    u32 vs_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs_id, 1, &vs_id_source, 0);
    glCompileShader(vs_id);
    
    // Create and compile fragment shader
    u32 fs_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs_id, 1, &fs_id_source, 0);
    glCompileShader(fs_id);
    
    // Create program, attach shaders to it, and link it
    result = glCreateProgram();
    glAttachShader(result, vs_id);
    glAttachShader(result, fs_id);
    glLinkProgram(result);
    
    // Clear
    glDeleteShader(vs_id);
    glDeleteShader(fs_id);
#endif
	
	MEMORY_FREE(vs_string.data);
	MEMORY_FREE(fs_string.data);
	
	return result;
}

internal void 
gl_draw_rectangle(Vector2f min, Vector2f max, Vector4f color)
{
	glBegin(GL_TRIANGLES);
	
	glColor4f(color.x, color.y, color.z, color.w);
	
	// Lower triangle
	glVertex2f(min.x, min.y);
	glVertex2f(max.x, min.y);
	glVertex2f(max.x, max.y);
	
	// Upper triangle
	glVertex2f(min.x, min.y);
	glVertex2f(max.x, max.y);
	glVertex2f(min.x, max.y);
	
	glEnd();
}

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
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

FR_API void 
submit(void *window_handle)
{
	gl_os_select_window(window_handle);
	
	gl_draw_rectangle(v2f(0.0f, 0.0f), v2f(0.5f, 0.5f), v4f(1.0f, 0.0f, 1.0f, 1.0f));
}

FR_API void 
end(void *window_handle)
{
	gl_os_finish(window_handle);
}