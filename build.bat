@echo off

echo creating lua wrappers...
stuff\swig.exe -lua -o src/wrap_camera.c stuff/camera.i
stuff\swig.exe -lua -o src/wrap_color.c stuff/color.i
stuff\swig.exe -lua -o src/wrap_image.c stuff/image.i
stuff\swig.exe -lua -o src/wrap_input.c stuff/input.i
stuff\swig.exe -lua -o src/wrap_light.c stuff/light.i
stuff\swig.exe -lua -o src/wrap_log.c stuff/log.i
stuff\swig.exe -lua -o src/wrap_object.c stuff/object.i
stuff\swig.exe -lua -o src/wrap_screen.c stuff/screen.i

echo generating project for mingw...
mkdir _CMAKE
cd _CMAKE
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=MinSizeRel ..

echo building...
mingw32-make
cd ..

echo moving to _build dir...
mkdir _build
move "_CMAKE\wiz3d.exe" "_build"
move "_CMAKE\ide.exe" "_build"

echo running...
cd _build
wiz3d.exe
log.txt
pause
