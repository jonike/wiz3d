# Wiz3D
Wiz3D is a lightweight programming environment based on Lua which has been designed to teach kids programming by making games. Its main features are:

* OpenGL fixed pipeline graphics.
* SDL for window, input and sound management.
* ODE for physics simulation.

The core runtime has been written in pure C, with the goal of being easily portable to any platform. It has currently been tested on Windows, macOS and Linux.

## Building on Windows
The libraries have been compiled with TDM-GCC. This compiler suite has not been updated in a while, but it can compile Wiz3D without any issues.

In order to compile, just double click the `build.bat` file. The executable file `wiz3d.exe` will be created on the `_build` folder.

## Building on macOS
You should have **Cmake** and **swig3.0** installed to be able to build the engine. You can install them for example with [Brew](https://brew.sh/). After installing Brew on your system, type:

`$brew install cmake`
`$brew install swig3.0`

Then, just double click from Finder on `build.command` and the executable file `wiz3d` will be created on the `_build` folder (it is a 32 bit binary).

## Building on Linux
You should have **Cmake** and **swig3.0** installed to be able to build the engine. For example, to install them on Ubuntu, type:

`$sudo apt install cmake`
`$sudo apt install swig3.0`

Then, from a Terminal go to the Wiz3D folder and run the build script:

`$./build.sh`

The executable file `wiz3d` will be created on the `_build` folder (it is a 64 bit binary).

## Using Wiz3D
The runtime accepts one optional parameter with the name of the Lua script to run. If you don't provide the parameter, then the file `data/main.lua` will be run.

The script folder is always set as the active directory when running a program, so all paths should be relative to this folder.
