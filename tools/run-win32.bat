@echo off

echo "-------------------------"
echo "        [Run]            "
echo "-------------------------"

set ROOT=%cd%
set PROGRAM_NAME=forge
set BUILD_PATH=%ROOT%\build\debug\x64

pushd %BUILD_PATH%
	.\%PROGRAM_NAME%.exe
popd