/*
	rexFrame.c

	Copyright 2017 (c) Michael Cabral.

	This software is free to use under the terms of the BSD-3 license.

	Please refer to the LICENSE file included with this source code. You can
	also find details here: https://opensource.org/licenses/BSD-3-Clause
*/

// C headers.
#include <stdlib.h>

// Render2d modules we use.
#include "rexMath.h"
#include "rexFrame.h"

#include "../src/Render2d.h"

void rexInitFrame(struct rexFrame *frame)
{
	// We init the size of every frame to something visible.
	frame->bounds = (struct rexRectf) { 0, 0, 64, 64 };

	// Frames use the x,y,w,h variables of the rect as u,v,s,t.
	frame->region = (struct rexRectf) { 0, 0, 1, 1 };
}

void rexDrawFrame(struct rexFrame *frame, float x, float y)
{
	rDraw (
		x + frame->bounds.x,
		y + frame->bounds.y,
		frame->bounds.w,
		frame->bounds.h,
		frame->region.x,
		frame->region.y,
		frame->region.w,
		frame->region.h
	);
}
