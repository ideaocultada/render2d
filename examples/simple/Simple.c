/*
	Simple.c

	Copyright 2017 (c) Michael Cabral.

	This software is free to use under the terms of the BSD-3 license.

	Please refer to the LICENSE file included with this source code. You can
	also find details here: https://opensource.org/licenses/BSD-3-Clause
*/

// Self explanatory window settings.
#define WINDOW_W	800
#define WINDOW_H	600
#define USE_VSYNC	1
#define USE_AA		1
#define ANIO_LEVEL	4

// Path to the background image.
#define BG_IMAGE_PATH "examples/data/checkerboard.png"

// We can use booleans.
#include <stdbool.h>

// The SDL2 library.
#include <SDL2/SDL.h>

// Our Render2d modules.
#include "../../src/rLogger.h"
#include "../../src/Render2d.h"

// The STB image loader.
#define STB_IMAGE_IMPLEMENTATION
#include "../../stb/stb_image.h"

// When this is true, quit the program.
static bool QuitFlag = false;

// Some SDL data we want to keep a hold of.
static SDL_Window *RenderWindow = NULL;
static SDL_GLContext RenderContext = NULL;

// This function initialies SDL.
static void InitSDL()
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
	RenderWindow = SDL_CreateWindow (
		"Simple",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_W,
		WINDOW_H,
		SDL_WINDOW_OPENGL
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
	if(SDL_GL_SetSwapInterval(USE_VSYNC))
	{
		goto sdl_error__;
	};

	// If we get here, everything went well.
	return;

	// Our error handling function which basically logs a message and exits.
	sdl_error__:
	rLogError("%s", SDL_GetError());
	exit(-1);
}

// Handle user events via SDL.
static void HandleSDLEvents()
{
	SDL_Event event;

	// Loop through events until we get a quit event.
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

// This is where the magic happens.
int main(int argc, char *argv[])
{
	// Init everything.
	InitSDL();
	rInit();
	rSetViewport(WINDOW_W, WINDOW_H);

	// Load the background image.
	int x, y, n;
	unsigned char *pixels = stbi_load(BG_IMAGE_PATH, &x, &y, &n, 0);

	// Make sure it loaded correctly.
	if(!pixels)
	{
		rLogError("Couldn't load image: %s", BG_IMAGE_PATH);
		exit(-1);
	}

	// Create the background texture.
	unsigned int bgTexId = rCreateTexture(x, y, n, pixels);

	// We don't need the image data any more so release it.
	stbi_image_free(pixels);

	// Calculate the s/t coords based on the window dimentsions so we get a
	//	nice tiling effect.
	float s = (float)WINDOW_W / (float)x;
	float t = (float)WINDOW_H / (float)y;

	// An offset we modify the uvs by to achive a scrolling effect.
	float scrollDelta = 0.0f;

	// Loop until quit flag has been set to true.
	while(!QuitFlag)
	{
		// Handle any events.
		HandleSDLEvents();

		// Clear the scene.
		rClear();

		// Begin a new render batch.
		rBegin();

		// Set the texture to the bg texture.
		rSetTexture(bgTexId);

		// Draw the geometry.
		rDraw (
			0,
			0,
			WINDOW_W,
			WINDOW_H,
			scrollDelta,
			scrollDelta,
			s + scrollDelta,
			t + scrollDelta
		);

		// Increment the scroll delta.
		scrollDelta += 0.01f;

		// Prevent unecessary rollover. Shouldn't really matter but I'm OCD.
		if(scrollDelta > 1.0f)
		{
			scrollDelta -= 1.0f;
		}

		// Finish the render batch.
		rEnd();

		// Swap to screen.
		SDL_GL_SwapWindow(RenderWindow);
	}

	// Free up the texture.
	rDestroyTexture(bgTexId);

	// Quit the engine.
	rQuit();

	// Free up SDL for a clean exit.
	SDL_GL_DeleteContext(RenderContext);
	SDL_DestroyWindow(RenderWindow);
	SDL_Quit();
}
