/*
	rexFrameSet.c

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
#include "rexFrameSet.h"

#include "../src/Render2d.h"

void rexInitFrameSet(struct rexFrameSet *fset)
{
	fset->frames = NULL;
	fset->numFrames = 0;
}

void rexInitFrameSetAsGrid (
	struct rexFrameSet *fset,
	unsigned int cols,
	unsigned int rows,
	float w,
	float h
)
{
	struct rexFrame frame = {
		.bounds = { 0, 0, w, h }
	};
	float uincr = 1.0f/(float)cols;
	float vincr = 1.0f/(float)rows;
	unsigned int frameNum = 0;
	for(unsigned int y = 0; y < rows; y++)
	{
		for(unsigned int x = 0; x < cols; x++)
		{
			// u
			frame.region.x = (float)x * uincr;

			// v
			frame.region.y = (float)y * vincr;

			// s
			frame.region.w = frame.region.x + uincr;

			// t
			frame.region.h = frame.region.y + vincr;

			// Sets the frame at the current num.
			fset->frames[frameNum++] = frame;

			// Check to make sure frame number doesn't exceed the amount of
			//	available frames.
			if(frameNum >= fset->numFrames)
			{
				return;
			}
		}
	}
}
