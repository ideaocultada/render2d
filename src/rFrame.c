/*
	rFrame.c

	Copyright 2017 (c) Michael Cabral.

	This software is free to use under the terms of the BSD-3 license.

	Please refer to the LICENSE file included with this source code. You can
	also find details here: https://opensource.org/licenses/BSD-3-Clause
*/

// Render2d modules we use.
#include "rMath.h"
#include "rFrame.h"
#include "Render2d.h"

void rInitFrame(struct rFrame *frame)
{
	frame->offset = (struct rPointf) { 0, 0 };

	// We init the size of every frame to something visible.
	frame->bounds = (struct rPointf) { 64, 64 };

	// Frames use the x,y,w,h variables of the rect as u,v,s,t.
	frame->region = (struct rRectf) { 0, 0, 1, 1 };
}

void rDrawFrame(struct rFrame *frame, float x, float y)
{
	rDraw (
		x + frame->offset.x,
		y + frame->offset.y,
		frame->bounds.x,
		frame->bounds.y,
		frame->region.x,
		frame->region.y,
		frame->region.w,
		frame->region.h
	);
}
