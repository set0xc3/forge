@echo off

: MODE_COMPILE ARCH MODE_RENDERER
: debug x64 vulkan

echo "-------------------------"
echo "        [Build]          "
echo "-------------------------"


: -------- variables --------

set ROOT=%cd%
set OUT_NAME=
set ARCH="x64"
set BUILD_DIR=%ROOT%\build
set CODE_DIR=%ROOT%\code
set MODE_COMPILE="debug"
set MODE_RENDERER="vulkan"

set SOURCE=
set INCLUDE=
set LIBS=
set FLAGS=

set INCLUDE=-I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\ucrt" 
set INCLUDE=%INCLUDE% -I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\um"
set INCLUDE=%INCLUDE% -I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\shared"
set INCLUDE=%INCLUDE% -I"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.34.31933\include" 

: -------- pre-build --------

if "%1"=="debug"   set MODE_COMPILE="debug"
if "%1"=="release" set MODE_COMPILE="release"

if "%2"=="x32"     set ARCH="x32"
if "%2"=="x64"     set ARCH="x64"

if "%3"=="vulkan"  set MODE_RENDERER="vulkan"
if "%3"=="opengl"  set MODE_RENDERER="opengl"

if not exist %BUILD_DIR%\                      mkdir %BUILD_DIR%
if not exist %BUILD_DIR%\%MODE_COMPILE%        mkdir %BUILD_DIR%\%MODE_COMPILE%
if not exist %BUILD_DIR%\%MODE_COMPILE%\%ARCH% mkdir %BUILD_DIR%\%MODE_COMPILE%\%ARCH%
	

pushd %BUILD_DIR%\%MODE_COMPILE%\%ARCH%


: -------- forge --------

set OUT_NAME=forge
set SOURCE="%CODE_DIR%\forge.c"
set LIBS=-lKernel32.lib -lUser32.lib -lGdi32.lib
set FLAGS=-std=c11 -O0 -Wall -Wextra
set FLAGS=%FLAGS% -Wno-unused-parameter -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -Wno-missing-declarations -Wno-missing-braces -Wno-implicit-function-declaration -Wno-deprecated-declarations -Wno-nonportable-include-path -Wno-ignored-pragma-intrinsic -Wno-ignored-attributes -Wno-pragma-pack -Wno-unknown-pragmas -Wno-microsoft-anon-tag
set FLAGS=%FLAGS% -DFR_PLATFORM_WINDOWS=1 -DFR_DYNAMIC_LINK_API=1 -DFR_CONSOLE=1

if %ARCH%=="x32" set FLAGS=%FLAGS% -m32
if %ARCH%=="x64" set FLAGS=%FLAGS% -m64

if %MODE_COMPILE%=="debug"   set FLAGS=%FLAGS% -g -DFR_ENABLE_ASSERT=1 -DFR_BUILD_INTERNAL=1 -DFR_BUILD_SLOW=1
if %MODE_COMPILE%=="release" set FLAGS=%FLAGS%

if %MODE_RENDERER%=="vulkan" set FLAGS=%FLAGS% -DFR_VULKAN=1
if %MODE_RENDERER%=="opengl" set FLAGS=%FLAGS% -DFR_OPENGL=1


: -------- executable --------

echo "[+] Building %OUT_NAME%.exe"
clang %INCLUDE% %LIBS% %FLAGS% %SOURCE% -o .\%OUT_NAME%.exe


: -------- Sandbox --------

set OUT_NAME=sandbox
set SOURCE="%CODE_DIR%\sandbox.c"

set LIBS=-lforge.lib
set FLAGS=-std=c11 -O0 -Wall -Wextra
set FLAGS=%FLAGS% -Wno-unused-parameter -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -Wno-missing-declarations -Wno-missing-braces -Wno-implicit-function-declaration -Wno-deprecated-declarations -Wno-nonportable-include-path -Wno-ignored-pragma-intrinsic -Wno-ignored-attributes -Wno-pragma-pack -Wno-unknown-pragmas -Wno-microsoft-anon-tag

if %ARCH%=="x32" set FLAGS=%FLAGS% -m32
if %ARCH%=="x64" set FLAGS=%FLAGS% -m64

if %MODE_COMPILE%=="debug"   set FLAGS=%FLAGS% -g -DFR_ENABLE_ASSERT=1 -DFR_BUILD_INTERNAL=1 -DFR_BUILD_SLOW=1
if %MODE_COMPILE%=="release" set FLAGS=%FLAGS%

set FLAGS=%FLAGS% -DFR_PLATFORM_WINDOWS=1 -DFR_STATIC_LINK_API=1

set INCLUDE=%INCLUDE% -I"%CODE_DIR%"


: -------- Dynamic library --------
:echo "[+] Building %OUT_NAME%.dll"
:clang  %INCLUDE% %FLAGS% %SOURCE% -c -o .\%OUT_NAME%.o
:clang %INCLUDE% %LIBS% %FLAGS% -shared -o .\%OUT_NAME%.dll .\%OUT_NAME%.o


: -------- Renderer Vulkan --------

set OUT_NAME=renderer_vulkan
set SOURCE="%CODE_DIR%\forge_renderer_vulkan.c"

set LIBS=-lUser32.lib -lGdi32.lib -lvulkan-1.lib -lforge.lib
set FLAGS=-std=c11 -O0 -Wall -Wextra
set FLAGS=%FLAGS% -Wno-unused-parameter -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -Wno-missing-declarations -Wno-missing-braces -Wno-implicit-function-declaration -Wno-deprecated-declarations -Wno-nonportable-include-path -Wno-ignored-pragma-intrinsic -Wno-ignored-attributes -Wno-pragma-pack -Wno-unknown-pragmas -Wno-microsoft-anon-tag

if %ARCH%=="x32" set FLAGS=%FLAGS% -m32
if %ARCH%=="x64" set FLAGS=%FLAGS% -m64

if %ARCH%=="x32" set LIBS=%LIBS% -L"C:\VulkanSDK\1.3.231.1\Lib32"
if %ARCH%=="x64" set LIBS=%LIBS% -L"C:\VulkanSDK\1.3.231.1\Lib"

if %MODE_COMPILE%=="debug"   set FLAGS=%FLAGS% -g -DFR_ENABLE_ASSERT=1 -DFR_BUILD_INTERNAL=1 -DFR_BUILD_SLOW=1
if %MODE_COMPILE%=="release" set FLAGS=%FLAGS%

set FLAGS=%FLAGS% -DFR_PLATFORM_WINDOWS=1 -DFR_DYNAMIC_LINK_API=1

set INCLUDE=%INCLUDE% -I"C:\VulkanSDK\1.3.231.1\Include"
set INCLUDE=%INCLUDE% -I"%CODE_DIR%"

: -------- Dynamic library --------
if %MODE_RENDERER%=="vulkan" (
echo "[+] Building %OUT_NAME%.dll"
clang %INCLUDE% %FLAGS% %SOURCE% -c -o .\%OUT_NAME%.o
clang %INCLUDE% %LIBS% %FLAGS% -shared -o .\%OUT_NAME%.dll .\%OUT_NAME%.o
)


: -------- Renderer OpenGL --------

set OUT_NAME=renderer_opengl
set SOURCE="%CODE_DIR%\forge_renderer_opengl.c"

set LIBS=-lUser32.lib -lGdi32.lib -lopengl32.lib -lforge.lib
set FLAGS=-std=c11 -O0 -Wall -Wextra
set FLAGS=%FLAGS% -Wno-unused-parameter -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -Wno-missing-declarations -Wno-missing-braces -Wno-implicit-function-declaration -Wno-deprecated-declarations -Wno-nonportable-include-path -Wno-ignored-pragma-intrinsic -Wno-ignored-attributes -Wno-pragma-pack -Wno-unknown-pragmas -Wno-microsoft-anon-tag

if %ARCH%=="x32" set FLAGS=%FLAGS% -m32
if %ARCH%=="x64" set FLAGS=%FLAGS% -m64

if %MODE_COMPILE%=="debug"   set FLAGS=%FLAGS% -g -DFR_ENABLE_ASSERT=1 -DFR_BUILD_INTERNAL=1 -DFR_BUILD_SLOW=1
if %MODE_COMPILE%=="release" set FLAGS=%FLAGS%

set FLAGS=%FLAGS% -DFR_PLATFORM_WINDOWS=1 -DFR_DYNAMIC_LINK_API=1

set INCLUDE=%INCLUDE% -I"%CODE_DIR%"

: -------- Dynamic library --------
if %MODE_RENDERER%=="opengl" (
echo "[+] Building %OUT_NAME%.dll"
clang  %INCLUDE% %FLAGS% %SOURCE% -c -o .\%OUT_NAME%.o
clang %INCLUDE% %LIBS% %FLAGS% -shared -o .\%OUT_NAME%.dll .\%OUT_NAME%.o
)

popd