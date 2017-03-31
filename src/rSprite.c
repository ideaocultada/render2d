/*
	rSprite.c

	Copyright 2017 (c) Michael Cabral.

	This software is free to use under the terms of the BSD-3 license.

	Please refer to the LICENSE file included with this source code. You can
	also find details here: https://opensource.org/licenses/BSD-3-Clause
*/

// C headers.
#include <stdlib.h>

// Render2d modules we use.
#include "rMath.h"
#include "rFrame.h"
#include "rSprite.h"
#include "rText.h"
#include "Render2d.h"

// Calculate the bounds of the sprite.
struct rRectf rCalcSpriteBounds(struct rSprite *sprite)
{
	switch(sprite->type)
	{
		case R_SPRITE_TYPE_SINGLE_FRAME:
			return sprite->singleFrame.frame.bounds;
			break;
		case R_SPRITE_TYPE_FRAME_SET:
			return sprite->frameSet.frameSet->frames [
				sprite->frameSet.curFrameIndex
			].bounds;
			break;
		case R_SPRITE_TYPE_TEXT_AREA:
			if(sprite->textArea.font && sprite->textArea.text)
			{
				return rCalcTextBounds(
					sprite->textArea.font,
					sprite->textArea.text,
					sprite->textArea.letting,
					sprite->textArea.kerning
				);
			}
			break;
		default:
			break;
	}
	return (struct rRectf) { 0, 0, 0, 0 };
}

void rInitSprite(struct rSprite *sprite, unsigned char type)
{
	sprite->type = type;
	sprite->texId = 0;
	switch(type)
	{
		case R_SPRITE_TYPE_SINGLE_FRAME:
			rInitFrame(&sprite->singleFrame.frame);
			break;
		case R_SPRITE_TYPE_FRAME_SET:
			sprite->frameSet.frameSet = NULL;
			sprite->frameSet.curFrameIndex = 0;
			break;
		case R_SPRITE_TYPE_TEXT_AREA:
			sprite->textArea.font = NULL;
			sprite->textArea.text = NULL;
			sprite->textArea.letting = 0;
			sprite->textArea.kerning = 0;
			break;
		default:
			break;
	}
}

void rDrawSprite(struct rSprite *sprite, float x, float y)
{
	if(!sprite->texId)
	{
		return;
	}
	rSetTexture(sprite->texId);
	switch(sprite->type)
	{
		case R_SPRITE_TYPE_SINGLE_FRAME:
			rDrawFrame(&sprite->singleFrame.frame, x, y);
			break;
		case R_SPRITE_TYPE_FRAME_SET:
			if(!sprite->frameSet.frameSet)
			{
				return;
			}
			rDrawFrame (
				&sprite->frameSet.frameSet->frames [
					sprite->frameSet.curFrameIndex
				],
				x,
				y
			);
			break;
		case R_SPRITE_TYPE_TEXT_AREA:
			if(!sprite->textArea.font || !sprite->textArea.text)
			{
				return;
			}
			rDrawText (
				sprite->textArea.font,
				sprite->textArea.text,
				x,
				y,
				sprite->textArea.letting,
				sprite->textArea.kerning
			);
			break;
		default:
			break;
	}
}
