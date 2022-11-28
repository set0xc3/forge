#!/bin/bash

echo "-------------------------"
echo "        [Build]          "
echo "-------------------------"

## Variables

ROOT=$PWD
OUT_NAME=
BUILD_PATH=$ROOT/build

SOURCE=
INCLUDE=
LIBS=
FLAGS=

#----------------------------------------------------------------------------------------------

# Pre-build

[ ! -d $BUILD_PATH ] && mkdir -p $BUILD_PATH

cd $BUILD_PATH

#----------------------------------------------------------------------------------------------

# Forge

OUT_NAME=libforge
SOURCE="$ROOT/code/forge.c"
FLAGS="
  -DFR_PLATFORM_LINUX=1 -DFR_ENABLE_ASSERT=1 -DFR_BUILD_INTERNAL=1 -DFR_BUILD_SLOW=1
  -std=c11 -g -O0 -m64 -Wall -Werror -Wextra
"
## Dynamic library
echo "[+] Building '"$OUT_NAME"d.so'"
clang $INCLUDE $LIBS $FLAGS $SOURCE -fPIC -c -o $OUT_NAME.o
clang $INCLUDE $LIBS $FLAGS -shared -o "$OUT_NAME"d.so "$OUT_NAME".o

#----------------------------------------------------------------------------------------------

# Fire

#OUT_NAME=libfire
#SOURCE="$ROOT/code/fire.c"
#INCLUDE="-I$ROOT/code"
#FLAGS="
#  -DFR_PLATFORM_LINUX=1 -DFR_ENABLE_ASSERT=1 -DFR_BUILD_INTERNAL=1 -DFR_BUILD_SLOW=1
#  -std=c11 -g -O0 -m64 -Wall -Werror -Wextra
#"
## Dynamic library
#echo "[+] Building '"$OUT_NAME"d.so'"
#clang $INCLUDE $LIBS $FLAGS $SOURCE -fPIC -c -o $OUT_NAME.o
#clang $INCLUDE $LIBS $FLAGS -shared -o "$OUT_NAME"d.so "$OUT_NAME".o

#----------------------------------------------------------------------------------------------

# Sandbox

OUT_NAME=sandbox
SOURCE="$ROOT/code/sandbox.c"
INCLUDE="-I$ROOT/code"
LIBS="
  -lm -lGL -lvulkan -lxcb -lX11 -lX11-xcb
  -L$ROOT/build -lforged
"
FLAGS="
  -DFR_PLATFORM_LINUX=1 -DFR_ENABLE_ASSERT=1 -DFR_BUILD_INTERNAL=1 -DFR_BUILD_SLOW=1
  -std=c11 -g -O0 -m64 -Wl,-rpath="." -Wall -Werror -Wextra
"
## Executable
echo "[+] Building '"$OUT_NAME"'"
clang $INCLUDE $LIBS $FLAGS $SOURCE -o $OUT_NAME
echo ""

#----------------------------------------------------------------------------------------------

# TMP
## Static library
#echo "[+] Building '"$OUT_NAME".o'"
#ar r $OUT_NAME.a $OUT_NAME.o