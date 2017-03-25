/*
	Render2d.h

	Copyright 2017 (c) Michael Cabral.

	This software is free to use under the terms of the BSD-3 license.

	Please refer to the LICENSE file included with this source code. You can
	also find details here: https://opensource.org/licenses/BSD-3-Clause
*/

#pragma once

// Version number of the lib.
#define R_VERSION 0.01

// This number represents how many total quads can be buffered before requiring
//	a flush.
#define R_BATCH_BUFFER_SIZE 2048

// This number represents how many total draw calls can be batched before
//	needing to be flushed. The more the less often a flush occures.
#define R_MAX_DRAW_CALLS 512

// Start the renderer.
void rInit();

// Quit the renderer and clean up any memory.
void rQuit();

// Set's the viewport size.
void rSetViewport(unsigned int x, unsigned int y, unsigned w, unsigned h);

// Set's the clear color.
void rSetClearColor(float r, float g, float b, float a);

// Clears the current target.
void rClear();

// Begin a batch.
void rBegin();

// Creates a new texture from the given data.
unsigned int rCreateTexture (
	unsigned int w, unsigned int h, unsigned int n, unsigned char *pixels
);

// Destroys a texture.
void rDestroyTexture(unsigned int texId);

// Sets the current texture to render with.
void rSetTexture(unsigned int texId);

// Draw a square.
void rDraw (
	float x, float y, float w, float h, float u, float v, float s, float t
);

// End the batch. This will flush to the current render target.
void rEnd();
