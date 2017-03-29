/*
	rText.h

	Copyright 2017 (c) Michael Cabral.

	This software is free to use under the terms of the BSD-3 license.

	Please refer to the LICENSE file included with this source code. You can
	also find details here: https://opensource.org/licenses/BSD-3-Clause
*/

#pragma once

// This expects an array of 256 frames. Each frame containing a glyph. It will
//	use these frames to render the given text to the screen.
//	Adjusting the letting makes the letters closer together or farther appart.
//	Adjusting the kerning makes the lines closer together or farther appart.
void rDrawText (
	struct rFrame *font,
	const char *text,
	float x,
	float y,
	float letting,
	float kerning
);
