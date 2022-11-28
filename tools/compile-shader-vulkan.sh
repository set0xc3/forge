#!/bin/sh

echo "-------------------------"
echo "        [Compile]        "
echo "-------------------------"

ROOT=$PWD

/usr/bin/glslc $ROOT/assets/shaders/shader.vert -o $ROOT/assets/shaders/shader.vert.spv
/usr/bin/glslc $ROOT/assets/shaders/shader.frag -o $ROOT/assets/shaders/shader.frag.spv
