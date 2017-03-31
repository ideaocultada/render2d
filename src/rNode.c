/*
	rNode.c

	Copyright 2017 (c) Michael Cabral.

	This software is free to use under the terms of the BSD-3 license.

	Please refer to the LICENSE file included with this source code. You can
	also find details here: https://opensource.org/licenses/BSD-3-Clause
*/

#include <stdlib.h>

// Render2d modules we use.
#include "rMath.h"
#include "rFrame.h"
#include "rSprite.h"
#include "rNode.h"
#include "Render2d.h"

void rInitNode(struct rNode *node)
{
	node->x = 0.0f;
	node->y = 0.0f;
	node->dx = 0.0f;
	node->dy = 0.0f;
	node->sprite = NULL;
}

void rUpdateNode(struct rNode *node)
{
	node->x += node->dx;
	node->y += node->dy;
}

void rDrawNode(struct rNode *node)
{
	if(!node->sprite)
	{
		return;
	}
	rDrawSprite(node->sprite, node->x, node->y);
}

struct rRectf rCalcNodeBounds(struct rNode *node)
{
	if(node->sprite)
	{
		struct rRectf r = rCalcSpriteBounds(node->sprite);
		r.x += node->x;
		r.y += node->y;
		return r;
	}
	return (struct rRectf) { node->x, node->y, 0.0f, 0.0f };
}
