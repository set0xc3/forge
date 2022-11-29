global HDC       gl_win32_pixel_format = 0;
global HINSTANCE gl_win32_hinstance = 0;
global HMODULE   gl_win32_hmodule = 0;
global HGLRC     gl_win32_ctx = 0;

internal void 
gl_os_init(void)
{
	gl_win32_hinstance = GetModuleHandleA(0);
	gl_win32_hmodule = GetModuleHandleA("opengl32.dll");
	
	// Register the window class.
	WNDCLASSA wnd = {0};
	wnd.lpfnWndProc = DefWindowProcA;
	wnd.hInstance = gl_win32_hinstance;
	wnd.lpszClassName = (LPCSTR)"DUMMYCLASS";
	if (!RegisterClassA(&wnd))
	{
		ASSERT(true);
	}
	
	// Create the window.
	HWND hwnd = 0;
	{
		hwnd = CreateWindowA(wnd.lpszClassName,
							 "",
							 WS_OVERLAPPEDWINDOW,
							 0, 0, 0, 0, 0, 0,
							 gl_win32_hinstance,
							 0);
		ASSERT(hwnd == 0);
	}
	
	HDC dummy_hdc = GetDC(hwnd);
	
	PIXELFORMATDESCRIPTOR format = {0};
	{
		format.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
		format.nVersion   = 1;
		format.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
		format.iPixelType = PFD_TYPE_RGBA;
		format.cColorBits = 32;
		format.cAlphaBits = 8;
		format.cDepthBits = 24;
		format.iLayerType = PFD_MAIN_PLANE;
		
		i32 pixel_format = ChoosePixelFormat(dummy_hdc, &format);
		if (pixel_format == 0) 
		{
			ASSERT(true);
		}
		if (!SetPixelFormat(dummy_hdc,
							pixel_format,
							&format))
		{
			ASSERT(true);
		}
	}
	
	gl_win32_ctx = wglCreateContext(dummy_hdc);
	if (gl_win32_ctx == 0)
	{
		ASSERT(true);
	}
	
	if (!wglMakeCurrent(dummy_hdc, gl_win32_ctx))
	{
		ASSERT(true);
	}
	
	//wglMakeCurrent(0, 0);
	//wglDeleteContext(temp_gl_context);
	
	ReleaseDC(gl_win32_ctx, dummy_hdc);
	DestroyWindow(hwnd);
}

internal void 
gl_os_select_window(void *window_handle)
{
	HWND hwnd = window_handle;
    HDC hdc = GetDC(hwnd);
    BOOL make_current_good = wglMakeCurrent(hdc, gl_win32_ctx);
    ReleaseDC(hwnd, hdc);
}

internal void 
gl_os_finish(void *window_handle)
{
	HDC hdc = GetDC(window_handle);
	SwapBuffers(hdc);
	ReleaseDC(window_handle, hdc);
}