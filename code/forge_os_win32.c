#include <Windows.h>

void __stdcall
win32_file_io_completion_routine(DWORD error_code,
								 DWORD number_of_bytes_transfered,
								 LPOVERLAPPED overlapped)
{
	LOG_ERROR("Error code:\t%x\n", error_code);
	LOG_INFO("Number of bytes:\t%x\n", number_of_bytes_transfered);
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

FR_API void os_memory_release(void *memory, u64 size)
{
	ASSERT(memory == 0 || size == 0);
	
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

FR_API DebugFileState
os_file_read(String8 path)
{
    DebugFileState result = {0};
	
	HANDLE file_handle = CreateFileA((char *)path.str,
									 GENERIC_READ,
									 FILE_SHARE_READ,
									 0,
									 OPEN_EXISTING,
									 0,
									 0);
    if (file_handle != INVALID_HANDLE_VALUE) 
    {
        LARGE_INTEGER tmp_size = {0}; 
        if ((GetFileSizeEx(file_handle, &tmp_size)))
        {
			i32 size32 = (i32)tmp_size.QuadPart;
			ASSERT(size32 >= i32_MAX);
			
            result.size = size32;
            result.data = MEMORY_MALLOC(result.size);
            DWORD file_bytes_read = 0;
            if (ReadFile(
						 file_handle,
						 result.data,
						 result.size,
						 &file_bytes_read,
						 0) && (DWORD)result.size == file_bytes_read)
            {
                return result;
            }
            else
            {
                ASSERT(true);
                LOG_ERROR("Read file failed");
                MEMORY_FREE(result.data);
                CloseHandle(file_handle);
                result.size = 0;
                result.data = 0;
                return result;
            }
        }
        else
        {
            ASSERT(true);
            LOG_ERROR("Get file size failed");
            CloseHandle(file_handle);
            return result;
        }
    }
    else
	{
        ASSERT(true);
        LOG_ERROR("Open file failed");
        CloseHandle(file_handle);
        return result;
    }
}

FR_API b8 
os_file_write(String8 path, void *memory, i32 memory_size)
{
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
        if (WriteFile(
					  file_handle,
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
}

FR_API b8 os_file_is_exist(String8 path)
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
win32_desktop_get_rect()
{
    RECT rect;
    HWND desktop = GetDesktopWindow();
    GetWindowRect(desktop, &rect);
    return v2i(rect.right, rect.bottom);
}

LRESULT CALLBACK win32_window_proc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
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
#if 0
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

typedef struct TestObject
{
	u64 id;
}TestObject;

#if FR_CONSOLE
int main(void)
#else
int WINAPI 
wWinMain(HINSTANCE instance, HINSTANCE prev_instance, PWSTR cmd_line, int cmd_show)
#endif
{
	app_entry_point();
	
	return EXIT_SUCCESS;
}

FR_API WindowState
* os_window_create(String8 title, Vector2i position, Vector2i size)
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
		win->handle = CreateWindowExA(0,                    	// Optional window styles.
									  wnd.lpszClassName,   	// Window class
									  (LPCSTR)title.str, 		// Window text
									  WS_OVERLAPPEDWINDOW, 	// Window style
									  
									  // Size and position
									  win->position.x,
									  win->position.y,
									  win->size.x,
									  win->size.y,
									  
									  0, // Parent window    
									  0, // Menu
									  module_handle, // Instance handle
									  0  // Additional application data
									  );
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

#if 0
WIN32_FILE_ATTRIBUTE_DATA file_info = {0};
is_error = GetFileAttributesExA(path,
								GetFileExInfoStandard,
								&file_info
								);
ASSERT(is_error == 0);
#endif