# Render2d
A very simple OpenGL 2D engine written in C.

## Overview
The engine itself is just plain C and OpenGL. It does not generate a window or a context for you. Some examples of how to use the engine are provided using [SDL2](https://www.libsdl.org/download-2.0.php). You can build the examples by running 'scons' at the project root using [SCons](http://scons.org/).

## Misc
The project also includes the awesome [stb](https://github.com/nothings/stb) image library. The [GLEW](http://glew.sourceforge.net/) library is used for windows and linux support.

## High-DPI Screen Scaling Issues
Because I am a lazy caveman who doesn't want to be bothered with an IDE, I have no convenient way to tell the demo app to give me a non dpi-scaled window. On MacOS it is good enough to give it a specific resolution and everything is fine. On on Windows using [TDM-GCC](http://tdm-gcc.tdragon.net/) (I'm a caveman remember!) it's a bit more annoying. You will need to set the compatibility settings on the generated app to not give you a DPI-scaled window.

After doing some research apparently you can set this setting programmatically if you are using Visual Studio. But I couldn't be bothered right now. Kinda' lame, I know. I'll get to it at some point!

## Screenshots

![](screenshots/simple.png?raw=true "Simple")
