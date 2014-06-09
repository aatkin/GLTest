# GLTest #

A desktop-OpenGL test project on X Window System (X11), using
  - [g++ 4.8.2](http://gcc.gnu.org/)
  - [GLFW 3.0.4](http://www.glfw.org/)
  - [GLEW 1.10.0](http://glew.sourceforge.net/)
  - [Code::Blocks 13.12](http://www.codeblocks.org/)

Developed and tested on Ubuntu 14.04 with Geforce GT415M and Nvidia binary driver version 331.38.

## Building ##
Before building dependencies, you should have [CMake](http://www.cmake.org/) installed. CMake should be pretty easy to install using popular package managers, such as `apt`. You should also have fairly modern GPU drivers installed, with support for >=OpenGL 3.2.

GLFW and GLEW dependencies need to be built and linked to the project before the project can be built. Easiest way (in my opinion) is to use the makefiles in both libraries:
  - GLFW3
    - `$ cd glfw-3.0.4 && sudo cmake "Unix Makefiles" && sudo make && sudo make install`
  - GLEW
    - `$ cd glew-1.10.0 && sudo make && sudo make install`

If CMake fails with an error, you might be missing some dependencies. You can then try to install the following:
  - `$ sudo apt-get install cmake xorg-dev libglu1-mesa-dev`

GCC should find these libraries by default at build-time. Last thing to do is to compile and build:
  - `$ g++ -std=c++11 -c src/main.cpp`
  - `$ g++ main.o -o main -lglfw3 -lGLEW -lGL -lGLU -lX11 -lXxf86vm -lXrandr -lpthread -lXi`

If the compiler complains about missing libraries (like Xxf86vm), google them up and install as needed.
