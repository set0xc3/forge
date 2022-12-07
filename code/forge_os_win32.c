#include <Windows.h>

// Clock
global f64           win32_clock_frequency;

internal void 
win32_clock_init(void) 
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    win32_clock_frequency = 1.0 / (f64)frequency.QuadPart;
}

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
	win32_clock_init();
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

FR_API FileInfo
os_file_read(String8 path)
{
    FileInfo result = {0};
	
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
            result.data = os_memory_reserve(result.size);
            DWORD file_bytes_read = 0;
            if (ReadFile(file_handle,
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

typedef void GameStartFunction(void);
typedef void GameUpdateFunction(f32 delta_time);
typedef void GameStopFunction(void);

typedef struct GameBackend
{
	void               *libray_handle;
	GameStartFunction  *start;
	GameUpdateFunction *update;
	GameStopFunction   *stop;
}GameBackend;

#if FR_CONSOLE
int main(int argc, char *argv[])
#else
int WINAPI 
wWinMain(HINSTANCE instance, HINSTANCE prev_instance, PWSTR cmd_line, int cmd_show)
#endif
{
	b8 is_quit = false;
	WindowState *window_state = 0;
	GameBackend game_backend = {0};
	
	void *array_ptr = array_reserve(argc, String8);
	
	LOG_INFO("argc: %i", argc);
	for (i32 i = 0; i < argc; i += 1)
	{
		String8 *string = os_memory_reserve(sizeof(String8));
		string->str = (u8 *)argv[i];
		string->size = strlen(argv[i]);
		
		array_push(array_ptr, string, String8);
	}
	
	for (u32 i = 0; i < array_size(array_ptr); i += 1)
	{
		String8 string = *(String8*)array_at(array_ptr, i, String8);
		LOG_INFO("arg: %s", string.str);
	}
	
	os_setup();
	
	window_state = os_window_create(str8_lit("Forge"), v2i(0, 0), v2i(800, 600));
	os_window_visibility(window_state->handle, true);
	
	RendererBackend renderer_backend = renderer_backend_load();
	renderer_backend.init(window_state->handle);
	
	game_backend.libray_handle = os_library_load(str8_lit("game"));
	game_backend.start         = (GameStartFunction *)  os_library_load_symbol(game_backend.libray_handle, str8_lit("start"));
	game_backend.update        = (GameUpdateFunction *) os_library_load_symbol(game_backend.libray_handle, str8_lit("update"));
	game_backend.stop          = (GameStopFunction *)   os_library_load_symbol(game_backend.libray_handle, str8_lit("stop"));
	
	// Start Modules
	{
		game_backend.start();
	}
	
	f32 fps_max = 60.0f;
	f32 period_max = 1.0f / fps_max;
	
	f64 begin_ticks = os_get_now_time();
	f64 end_ticks = 0;
	end_ticks  = os_get_now_time();
	
	LARGE_INTEGER perf_count_frequency_result;
    QueryPerformanceFrequency(&perf_count_frequency_result);
	f64 perf_count_frequency = perf_count_frequency_result.QuadPart;
	
	LARGE_INTEGER last_counter;
	f64 last_cycle_count;
	
	LARGE_INTEGER end_counter;
	QueryPerformanceCounter(&end_counter);
	last_counter = end_counter;
	
	while (!is_quit)
	{
		f64 end_cycle_count = (f64)__rdtsc();
		QueryPerformanceCounter(&end_counter);
		
		f64 cycles_elapsed  = (f64)end_cycle_count - last_cycle_count;
		f64 counter_elapsed = (f64)end_counter.QuadPart - last_counter.QuadPart;
		f32 ms_per_frame    = (f32)counter_elapsed / (f32)perf_count_frequency;
		f32 fps             = (f32)perf_count_frequency / (f32)counter_elapsed;
		f32 mcpf            = (f32)cycles_elapsed / 1000000.0f;
		
#if 0
		LOG_TRACE("period_max:           %f", (f32)period_max);
		
		LOG_TRACE("perf_count_frequency: %f", (f32)perf_count_frequency);
		
		LOG_TRACE("end_counter:          %f", (f64)end_counter.QuadPart);
		LOG_TRACE("last_counter:         %f", (f64)last_counter.QuadPart);
		
		LOG_TRACE("end_cycle_count:      %f", (f64)end_cycle_count);
		LOG_TRACE("last_cycle_count:     %f", (f64)last_cycle_count);
		
		LOG_TRACE("cycles_elapsed:       %f", (f64)cycles_elapsed);
		LOG_TRACE("counter_elapsed:      %f", (f64)counter_elapsed);
		LOG_TRACE("ms_per_frame:         %f", (f32)ms_per_frame);
		LOG_TRACE("fps:                  %f", (f32)fps);
		LOG_TRACE("mcpf:                 %f", (f32)mcpf);
#endif
		
#if 1
		if (ms_per_frame >= period_max)
#endif
		{
			if (ms_per_frame >= 1.0f)
			{
				ms_per_frame = period_max;
			}
			
			if (!os_window_process_event(window_state->handle))
			{
				is_quit = true;
			}
			
			// Update Modules
			{
				game_backend.update(ms_per_frame);
			}
			
			//for (u64 i = 0; i < 1000000; i++)
			{
				renderer_backend.begin(window_state->handle);
				renderer_backend.submit(window_state->handle);
				renderer_backend.end(window_state->handle);
			}
			
			LOG_TRACE("[OS] fps: %.02ff/s, %.02fms/f, %.02fmc/f, %.02f/ghz/f", fps, ms_per_frame, mcpf, fps * mcpf);
			
			last_counter = end_counter;
			last_cycle_count = end_cycle_count;
		}
	}
	
	// Stop Modules
	{
		game_backend.stop();
	}
	
	os_shutdown();
	
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
os_get_now_time(void)
{
	LARGE_INTEGER now_time;
    QueryPerformanceCounter(&now_time);
    return (f64)now_time.QuadPart * win32_clock_frequency;
}

FR_API void 
os_clock_start(Clock *clock)
{
	clock->start_time = os_get_now_time();
    clock->elapsed = 0;
}

FR_API void 
os_clock_stop(Clock *clock)
{
	clock->start_time = 0;
}

FR_API void 
os_clock_update(Clock *clock)
{
	if (clock->start_time != 0) 
	{
        clock->elapsed = os_get_now_time() - clock->start_time;
    }
}

#if 0
WIN32_FILE_ATTRIBUTE_DATA file_info = {0};
is_error = GetFileAttributesExA(path,
								GetFileExInfoStandard,
								&file_info
								);
ASSERT(is_error == 0);
#endif