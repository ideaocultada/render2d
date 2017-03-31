/*
	rMath.h

	Copyright 2017 (c) Michael Cabral.

	This software is free to use under the terms of the BSD-3 license.

	Please refer to the LICENSE file included with this source code. You can
	also find details here: https://opensource.org/licenses/BSD-3-Clause
*/

#pragma once

// Your run of the mill point type.
struct rPoint
{
	int x, y;
};

// Your run of the mill rect type.
struct rRect
{
	int x, y, w, h;
};

// Your run of the mill point type with floating point values.
struct rPointf
{
	float x, y;
};

// Your run of the mill rect type with floating point values.
struct rRectf
{
	float x, y, w, h;
};

static inline float rRectfLeft(struct rRectf *r)
{
	return r->x;
}

static inline float rRectfTop(struct rRectf *r)
{
	return r->y;
}

static inline float rRectfRight(struct rRectf *r)
{
	return r->x + r->w;
}

static inline float rRectfBottom(struct rRectf *r)
{
	return r->y + r->h;
}

static inline float rRectLeft(struct rRect *r)
{
	return r->x;
}

static inline float rRectTop(struct rRect *r)
{
	return r->y;
}

static inline float rRectRight(struct rRect *r)
{
	return r->x + r->w;
}

static inline float rRectBottom(struct rRect *r)
{
	return r->y + r->h;
}
