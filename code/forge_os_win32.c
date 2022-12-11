#include <windows.h>
#include <shellapi.h>

void WINAPI
win32_file_io_completion_routine(DWORD error_code,
								 DWORD number_of_bytes_transfered,
								 LPOVERLAPPED overlapped)
{
	LOG_ERROR("Error code:\t%x\n", error_code);
	LOG_INFO("Number of bytes:\t%x\n", number_of_bytes_transfered);
}

FR_API void 
os_setup(void)
{
}

FR_API void 
os_shutdown(void)
{
	
}

FR_API void
os_sleep(u32 milliseconds)
{
	if (milliseconds >= 1000)
		Sleep(milliseconds / 1000);
	else
		Sleep(milliseconds);
}

// Memory

FR_API void 
*os_memory_reserve(u64 size)
{
	ASSERT(size == 0);
	
	void *result = VirtualAlloc(0, size, MEM_COMMIT, PAGE_READWRITE);
	if (result)
	{
		//LOG_DEBUG("MRS: 0x%X, %i", result, size);
	}
	else
	{
		LOG_ERROR("memory reserve failed");
		ASSERT(true);
	}
	
    return result;
}

FR_API void 
os_memory_release(void *memory)
{
	ASSERT(memory == 0);
	
	if (VirtualFree(memory, 0, MEM_RELEASE))
	{
		//LOG_DEBUG("MRL: 0x%X, %i", memory, size);
	}
	else
	{
		LOG_ERROR("memory release failed");
		ASSERT(true);
	}
}

// System

FR_API String8
os_file_read(String8 path)
{
    String8 result = {0};
    FILE *file = fopen((char *)path.str , "r");
    if (file != 0)
    {
        fseek(file, 0, SEEK_END);
        result.size = ftell(file) - 1;
        result.str = (u8 *)os_memory_reserve(result.size);
        fseek(file, 0, SEEK_SET);
        fread(result.str, 1, result.size, file);
        result.str[result.size - 1] = 0;
        fclose(file);
    }
    return result;
}

FR_API b8 
os_file_write(String8 path, void *memory, i32 memory_size)
{
	// TODO
#if 0
    HANDLE file_handle = CreateFileA((char *)path.str,
									 GENERIC_WRITE,
									 FILE_SHARE_READ,
									 0,
									 CREATE_ALWAYS,
									 0,
									 0);
    if (file_handle != INVALID_HANDLE_VALUE)
    {
		i32 file_bytes_write = 0;
        if (WriteFile(file_handle,
					  memory,
					  memory_size,
					  &file_bytes_write,
					  0) && memory_size == file_bytes_write)
        {
            CloseHandle(file_handle);
            return true;
        }
        else
        {
            ASSERT(true);
            LOG_ERROR("Write file failed");
            CloseHandle(file_handle);
            return false;
        }
    }
    else
	{
        ASSERT(true);
        LOG_ERROR("Open file failed");
        CloseHandle(file_handle);
        return false;
    }
#endif
	return 0;
}

FR_API b8 
os_file_is_exist(String8 path)
{
	HANDLE file_handle = CreateFileA((char *)path.str,
									 GENERIC_READ,
									 FILE_SHARE_READ,
									 0,
									 OPEN_EXISTING,
									 0,
									 0);
    if (file_handle != INVALID_HANDLE_VALUE) 
	{
		return true;
	}
	else
	{
		return false;
	}
}

FR_API void
*os_library_load(String8 path)
{
	ASSERT(path.size > 512);
	
	char path_ext[512] = {0};
	strcpy(path_ext, (char *)path.str);
	strncat(path_ext, FR_PLATFORM_EXT, strlen(path_ext));
	
	HMODULE result = LoadLibraryA(path_ext);
	ASSERT(result == (HMODULE)0);
	
	return result;
}

FR_API VoidFunction
*os_library_load_symbol(void* handle, String8 name)
{
	FARPROC result = GetProcAddress(handle, (char *)name.str);
	ASSERT(result == 0);
	
	return (VoidFunction*)result;
}

FR_API void
os_library_unload(void* handle)
{
	BOOL result = FreeLibrary(handle);
	ASSERT(result == 0);
}

// Window

internal Vector2i
win32_desktop_get_rect(void)
{
    RECT rect;
    HWND desktop = GetDesktopWindow();
    GetWindowRect(desktop, &rect);
    return v2i(rect.right, rect.bottom);
}

LRESULT CALLBACK 
win32_window_proc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
    LRESULT result = 0;
	
    switch (umsg)
    {
		case WM_CLOSE:
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
		case WM_PAINT:
		{
#if 1
			String8 greeting = str8_lit("Hello, World!");
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			
			FillRect(hdc, &ps.rcPaint, (HBRUSH)GetStockObject(DKGRAY_BRUSH));
			TextOutA(hdc, 5, 5, (LPCSTR)greeting.str, (i32)greeting.size);
			
			EndPaint(hwnd, &ps);
#endif
			break;
		}
		default:
		{
			result = DefWindowProcA(hwnd, umsg, wparam, lparam);
			break;
		}
    }
    return result;
}

#if FR_CONSOLE
int main(int argc, char *argv[])
#else
int WINAPI 
wWinMain(HINSTANCE instance, HINSTANCE prev_instance, PWSTR cmd_line, int cmd_show)
#endif
{
	/*
 Usage: forge.exe [OPTIONS]

Options:
-h, --help
--vestion
-f, --full-screen
--console
--vulkan
--opengl
	*/
	
	i32 arg_count = 0;
	char **arg_list = 0;
	
	arg_list = (char **)CommandLineToArgvW(GetCommandLineW(), &arg_count);
	
	LOG_DEBUG("[WIN] args: %i", arg_count);
	for (i32 i = 0; i < arg_count; i++)
	{
		if (arg_list[i][0] == '-')
		{
			LOG_DEBUG("[WIN] arg_list: %ws", arg_list[i]);
		}
	}
	
	app_run();
	
	return EXIT_SUCCESS;
}

FR_API WindowState
*os_window_create(String8 title, Vector2i position, Vector2i size)
{
	HMODULE module_handle = GetModuleHandleA(0);
	String8 window_class_name = str8_lit("Forge Class");
	
	// Initialization WindowState
	WindowState* win = MEMORY_MALLOC(sizeof(WindowState));
	MEMORY_ZERO_STRUCT(win, sizeof(WindowState));
	win->title = title;
	win->position = position;
	win->size = size;
	
	// Register the window class.
	WNDCLASSEXA wnd = {0};
	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wnd.lpfnWndProc = win32_window_proc;
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hInstance = module_handle;
	wnd.hIcon = 0; // TODO
	wnd.hCursor = LoadCursor(0, IDC_ARROW);
	wnd.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wnd.lpszMenuName = 0;
	wnd.lpszClassName = (LPCSTR)window_class_name.str;
	wnd.hIconSm = 0; // TODO
	if (!RegisterClassExA(&wnd))
	{
		ASSERT(true);
	}
	
	// Calculation position and adjust window size
	
	RECT rect = {0};
	rect.right = win->size.x;
	rect.bottom = win->size.y;
	
	DWORD window_style = WS_OVERLAPPEDWINDOW;
	AdjustWindowRect(&rect, window_style, false);
	
	win->size.x = rect.right - rect.left;
	win->size.y = rect.bottom - rect.top;
	
	win->position.x = (win->position.x + win32_desktop_get_rect().x) - (win32_desktop_get_rect().x / 2) - (win->size.x / 2);
	win->position.y = (win->position.y + win32_desktop_get_rect().y) - (win32_desktop_get_rect().y / 2) - (win->size.y / 2);
	
	// Create the window.
	{
		win->handle = CreateWindowExA(0, wnd.lpszClassName,
									  (LPCSTR)title.str,
									  WS_OVERLAPPEDWINDOW,
									  
									  win->position.x,
									  win->position.y,
									  win->size.x,
									  win->size.y,
									  
									  0, 0, module_handle, 0);
		ASSERT(win->handle == 0);
	}
	
	return win;
}

FR_API void
os_window_destroy(void* handle)
{
	ASSERT(handle == 0);
	DestroyWindow(handle);
}

FR_API b32
os_window_process_event(void* handle)
{
	ASSERT(handle == 0);
	MSG msg;
	if (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return false;
		}
		
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return true;
}

FR_API void
os_window_visibility(void* handle, b32 state)
{
	ASSERT(handle == 0);
	ShowWindow(handle, state);
	UpdateWindow(handle);
}

FR_API Vector2i
os_window_get_position(void* handle)
{
	ASSERT(handle == 0);
	RECT rect;
	GetWindowRect(handle, &rect);
	return v2i(rect.left, rect.top);
}

FR_API Vector2i
os_window_get_size(void* handle)
{
	ASSERT(handle == 0);
	RECT rect;
	GetWindowRect(handle, &rect);
	return v2i(rect.right - rect.left, rect.bottom - rect.top);
}

FR_API f64 
os_time_now(void)
{
	LARGE_INTEGER result;
	QueryPerformanceCounter(&result);
	return (f64)result.QuadPart;
}

FR_API f64 
os_time_frequency(void)
{
	LARGE_INTEGER result;
	local b8 is_init = false;
	if (!is_init)
	{
		QueryPerformanceFrequency(&result);
		is_init = true;
	}
	return (f64)result.QuadPart;
}