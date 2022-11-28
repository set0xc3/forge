#ifndef LOGGER_H
#define LOGGER_H

FR_API void logger_print(const char* type, const char* fmt, ...);
internal void logger_print_all();

#define LOG_INFO(...)    logger_print("[INFO] ", ##__VA_ARGS__)
#if FR_BUILD_SLOW
#define LOG_DEBUG(...)   logger_print("[DEBUG] ", ##__VA_ARGS__)
#define LOG_TRACE(...)   logger_print("[TRACE] ", ##__VA_ARGS__)
#define LOG_WARNING(...) logger_print("[WARNING] ", ##__VA_ARGS__)
#define LOG_ERROR(...)   logger_print("[ERROR] ", ##__VA_ARGS__)
#define LOG_FATAL(...)   logger_print("[FATAL] ", ##__VA_ARGS__)
#else
#define LOG_DEBUG(...)
#define LOG_TRACE(...)
#define LOG_WARNING(...)
#define LOG_ERROR(...)
#define LOG_FATAL(...)
#endif

#endif //LOGGER_H