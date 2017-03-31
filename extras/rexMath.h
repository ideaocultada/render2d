/*
	rexMath.h

	Copyright 2017 (c) Michael Cabral.

	This software is free to use under the terms of the BSD-3 license.

	Please refer to the LICENSE file included with this source code. You can
	also find details here: https://opensource.org/licenses/BSD-3-Clause
*/

#pragma once

// Your run of the mill point type.
struct rexPoint
{
	int x, y;
};

// Your run of the mill rect type.
struct rexRect
{
	int x, y, w, h;
};

// Your run of the mill point type with floating point values.
struct rexPointf
{
	float x, y;
};

// Your run of the mill rect type with floating point values.
struct rexRectf
{
	float x, y, w, h;
};

static inline float rexRectfLeft(struct rexRectf *r)
{
	return r->x;
}

static inline float rexRectfTop(struct rexRectf *r)
{
	return r->y;
}

static inline float rexRectfRight(struct rexRectf *r)
{
	return r->x + r->w;
}

static inline float rexRectfBottom(struct rexRectf *r)
{
	return r->y + r->h;
}

static inline float rexRectLeft(struct rexRect *r)
{
	return r->x;
}

static inline float rexRectTop(struct rexRect *r)
{
	return r->y;
}

static inline float rexRectRight(struct rexRect *r)
{
	return r->x + r->w;
}

static inline float rexRectBottom(struct rexRect *r)
{
	return r->y + r->h;
}
