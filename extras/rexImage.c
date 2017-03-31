/*
	rexImage.c

	Copyright 2017 (c) Michael Cabral.

	This software is free to use under the terms of the BSD-3 license.

	Please refer to the LICENSE file included with this source code. You can
	also find details here: https://opensource.org/licenses/BSD-3-Clause
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "rexLogger.h"

// The STB image loader.
#define STB_IMAGE_IMPLEMENTATION
#include "../../stb/stb_image.h"

int rexLoadImage(struct rexImage *img, const char *path)
{
	img->pixels = stbi_load(path, &img->w, &img->h, &img->n, 0);
	if(!img->pixels)
	{
		rexLogError("Couldn't load image: %s", path);
		return 0;
	}
	rexLogInfo("Loaded image: %s, %d, %d, %d", path, img->w, img->h, img->n);
	return 1;
}

void rexFreeImage(struct rexImage *img)
{
	stbi_image_free(img->pixels);
}
