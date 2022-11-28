#!/bin/sh

echo "-------------------------"
echo "        [Run]            "
echo "-------------------------"

ROOT=$PWD
PROGRAM_NAME=sandbox
BUILD_PATH=$ROOT/build

pushd $BUILD_PATH
	./$PROGRAM_NAME
popd