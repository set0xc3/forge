FR_API void
logger_print(const char* type, const char* fmt, ...)
{
	char log_buffer[512];
	sprintf(log_buffer, "%s", type);
	
	va_list va;
	va_start(va, fmt);
	{
		char buffer[512];
		vsprintf(buffer, fmt, va);
		strcat(log_buffer, buffer);
		fprintf(stderr, "%s\n", log_buffer);
	}
	va_end(va);
}

internal void 
logger_print_all()
{
	LOG_INFO("Test: %i", 123);
	LOG_DEBUG("Test: %i", 123);
	LOG_TRACE("Test: %i", 123);
	LOG_WARNING("Test: %i", 123);
	LOG_ERROR("Test: %i", 123);
	LOG_FATAL("Test: %i", 123);
}