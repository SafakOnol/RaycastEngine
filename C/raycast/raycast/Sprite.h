#ifndef SPRITE_H
#define SPRITE_H

#include "Definitions.h"
#include "Graphics.h"
#include "Player.h"
#include "Texture.h"

typedef struct
{
	float x;
	float y;
	float distance;
	float angle;
	int textureArrayIndex;
} sprite_t;

void RenderSpriteProjection(void);
void RenderMiniMapSprites(void);

#endif // !SPRITE_H
