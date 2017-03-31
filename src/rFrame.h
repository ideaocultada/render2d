/*
	rFrame.h

	Copyright 2017 (c) Michael Cabral.

	This software is free to use under the terms of the BSD-3 license.

	Please refer to the LICENSE file included with this source code. You can
	also find details here: https://opensource.org/licenses/BSD-3-Clause
*/

#pragma once

// This defines a frame you can use to draw with.
struct rFrame
{
	// x, y are the offset of the frame, and w, h are the dimensions of the
	//	frame.
	struct rRectf bounds;

	// The image revion (uvs) the frame draws.
	struct rRectf region;
};

// A basic frame set.
struct rFrameSet
{
	// Pointer to memory housing the frames.
	struct rFrame *frames;

	// Amount of frames in this set.
	unsigned int numFrames;
};

// Initialize a frame to its default values.
void rInitFrame(struct rFrame *frame);

// Draw the frame using our standard calls.
void rDrawFrame(struct rFrame *frame, float x, float y);

// Sets the frame set to its default values.
void rInitFrameSet(struct rFrameSet *fset);

// Inits the frameset as a grid of frames with the given cols, rows, w, h. This
//	function is great for things like sprite sheets/tilesheets without
//	requiring extra data types or file formats.
//	NOTE: This assumes a uniform grid of frames.
void rInitFrameSetAsGrid (
	struct rFrameSet *fset,
	unsigned int cols,
	unsigned int rows,
	float w,
	float h
);
