/*
	Simple.c

	Copyright 2017 (c) Michael Cabral.

	This software is free to use under the terms of the BSD-3 license.

	Please refer to the LICENSE file included with this source code. You can
	also find details here: https://opensource.org/licenses/BSD-3-Clause
*/

#include <stdbool.h>

#include <SDL2/SDL.h>

#include "../../src/Render2d.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../stb/stb_image.h"

static bool QuitFlag = false;
static SDL_Window *RenderWindow = NULL;
static SDL_GLContext RenderContext = NULL;

static void InitSDL()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	RenderWindow = SDL_CreateWindow (
		"Simple",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800,
		600,
		SDL_WINDOW_OPENGL
	);
	SDL_GL_CreateContext(RenderWindow);
	SDL_GL_SetSwapInterval(1);
}

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
	InitSDL();
	rInit();
	rSetViewport(800, 600);

	int x, y, n;
	unsigned char *pixels = stbi_load (
		"examples/data/beachball.png", &x, &y, &n, 0
	);

	if(!pixels)
	{
		fprintf(stderr, "Couldn't load image!\n");
		exit(-1);
	}

	int m = 0;

	unsigned int texId = rCreateTexture(x, y, n, pixels);
	while(!QuitFlag)
	{
		HandleSDLEvents();
		rClear();
		rBegin();
		rSetTexture(texId);
		rDraw(m++, 0, 256, 256, 0, 0, 1, 1);
		rEnd();
		SDL_GL_SwapWindow(RenderWindow);
	}
}
