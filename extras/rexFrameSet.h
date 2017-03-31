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
