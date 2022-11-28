@echo off 

if "%VSCMD_INIT%"=="" (
	call "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat" x64
	set VSCMD_INIT="TRUE"
)

echo "-------------------------"
echo "        [Build]          "
echo "-------------------------"

:: variables

set ROOT=%cd%
set OUT_NAME=
set BUILD_DIR=%ROOT%\build
set CODE_DIR=%ROOT%\code
set MODE_COMPILE="debug"

set SOURCE=
set INCLUDE=
set LIBS=
set FLAGS=

:: -------- pre-build --------

if not exist %BUILD_DIR%\ mkdir %BUILD_DIR%

if "%1"==""        set MODE_COMPILE="debug"
if "%1"=="release" set MODE_COMPILE="release"

if %MODE_COMPILE%=="debug"   echo "debug"
if %MODE_COMPILE%=="release" echo "release"

pushd %BUILD_DIR%

:: -------- Forge --------

set OUT_NAME=forge

set OPTS=/W4 /wd4310 /wd4100 /wd4201 /wd4505 /wd4996 /wd4127 /wd4510 /wd4512 /wd4610 /wd4390 /wd4189 /WX

if %MODE_COMPILE%=="debug"   set OPTS=%OPTS% /std:c11 /GR- /EHa- /nologo /FC /Zi /Od /MTd /EHsc /Fe"%BUILD_DIR%\%OUT_NAME%d" /Fo"%BUILD_DIR%\%OUT_NAME%d"
if %MODE_COMPILE%=="release" set OPTS=%OPTS% /std:c11 /GR- /EHa- /nologo /FC /Zi /O2 /MT /EHsc /Fe"%BUILD_DIR%\%OUT_NAME%" /Fo"%BUILD_DIR%\%OUT_NAME%"

set SOURCE="%CODE_DIR%\forge.c"
set INCLUDE=/I"C:\Program Files (x86)\Windows Kits\10\INCLUDE\10.0.22621.0\ucrt" 
set INCLUDE=%INCLUDE% /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\um"
set INCLUDE=%INCLUDE% /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\shared"
set INCLUDE=%INCLUDE% /I"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.34.31933\INCLUDE" 
set LIBS=Kernel32.lib User32.lib Gdi32.lib
set FLAGS=/DFR_DYNAMIC_LINK_API=1 /DFR_ENABLE_ASSERT=1 /DFR_BUILD_INTERNAL=1 /DFR_BUILD_SLOW=1

:: -------- Executable --------
echo "[+] Building %OUT_NAME%.exe"
cl %INCLUDE% %LIBS% %FLAGS% %SOURCE% %OPTS%


:: -------- Sandbox --------

set OUT_NAME=sandbox

set OPTS=/W4 /wd4310 /wd4100 /wd4201 /wd4505 /wd4996 /wd4127 /wd4510 /wd4512 /wd4610 /wd4390 /wd4189 /WX

if %MODE_COMPILE%=="debug"   set OPTS=%OPTS% /std:c11 /GR- /EHa- /nologo /FC /Zi /Od /DLL /MTd /EHsc /LDd /Fe"%BUILD_DIR%\%OUT_NAME%d" /Fo"%BUILD_DIR%\%OUT_NAME%d"
if %MODE_COMPILE%=="release" set OPTS=%OPTS% /std:c11 /GR- /EHa- /nologo /FC /Zi /O2 /DLL /MT /EHsc /LD /Fe"%BUILD_DIR%\%OUT_NAME%" /Fo"%BUILD_DIR%\%OUT_NAME%"

set SOURCE="%CODE_DIR%\sandbox.c"
set INCLUDE=/I"C:\Program Files (x86)\Windows Kits\10\INCLUDE\10.0.22621.0\ucrt"
set INCLUDE=%INCLUDE% /I"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.34.31933\INCLUDE" 
set INCLUDE=%INCLUDE% /I"%CODE_DIR%"

if %MODE_COMPILE%=="debug"   set LIBS=forged.lib
if %MODE_COMPILE%=="release" set LIBS=forge.lib

set FLAGS=/DFR_STATIC_LINK_API=1 /DFR_ENABLE_ASSERT=1 /DFR_BUILD_INTERNAL=1 /DFR_BUILD_SLOW=1

:: -------- Dynamic library --------
echo "[+] Building %OUT_NAME%d.dll"
	cl %INCLUDE% %LIBS% %FLAGS% %SOURCE% %OPTS%
popd