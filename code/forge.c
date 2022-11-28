#include "forge.h"

#include "forge_base_types.c"
#include "forge_array.c"
#include "forge_linked_list.c"
#include "forge_string.c"
#include "forge_logger.c"

#if FR_PLATFORM_LINUX
#include "FR_PLATFORM_LINUX/forge_linux_main.c"
#elif FR_PLATFORM_WINDOWS
#include <Windows.h>
#include "forge_win32_main.c"
#else
#error missing platform detection
#endif

#include "forge_render.c"

#include "app.c"