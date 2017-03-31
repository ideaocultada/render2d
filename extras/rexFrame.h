/*
	rexFrame.h

	Copyright 2017 (c) Michael Cabral.

	This software is free to use under the terms of the BSD-3 license.

	Please refer to the LICENSE file included with this source code. You can
	also find details here: https://opensource.org/licenses/BSD-3-Clause
*/

#pragma once

// This defines a frame you can use to draw with.
struct rexFrame
{
	// x, y are the offset of the frame, and w, h are the dimensions of the
	//	frame.
	struct rexRectf bounds;

	// The image revion (uvs) the frame draws.
	struct rexRectf region;
};
