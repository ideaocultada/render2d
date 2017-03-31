/*
	rexSDLTools.c

	Copyright 2017 (c) Michael Cabral.

	This software is free to use under the terms of the BSD-3 license.

	Please refer to the LICENSE file included with this source code. You can
	also find details here: https://opensource.org/licenses/BSD-3-Clause
*/

// The SDL2 library.
#include <SDL2/SDL.h>

// Our Render2d modules.
#include "../../src/Render2d.h"

// Some SDL data we want to keep a hold of.
static SDL_Window *RenderWindow = NULL;
static SDL_GLContext RenderContext = NULL;

// Change at will. Lower means smoother, but higher values respond faster.
static float FPSAccuracy = 0.1f;

// Bookeeping variables for fps counting.
static Uint32 CurTicks = 0, TickDelta = 0, LastTicks = 0;
static float FrameTime = 0, FPS = 0;

float rexGetFPS()
{
	return FPS;
}

void rexInitSDL (
	const char *title,
	unsigned int w,
	unsigned int h,
	int vsync,
	int fullscreen
)
{
	if(SDL_Init(SDL_INIT_VIDEO))
	{
		goto sdl_error__;
	}
	if(SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, USE_AA))
	{
		goto sdl_error__;
	}
	if(SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, ANIO_LEVEL))
	{
		goto sdl_error__;
	}
	int flags = SDL_WINDOW_OPENGL;
	if(fullscreen)
	{
		flags |= SDL_WINDOW_FULLSCREEN;
	}
	RenderWindow = SDL_CreateWindow (
		title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		w,
		h,
		flags
	);
	if(!RenderWindow)
	{
		goto sdl_error__;
	}
	RenderContext = SDL_GL_CreateContext(RenderWindow);
	if(!RenderContext)
	{
		goto sdl_error__;
	}
	if(SDL_GL_SetSwapInterval(vsync ? 1 : 0))
	{
		goto sdl_error__;
	};

	// If we get here, everything went well.
	return;

	// Our error handling function which basically logs a message and exits.
	sdl_error__:
	rexFatalError("%s", SDL_GetError());
}

void rexSwapWindow()
{
	// Swap to screen.
	SDL_GL_SwapWindow(RenderWindow);

	// Calculate the FPS
	CurTicks = SDL_GetTicks();
	TickDelta = CurTicks - LastTicks;
	LastTicks = CurTicks;
	FrameTime = FPSAccuracy * TickDelta + (1.0 - FPSAccuracy) * FrameTime;
	FPS = 1000.0 / FrameTime;
}

void rexQuitSDL()
{
	// Log out the final fps value.
	rexLogInfo("Quitting SDL. Final FPS: %f", FPS);

	// Free up SDL for a clean exit.
	SDL_GL_DeleteContext(RenderContext);
	SDL_DestroyWindow(RenderWindow);
	SDL_Quit();
}
