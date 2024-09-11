#ifndef SPRITE_H
#define SPRITE_H

#include "Definitions.h"
#include "Graphics.h"
#include "Player.h"
#include "Texture.h"
#include "Utility.h"

typedef struct
{
	float x;
	float y;
	float distance;
	float angle;
	bool bIsVisible;
	int textureArrayIndex;
} sprite_t;

void RenderMiniMapSprites(void);
void RenderSpriteProjection(void);


#endif // !SPRITE_H
