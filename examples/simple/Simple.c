/*
	Simple.c

	Copyright 2017 (c) Michael Cabral.

	This software is free to use under the terms of the BSD-3 license.

	Please refer to the LICENSE file included with this source code. You can
	also find details here: https://opensource.org/licenses/BSD-3-Clause
*/

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "../../extras/rexSDLTools.h"
#include "../../src/Render2d.h"

#define WINDOW_W		800
#define WINDOW_H		600
#define USE_VSYNC 		1
#define USE_FULLSCREEN 	0
#define WINDOW_TITLE 	"Simple"

static bool QuitFlag = false;

static void HandleSDLEvents()
{
	SDL_Event event;
	while(SDL_PollEvent(&event) && !QuitFlag)
	{
		switch(event.type)
		{
			case SDL_QUIT:
				QuitFlag = true;
				break;
			case SDL_KEYUP:
				if(event.key.keysym.sym == SDLK_ESCAPE)
				{
					QuitFlag = true;
				}
				break;
			default:
				break;
		}
	}
}

int main(int argc, char *argv[])
{
	rexInitSDL (
		WINDOW_TITLE,
		WINDOW_W,
		WINDOW_H,
		USE_VSYNC,
		USE_FULLSCREEN
	);
	rInit();
	rSetViewport(WINDOW_W, WINDOW_H);
	while(!QuitFlag)
	{
		HandleSDLEvents();
		rClear();
		rexSwapWindow();
	}
	rQuit();
	rexQuitSDL();
	return 0;
}
