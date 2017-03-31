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
#define USE_FULLSCREEN false

// Number of bouncing ball sprites to render.
#define NUM_BALLS 20

// Path to the background image.
#define BG_IMAGE_PATH "examples/data/checkerboard.png"

// Path to the title image.
#define TITLE_IMAGE_PATH "examples/data/title.png"

// Path to the ball image.
#define BALL_IMAGE_PATH "examples/data/ball.png"

// Path to the test font image.
#define FONT_IMAGE_PATH "examples/data/font.png"

// We can use booleans.
#include <stdbool.h>

// These are for random numbers.
#include <stdlib.h>
#include <time.h>

// The SDL2 library.
#include <SDL2/SDL.h>

// Our Render2d modules.
#include "../../src/rLogger.h"
#include "../../src/rMath.h"
#include "../../src/rFrame.h"
#include "../../src/rText.h"
#include "../../src/rSprite.h"
#include "../../src/rNode.h"
#include "../../src/Render2d.h"

// The STB image loader.
#define STB_IMAGE_IMPLEMENTATION
#include "../../stb/stb_image.h"

// 256 frames for our ASCII font.
static struct rFrame FontFrames[256];

// When this is true, quit the program.
static bool QuitFlag = false;

// Some SDL data we want to keep a hold of.
static SDL_Window *RenderWindow = NULL;
static SDL_GLContext RenderContext = NULL;

// FPS counter based on method described here:
//	http://sdl.beuc.net/sdl.wiki/SDL_Average_FPS_Measurement

// Change at will. Lower means smoother, but higher values respond faster.
static float FPSAccuracy = 0.1f;

// Bookeeping variables for fps counting.
static Uint32 CurTicks = 0, TickDelta = 0, LastTicks = 0;
static float FrameTime = 0, FPS = 0;

// A little data structure to store image data.
struct Image
{
	int x, y, n;
	unsigned char *pixels;
};

// The ball sprite itself.
struct rSprite BallSprite;

// An array with all the balls we are going to render.
static struct rNode Balls[NUM_BALLS];

// Returns a random number in the given range.
static int RandomRange(int min, int max)
{
	return rand() % ((max + 1) - min) + min;
}

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
	int flags = SDL_WINDOW_OPENGL;
	if(USE_FULLSCREEN)
	{
		flags |= SDL_WINDOW_FULLSCREEN;
	}
	RenderWindow = SDL_CreateWindow (
		"Simple",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_W,
		WINDOW_H,
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

// This function takes care of bouncing the ball off the edges of the screen.
static void BounceBall(struct rNode *ball)
{
	rUpdateNode(ball);
	if (
		ball->x < 0.0f ||
		ball->x + ball->sprite->singleFrame.frame.bounds.w > WINDOW_W
	)
	{
		ball->dx *= -1.0f;
	}
	if (
		ball->y < 0.0f ||
		ball->y + ball->sprite->singleFrame.frame.bounds.h > WINDOW_H
	)
	{
		ball->dy *= -1.0f;
	}
}

// This is where the magic happens.
int main(int argc, char *argv[])
{
	// A nice text buffer to contain our FPS readout.
	char fpsStr[32] = "";

	// Kick off random number generator.
	srand(time(NULL));

	// Init everything.
	InitSDL();
	rInit();
	rSetViewport(WINDOW_W, WINDOW_H);

	// Our image variables.
	struct Image bgImage, titleImage, ballImage, fontImage;

	// Font frame set.
	struct rFrameSet fontFrameSet = {
		.frames = FontFrames,
		.numFrames = 256
	};
	rInitFrameSetAsGrid(&fontFrameSet, 16, 16, 16, 16);

	// Load our image assets.
	LoadImage(&bgImage, BG_IMAGE_PATH);
	LoadImage(&titleImage, TITLE_IMAGE_PATH);
	LoadImage(&ballImage, BALL_IMAGE_PATH);
	LoadImage(&fontImage, FONT_IMAGE_PATH);

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

	unsigned int fontTexId = rCreateTexture (
		fontImage.x, fontImage.y, fontImage.n, fontImage.pixels
	);

	// Init the ball sprite.
	rInitSprite(&BallSprite, R_SPRITE_TYPE_SINGLE_FRAME);
	BallSprite.texId = ballTexId;
	BallSprite.texId = ballTexId;
	BallSprite.singleFrame.frame.bounds.w = ballImage.x;
	BallSprite.singleFrame.frame.bounds.h = ballImage.y;

	// Initialize all the bouncing balls.
	for(int i = 0; i < NUM_BALLS; i++)
	{
		// We don't want all the balls bouncing in the same direction.
		float xdir = RandomRange(1, 100) < 50 ? -1.0f : 1.0f;
		float ydir = RandomRange(1, 100) < 50 ? -1.0f : 1.0f;

		// We probably don't want them going all at the same speed either.
		float speedx = RandomRange(1, 3);
		float speedy = RandomRange(1, 3);

		// Init the ball.
		rInitNode(&Balls[i]);
		Balls[i].sprite = &BallSprite;
		Balls[i].x = RandomRange(0, WINDOW_W - ballImage.x);
		Balls[i].y = RandomRange(0, WINDOW_H - ballImage.y);
		Balls[i].dx = speedx * xdir;
		Balls[i].dy = speedy * ydir;
	}

	// We don't need the image data any more so release it.
	FreeImage(&bgImage);
	FreeImage(&titleImage);
	FreeImage(&ballImage);
	FreeImage(&fontImage);

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

		// Copy the current FPS value to our FPS string.
		snprintf(fpsStr, 32, "FPS: %1.2f", FPS);

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

		// Loop through all the balls, and draw them.
		for(int i = 0; i < NUM_BALLS; i++)
		{
			// Draw the ball.
			rDrawNode(&Balls[i]);
		}

		// Lets draw the title.
		rSetTexture(titleTexId);

		// Draw the title image.
		rDraw(16, -16, titleImage.x, titleImage.y, 0, 0, 1, 1);

		// Set the font texture.
		rSetTexture(fontTexId);

		// Draw text with our font.
		rDrawText(fontFrameSet.frames, fpsStr, 16, WINDOW_H - 24, -6, 16);

		// Finish the render batch.
		rEnd();

		// Loop through all the balls, and animate them.
		for(int i = 0; i < NUM_BALLS; i++)
		{
			// Animate the ball.
			BounceBall(&Balls[i]);
		}

		// Swap to screen.
		SDL_GL_SwapWindow(RenderWindow);

		// Calculate the FPS
		CurTicks = SDL_GetTicks();
		TickDelta = CurTicks - LastTicks;
		LastTicks = CurTicks;
		FrameTime = FPSAccuracy * TickDelta + (1.0 - FPSAccuracy) * FrameTime;
		FPS = 1000.0 / FrameTime;
	}

	// Free up the texture.
	rDestroyTexture(bgTexId);

	// Quit the engine.
	rQuit();

	// Log out the final fps value. (Try taking off vsync!)
	rLogInfo("Final FPS: %f", FPS);

	// Free up SDL for a clean exit.
	SDL_GL_DeleteContext(RenderContext);
	SDL_DestroyWindow(RenderWindow);
	SDL_Quit();
}
