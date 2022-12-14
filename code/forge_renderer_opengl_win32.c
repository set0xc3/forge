global HINSTANCE win32_hinstance = 0;
global HMODULE   win32_gl_hmodule = 0;
global HGLRC     win32_gl_ctx = 0;

typedef BOOL __stdcall wglSwapIntervalEXTFunction(int interval);
global wglSwapIntervalEXTFunction *wglSwapIntervalEXT = 0;

internal void 
*win32_get_gl_func_address(HMODULE module, const char *name)
{
	void *p = (void *)wglGetProcAddress(name);
	if(p == 0 ||
	   p == (void*)0x1 || 
	   p == (void*)0x2 || 
	   p == (void*)0x3 ||
	   p == (void*)-1)
	{
		p = (void *)GetProcAddress(module, name);
	}
	
	return p;
}

internal void 
gl_os_init(void *window_handle)
{
	ASSERT(window_handle == 0);
	
	HWND hwnd = (HWND)window_handle;
	
	win32_hinstance = GetModuleHandleA(0);
	win32_gl_hmodule = GetModuleHandleA("opengl32.dll");
	
	HDC dummy_hdc = GetDC(hwnd);
	
	PIXELFORMATDESCRIPTOR format;
	MEMORY_ZERO_STRUCT(&format, sizeof(PIXELFORMATDESCRIPTOR));
	{
		format.nSize      = sizeof(format);
		format.nVersion   = 1;
		format.dwFlags    = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
		format.iPixelType = PFD_TYPE_RGBA;
		format.cColorBits = 32;
		format.cAlphaBits = 8;
		format.cDepthBits = 24;
		format.iLayerType = PFD_MAIN_PLANE;
		
		i32 pixel_format_index = ChoosePixelFormat(dummy_hdc, &format);
		if (pixel_format_index == 0) 
			ASSERT(true);
		if (!SetPixelFormat(dummy_hdc, pixel_format_index, &format))
			ASSERT(true);
	}
	
	win32_gl_ctx = wglCreateContext(dummy_hdc);
	if (win32_gl_ctx == 0)
		ASSERT(true);
	
	if (!wglMakeCurrent(dummy_hdc, win32_gl_ctx))
		ASSERT(true);
	
#define LOAD_WGL_FUNC(N) N = (N##Function *)win32_get_gl_func_address(win32_gl_hmodule, #N); ASSERT(N == 0);
#define GL_FUNC(N, T, P) LOAD_WGL_FUNC(N)
#include "forge_renderer_opengl_funcs.h"
	
	LOAD_WGL_FUNC(wglSwapIntervalEXT);
	if (wglSwapIntervalEXT)
	{
		wglSwapIntervalEXT(1);
	}
	else
	{
		LOG_WARNING("Get func address failed: wglSwapIntervalEXT");
	}
	
	wglMakeCurrent(0, 0);
	ReleaseDC(hwnd, dummy_hdc);
}

internal void 
gl_os_window_select(void *window_handle)
{
	ASSERT(window_handle == 0);
	
	HWND hwnd = (HWND)window_handle;
    HDC hdc = GetDC(hwnd);
	wglMakeCurrent(hdc, win32_gl_ctx);
	ReleaseDC(hwnd, hdc);
}

internal void 
gl_os_finish(void *window_handle)
{
	ASSERT(window_handle == 0);
	
	HWND hwnd = (HWND)window_handle;
	HDC hdc = GetDC(hwnd);
	SwapBuffers(hdc);
	ReleaseDC(hwnd, hdc);
}