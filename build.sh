#!/bin/sh
cd `dirname $0`

echo "creating lua wrappers..."
swig -lua -o src/wrap_camera.c stuff/camera.i
swig -lua -o src/wrap_color.c stuff/color.i
swig -lua -o src/wrap_image.c stuff/image.i
swig -lua -o src/wrap_input.c stuff/input.i
swig -lua -o src/wrap_light.c stuff/light.i
swig -lua -o src/wrap_log.c stuff/log.i
swig -lua -o src/wrap_object.c stuff/object.i
swig -lua -o src/wrap_screen.c stuff/screen.i

echo "generating project for gcc..."
mkdir _CMAKE
cd _CMAKE
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=MinSizeRel ..

echo "building..."
make
cd ..

echo "moving to _build dir..."
mkdir _build
mv _CMAKE/wiz3d _build/wiz3d
mv _CMAKE/ide _build/ide

echo "running..."
cd _build
./wiz3d data/helloworld.lua
./wiz3d data/rotatingcube.lua
./wiz3d data/angel.lua

echo "done."

