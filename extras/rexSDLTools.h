/*
	rexSDLTools.h

	Copyright 2017 (c) Michael Cabral.

	This software is free to use under the terms of the BSD-3 license.

	Please refer to the LICENSE file included with this source code. You can
	also find details here: https://opensource.org/licenses/BSD-3-Clause
*/

#pragma once

// Returns a rough FPS count.
float rexGetFPS();

// Sets up an SDL2 based environment.
void rexInitSDL (
	const char *title,
	unsigned int w,
	unsigned int h,
	int vsync,
	int fullscreen
);

// Swaps the SDL2 window to the screen.
void rexSwapWindow();

// Quits SDL and cleans up.
void rexQuitSDL();
