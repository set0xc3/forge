#include "forge.h"

#if FR_PLATFORM_WINDOWS
#include <windows.h>
#include <gl/gl.h>
#include <glext.h>
#elif FR_PLATFORM_LINUX
#endif

#include "forge_renderer_opengl.h"
#define GL_FUNC(N, T, P) typedef T (__stdcall N##Function)P; N##Function *N = 0; 
#include "forge_renderer_opengl_funcs.h"

#if FR_PLATFORM_LINUX
#include "forge_renderer_opengl_linux.c"
#elif FR_PLATFORM_WINDOWS
#include "forge_renderer_opengl_win32.c"
#else
#error missing platform detection
#endif

global Shader shader = {0};
global u32 VBO, VAO, EBO = 0;

internal u32 
gl_shader_create(const char *vs_path, const char *fs_path)
{
    u32 result = 0;
	i32 success = 0;
    char buffer_log[512] = {0};
    
	const char *vs_string = (const char *)os_file_read(str8_lit(vs_path)).str;
    const char *fs_string = (const char *)os_file_read(str8_lit(fs_path)).str;
    
    // Create and compile vertex shader
    u32 vs_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs_id, 1, &vs_string, 0);
    glCompileShader(vs_id);
	glGetShaderiv(vs_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vs_id, 512, 0, buffer_log);
		LOG_ERROR("[GL] Shader vertex compilation failed: %s", buffer_log);
    }
	
    // Create and compile fragment shader
    u32 fs_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs_id, 1, &fs_string, 0);
    glCompileShader(fs_id);
	glGetShaderiv(vs_id, GL_COMPILE_STATUS, &success);
	if (!success)
    {
        glGetShaderInfoLog(fs_id, 512, 0, buffer_log);
		LOG_ERROR("[GL] Shader fragment compilation failed: %s", buffer_log);
    }
    
    // Create program, attach shaders to it, and link it
    result = glCreateProgram();
    glAttachShader(result, vs_id);
    glAttachShader(result, fs_id);
    glLinkProgram(result);
    
    // Clear
    glDeleteShader(vs_id);
    glDeleteShader(fs_id);
	
	return result;
}

FR_API void
init(void *window_handle)
{
	gl_os_init(window_handle);
	
	gl_os_window_select(window_handle);
	
	shader.id = gl_shader_create("assets/shaders/shader.vert", "assets/shaders/shader.frag");
	
	Vertex2d vertex[] = 
	{
		-0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f,
		0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f,
		0.5f, 0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f,
		-0.5f, 0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	}; 
	u32 indices[] = 
	{
        0, 1, 3,
        1, 2, 3
    };
	
    glGenVertexArrays(1, &VAO);
	
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
	glBindVertexArray(VAO);
	
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
	
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	// position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(f32), (void*)0);
    glEnableVertexAttribArray(0);
	
	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(f32), (void*)(2 * sizeof(f32)));
    glEnableVertexAttribArray(1);
	
	// uv
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(f32), (void*)(5 * sizeof(f32)));
    glEnableVertexAttribArray(2);
	
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
	
	glDeleteBuffers(1, &VBO);
}

FR_API void 
begin(void *window_handle)
{
	gl_os_window_select(window_handle);
	
	glViewport(0, 0, 800, 600); 
	
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

FR_API void 
submit(void *window_handle)
{
	gl_os_window_select(window_handle);
	
	glUseProgram(shader.id);
	glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

FR_API void 
end(void *window_handle)
{
	gl_os_finish(window_handle);
}