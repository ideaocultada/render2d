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
	// Sometimes you want to offset the frames origin.
	struct rPointf offset;

	// Actual pixel w/h of the frame.
	struct rPointf bounds;

	// The image revion (uvs) the frame draws.
	struct rRectf region;
};

// Initialize a frame to its default values.
void rInitFrame(struct rFrame *frame);

// Draw the frame using our standard calls.
void rDrawFrame(struct rFrame *frame, float x, float y);
