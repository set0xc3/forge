#ifndef OS_H
#define OS_H

typedef struct FileInfo
{
	i32   size;
	void *data;
}FileInfo;

typedef struct WindowState
{
	void*    handle;
	String8  title;
	Vector2i size;
	Vector2i position;
}WindowState;

FR_API void          os_setup(void);
FR_API void          os_shutdown(void);

FR_API void         *os_memory_reserve(u64 size);
FR_API void          os_memory_release(void *memory);

FR_API void          os_sleep(u32 milliseconds);

FR_API FileInfo      os_file_read(String8 path); /* function allocated memory */
FR_API b8            os_file_write(String8 path, void *memory, i32 memory_size);
FR_API b8            os_file_is_exist(String8 path);

FR_API void         *os_library_load(String8 path);
FR_API VoidFunction *os_library_load_symbol(void* handle, String8 name);
FR_API void          os_library_unload(void* handle);

FR_API WindowState  *os_window_create(String8 title, Vector2i position, Vector2i size);
FR_API void          os_window_destroy(void* handle);
FR_API b32           os_window_process_event(void* handle);
FR_API void          os_window_visibility(void* handle, b32 state);

FR_API Vector2i      os_window_get_position(void* handle);
FR_API Vector2i      os_window_get_size(void* handle);

FR_API f64           os_time_now(void);
FR_API f64           os_time_frequency(void);

#endif //OS_H