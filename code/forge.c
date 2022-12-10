#include "forge.h"

#include "forge_app.h"

#include "forge_vector.c"
#include "forge_array.c"
#include "forge_linked_list.c"
#include "forge_string.c"
#include "forge_logger.c"

#include "forge_renderer.c"

#if FR_PLATFORM_LINUX
#include "FR_PLATFORM_LINUX/forge_os_linux.c"
#elif FR_PLATFORM_WINDOWS
#include "forge_os_win32.c"
#else
#error missing platform detection
#endif

#include "forge_app.c"