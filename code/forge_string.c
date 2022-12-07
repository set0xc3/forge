FR_API inline String8 
str8(u8 *str, u64 size)
{
	String8 result = { str, size };
	return result;
}

FR_API String8 
string_remove_path(const String8 path)
{
	String8 result = path;
	i32 i = result.size - 1;
	for (; i >= 0 && 
		 result.str[i] != '/' && 
		 result.str[i] != '\\'; 
		 i -= 1);
	i32 slash_pos = i + 1;
	if (slash_pos >= 0)
	{
		result.str += slash_pos;
		result.size -= slash_pos;
	}
	return result;
}

FR_API String8 string_remove_name(const String8 path)
{
	String8 result = path;
	i32 i = result.size - 1;
	for (; i >= 0 && 
		 result.str[i] != '/' && 
		 result.str[i] != '\\'; 
		 i -= 1);
	i32 slash_pos = i + 1;
	if (slash_pos >= 0)
	{
		result.str[slash_pos - 1] = 0;
		result.size = slash_pos;
	}
	return result;
}

FR_API String8 string_remove_extension(const String8 path)
{
	String8 result = path;
	i32 i = result.size - 1;
	for (; i >= 0 && 
		 result.str[i] != '.'; 
		 i -= 1);
	i32 slash_pos = i + 1;
	if (slash_pos >= 0)
	{
		result.str[slash_pos - 1] = 0;
		result.size = slash_pos;
	}
	return result;
}

FR_API String8 
string_get_extension(const String8 path)
{
	String8 result = path;
	i32 i = result.size - 1;
	for (; i >= 0 && 
		 result.str[i] != '.'; 
		 i -= 1);
	i32 slash_pos = i + 1;
	if (slash_pos >= 0)
	{
		result.str += slash_pos;
		result.size -= slash_pos;
	}
	return result;
}