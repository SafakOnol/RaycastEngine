#ifndef SPRITE_H
#define SPRITE_H

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
