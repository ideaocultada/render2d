/*
	Simple.c

	Copyright 2017 (c) Michael Cabral.

	This software is free to use under the terms of the BSD-3 license.

	Please refer to the LICENSE file included with this source code. You can
	also find details here: https://opensource.org/licenses/BSD-3-Clause
*/

#define WINDOW_W	1024
#define WINDOW_H	768
#define USE_VSYNC	1
#define USE_AA		1
#define ANIO_LEVEL	4

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
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, USE_AA);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, ANIO_LEVEL);
	RenderWindow = SDL_CreateWindow (
		"Simple",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_W,
		WINDOW_H,
		SDL_WINDOW_OPENGL
	);
	SDL_GL_CreateContext(RenderWindow);
	SDL_GL_SetSwapInterval(USE_VSYNC);
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
	rSetViewport(WINDOW_W, WINDOW_H);

	int x, y, n;
	unsigned char *pixels = stbi_load (
		"examples/data/beachball.png", &x, &y, &n, 0
	);

	if(!pixels)
	{
		fprintf(stderr, "Couldn't load image!\n");
		exit(-1);
	}

	float m = (float)WINDOW_W / (float)x;
	float k = (float)WINDOW_H / (float)y;
	float f = 0.0f;

	unsigned int texId = rCreateTexture(x, y, n, pixels);
	while(!QuitFlag)
	{
		HandleSDLEvents();
		rClear();
		rBegin();
		rSetTexture(texId);
		rDraw(0, 0, WINDOW_W, WINDOW_H, f, f, k + f, m + f);
		f += 0.01f;
		rEnd();
		SDL_GL_SwapWindow(RenderWindow);
	}
}
