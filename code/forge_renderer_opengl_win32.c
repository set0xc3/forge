global HDC       gl_win32_pixel_format = 0;
global HINSTANCE gl_win32_hinstance = 0;
global HMODULE   gl_win32_hmodule = 0;
global HGLRC     gl_win32_ctx = 0;

internal void 
gl_os_init(void *window_handle)
{
	ASSERT(window_handle == 0);
	
	HWND hwnd = window_handle;
	
	gl_win32_hinstance = GetModuleHandleA(0);
	gl_win32_hmodule = GetModuleHandleA("opengl32.dll");
	
	HDC dummy_hdc = GetDC(hwnd);
	
	PIXELFORMATDESCRIPTOR format = {0};
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
	
	gl_win32_ctx = wglCreateContext(dummy_hdc);
	if (gl_win32_ctx == 0)
		ASSERT(true);
	
	if (!wglMakeCurrent(dummy_hdc, gl_win32_ctx))
		ASSERT(true);
	
	wglMakeCurrent(0, 0);
	ReleaseDC(gl_win32_ctx, dummy_hdc);
}

internal void 
gl_os_select_window(void *window_handle)
{
	ASSERT(window_handle == 0);
	
	HWND hwnd = window_handle;
    HDC hdc = GetDC(hwnd);
	wglMakeCurrent(hdc, gl_win32_ctx);
	ReleaseDC(hwnd, hdc);
}

internal void 
gl_os_finish(void *window_handle)
{
	ASSERT(window_handle == 0);
	
	HDC hdc = GetDC(window_handle);
	SwapBuffers(hdc);
	ReleaseDC(window_handle, hdc);
}