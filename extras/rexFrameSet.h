/*
	rexFrameSet.h

	Copyright 2017 (c) Michael Cabral.

	This software is free to use under the terms of the BSD-3 license.

	Please refer to the LICENSE file included with this source code. You can
	also find details here: https://opensource.org/licenses/BSD-3-Clause
*/

#pragma once

// A basic frame set.
struct rexFrameSet
{
	// Pointer to memory housing the frames.
	struct rexFrame *frames;

	// Amount of frames in this set.
	unsigned int numFrames;
};

// Sets the frame set to its default values.
void rexInitFrameSet(struct rexFrameSet *fset);

// Inits the frameset as a grid of frames with the given cols, rows, w, h. This
//	function is great for things like sprite sheets/tilesheets without
//	requiring extra data types or file formats.
//	NOTE: This assumes a uniform grid of frames.
void rexInitFrameSetAsGrid (
	struct rexFrameSet *fset,
	unsigned int cols,
	unsigned int rows,
	float w,
	float h
);
