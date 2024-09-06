#include "Sprite.h"

#define NUM_SPRITES 1

static sprite_t sprites[NUM_SPRITES] =
{
	{.x = 640, .y = 630, .textureArrayIndex = 9 }
};

void RenderMiniMapSprites(void)
{
	for (int i = 0; i < NUM_SPRITES; i++)
	{
		DrawRect
		(
			sprites[i].x * MINIMAP_SCALE_FACTOR, 
			sprites[i].y * MINIMAP_SCALE_FACTOR, 
			2, 
			2, 
			(sprites[i].bIsVisible) ? 0xFF00FFFF : 0xFF444444
		);
	}
}

void RenderSpriteProjection(void)
{
	sprite_t visibleSprites[NUM_SPRITES];
	int numVisibleSprites = 0;

	// Find sprites inside FOV
	for (int i = 0; i < NUM_SPRITES; i++)
	{
		float angleBetweenSpriteAndPlayer = player.rotationAngle - atan2(sprites[i].y - player.y, sprites[i].x - player.x);
		
		// Check if the angle is between 0 - 180 degrees
		if (angleBetweenSpriteAndPlayer > PI)
		{
			angleBetweenSpriteAndPlayer -= TWO_PI;
			//printf("angle more than PI! \n");
		}
			
		//printf("angleBetweenSpriteAndPlayer: %f \n", angleBetweenSpriteAndPlayer * 180 / PI);
		angleBetweenSpriteAndPlayer = fabs(angleBetweenSpriteAndPlayer);
		

		// check if the angle is less than half of the FOV angle and compansate with an error margin
		if (angleBetweenSpriteAndPlayer < (FOV_ANGLE * 0.5))
		{
			sprites[i].bIsVisible = true;
			sprites[i].angle = angleBetweenSpriteAndPlayer;
			sprites[i].distance = DistanceBetweenPoints(sprites[i].x, sprites[i].y, player.x, player.y);
			visibleSprites[numVisibleSprites] = sprites[i]; // we could use a form of append here maybe?? Safak
			numVisibleSprites++;
		}
		else
		{
			sprites[i].bIsVisible = false;
		}
	}

	for (int i = 0; i < numVisibleSprites; i++)
	{
		// TODO: draw the pixels of the sprite in the correct position
	}
}
