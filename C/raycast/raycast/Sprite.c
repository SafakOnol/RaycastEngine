#include "Sprite.h"

#define NUM_SPRITES 1

static sprite_t sprites[NUM_SPRITES] =
{
	{.x = 640, .y = 630, .textureArrayIndex = 9 }
};

void RenderSpriteProjection(void)
{
	// TODO:
}

void RenderMiniMapSprites(void)
{
	for (int i = 0; i < NUM_SPRITES; i++)
	{
		DrawRect(sprites[i].x * MINIMAP_SCALE_FACTOR, sprites[i].y * MINIMAP_SCALE_FACTOR, 2, 2, 0x0000FFFF);
	}
}
