/*
	rText.c

	Copyright 2017 (c) Michael Cabral.

	This software is free to use under the terms of the BSD-3 license.

	Please refer to the LICENSE file included with this source code. You can
	also find details here: https://opensource.org/licenses/BSD-3-Clause
*/

// C headers.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Render2d modules we use.
#include "rMath.h"
#include "rFrame.h"
#include "rText.h"
#include "Render2d.h"

void rDrawText (
	struct rFrame *font,
	const char *text,
	float x,
	float y,
	float letting,
	float kerning
)
{
	// Cache the length of the strung.
	unsigned int strLen = strlen(text);

	// The "cursors" coordinates for tracking where we are in the text.
	unsigned int cursorX = 0;
	unsigned int cursorY = 0;

	// Loop through each character in the string.
	for(unsigned int i = 0; i < strLen; i++)
	{
		// Cache the character.
		char c = text[i];

		// Calculate the char index.
		unsigned int charIndex = c - 32;

		// If we have a new-line advance cursorY.
		if(c == '\n')
		{
			cursorY += kerning;
			cursorX = 0;
		}
		// Ensure we are a standard ASCII character.
		else if(c >= 32)
		{
			// Skip spaces.
			if(c != 32)
			{
				// Draw the frame.
				rDrawFrame(&font[charIndex], x + cursorX, y + cursorY);
			}

			// Offset the cursorX position.
			cursorX += (
				font[charIndex].bounds.w
			) + letting;
		}
	}
}


struct rRectf rCalcTextBounds (
	struct rFrame *font,
	const char *text,
	float letting,
	float kerning
)
{
	struct rRectf r = { 0.0f, 0.0f, 0.0f, 0.0f };

	// Cache the length of the strung.
	unsigned int strLen = strlen(text);

	// The "cursors" coordinates for tracking where we are in the text.
	unsigned int cursorX = 0;
	unsigned int cursorY = 0;

	// Loop through each character in the string.
	for(unsigned int i = 0; i < strLen; i++)
	{
		// Cache the character.
		char c = text[i];

		// Calculate the char index.
		unsigned int charIndex = c - 32;

		// Expand the bounds rect based on the current character and
		//	cursorX/cursorY.
		float w = cursorX + font[charIndex].bounds.w;
		float h = cursorY + font[charIndex].bounds.h;
		if(w > r.w)
		{
			r.w = w;
		}
		if(h > r.h)
		{
			r.h = h;
		}

		// If we have a new-line advance cursorY.
		if(c == '\n')
		{
			cursorY += kerning;
			cursorX = 0;
		}

		// Ensure we are a standard ASCII character.
		else if(c >= 32)
		{
			// Offset the cursorX position.
			cursorX += (
				font[charIndex].bounds.w
			) + letting;
		}
	}

	return r;
}
