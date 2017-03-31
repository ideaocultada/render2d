/*
	rSprite.h

	Copyright 2017 (c) Michael Cabral.

	This software is free to use under the terms of the BSD-3 license.

	Please refer to the LICENSE file included with this source code. You can
	also find details here: https://opensource.org/licenses/BSD-3-Clause
*/

#pragma once

enum
{
	R_SPRITE_TYPE_SINGLE_FRAME,
	R_SPRITE_TYPE_FRAME_SET,
	R_SPRITE_TYPE_TEXT_AREA,
	R_NUM_SPRITE_TYPES
};

struct rSprite
{
	unsigned char type;
	unsigned int texId;
	union
	{
		// R_SPRITE_TYPE_SINGLE_FRAME
		struct
		{
			struct rFrame frame;

		} singleFrame;

		// R_SPRITE_TYPE_FRAME_SET
		struct
		{
			struct rFrameSet *frameSet;
			unsigned int curFrameIndex;

		} frameSet;

		// R_SPRITE_TYPE_TEXT_AREA
		struct
		{
			struct rFrame *font;
			const char *text;
			float letting;
			float kerning;

		} textArea;
	};
};

// Initialize the sprite with the given type.
void rInitSprite(struct rSprite *sprite, unsigned char type);

// Draw the sprite.
void rDrawSprite(struct rSprite *sprite, float x, float y);

// Calculate the bounds of the sprite.
struct rRectf rCalcSpriteBounds(struct rSprite *sprite);
