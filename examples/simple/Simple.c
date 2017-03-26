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

// Path to the title image.
#define TITLE_IMAGE_PATH "examples/data/title.png"

// Path to the ball image.
#define BALL_IMAGE_PATH "examples/data/ball.png"

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

// A little data structure to store image data.
struct Image
{
	int x, y, n;
	unsigned char *pixels;
};

// Data for the ball.
struct Ball
{
	int w, h;
	float x, y, dx, dy;
};

// Load image data.
static void LoadImage(struct Image *img, const char *path)
{
	img->pixels = stbi_load(path, &img->x, &img->y, &img->n, 0);
	if(!img->pixels)
	{
		rLogError("Couldn't load image: %s", path);
		exit(-1);
	}
	rLogInfo("Loaded image: %s, %d, %d, %d", path, img->x, img->y, img->n);
}

// Freeimage data.
static void FreeImage(struct Image *img)
{
	stbi_image_free(img->pixels);
}

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

	int glw, glh;
	SDL_GL_GetDrawableSize(RenderWindow, &glw, &glh);
	rLogInfo("glw: %d, glh: %d", glw, glh);

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

// This function takes care of bouncing the ball off the edges of the screen.
static void BounceBall(struct Ball *ball)
{
	ball->x += ball->dx;
	ball->y += ball->dy;
	if(ball->x < 0.0f || ball->x + ball->w > WINDOW_W)
	{
		ball->dx *= -1.0f;
	}
	if(ball->y < 0.0f || ball->y + ball->h > WINDOW_H)
	{
		ball->dy *= -1.0f;
	}
}

// This is where the magic happens.
int main(int argc, char *argv[])
{
	// Init everything.
	InitSDL();
	rInit();
	rSetViewport(WINDOW_W, WINDOW_H);

	// Our image variables.
	struct Image bgImage, titleImage, ballImage;

	// Load our image assets.
	LoadImage(&bgImage, BG_IMAGE_PATH);
	LoadImage(&titleImage, TITLE_IMAGE_PATH);
	LoadImage(&ballImage, BALL_IMAGE_PATH);

	// The state of the ball.
	struct Ball ball = (struct Ball) {
		ballImage.x, ballImage.y, 0, 0, 2.0f, 2.0f
	};

	// Create the background texture.
	unsigned int bgTexId = rCreateTexture (
		bgImage.x, bgImage.y, bgImage.n, bgImage.pixels
	);

	// Create the title texture.
	unsigned int titleTexId = rCreateTexture (
		titleImage.x, titleImage.y, titleImage.n, titleImage.pixels
	);

	// Create the ball.
	unsigned int ballTexId = rCreateTexture (
		ballImage.x, ballImage.y, ballImage.n, ballImage.pixels
	);

	// We don't need the image data any more so release it.
	FreeImage(&bgImage);
	FreeImage(&titleImage);
	FreeImage(&ballImage);

	// Calculate the s/t coords based on the window dimentsions so we get a
	//	nice tiling effect.
	float s = (float)WINDOW_W / (float)bgImage.x;
	float t = (float)WINDOW_H / (float)bgImage.y;

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
		scrollDelta += 0.0025f;

		// Prevent unecessary rollover. Shouldn't really matter but I'm OCD.
		if(scrollDelta > 1.0f)
		{
			scrollDelta -= 1.0f;
		}

		// Set the ball texture.
		rSetTexture(ballTexId);

		// Draw the ball.
		rDraw(ball.x, ball.y, ballImage.x, ballImage.y, 0, 0, 1, 1);

		// Animate the ball.
		BounceBall(&ball);

		// Lets draw the title.
		rSetTexture(titleTexId);

		// Draw the title image.
		rDraw(16, -16, titleImage.x, titleImage.y, 0, 0, 1, 1);

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
