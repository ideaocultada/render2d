/*
	rexImage.h

	Copyright 2017 (c) Michael Cabral.

	This software is free to use under the terms of the BSD-3 license.

	Please refer to the LICENSE file included with this source code. You can
	also find details here: https://opensource.org/licenses/BSD-3-Clause
*/

#pragma once

struct rexImage
{
	int w, h, n;
	unsigned char *pixels;
};

int rexLoadImage(struct rexImage *img, const char *path);

void rexFreeImage(struct rexImage *img);
