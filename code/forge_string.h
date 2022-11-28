#ifndef F_STRING_H
#define F_STRING_H

typedef struct String8
{
	u8 *str;
	u64 size;
}String8;

FR_API String8 str8(u8 *str, u32 size);
#define str8_lit(s) str8((u8 *)s, sizeof(s) - 1)

FR_API String8 string_remove_path(const String8 path);
FR_API String8 string_remove_name(const String8 path);
FR_API String8 string_remove_extension(const String8 path);

FR_API String8 string_get_extension(const String8 path);

#endif //F_STRING_H