#include "Sprite.h"

#define NUM_SPRITES 3

static sprite_t sprites[NUM_SPRITES] =
{
	{ .x = 640, .y = 630, .textureArrayIndex = 9 },  // Barrel
	{ .x = 1300, .y = 700, .textureArrayIndex = 11 }, // Table
	{ .x = 140, .y = 630, .textureArrayIndex = 12 }   // Guard
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
		
		const float DRAW_MARGIN = 0.1;
		
		if (angleBetweenSpriteAndPlayer < (FOV_ANGLE * 0.5) + DRAW_MARGIN)
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
		sprite_t sprite = visibleSprites[i];

		// Calculate projected sprite height (TILE_SIZE is the original sprite height since this engine only allows one square size)
		float spriteHeight = (TILE_SIZE / sprite.distance) * DISTANCE_TO_PROJECTION_PLANE;
		float spriteWidth = spriteHeight;

		float spriteTopY = (RESOLUTION_WINDOW_HEIGHT * 0.5) - (spriteHeight * 0.5);
		spriteTopY = (spriteTopY < 0) ? 0 : spriteTopY;

		float spriteBottomY = (RESOLUTION_WINDOW_HEIGHT * 0.5) + (spriteHeight * 0.5);
		spriteBottomY = (spriteBottomY > RESOLUTION_WINDOW_HEIGHT) ? RESOLUTION_WINDOW_HEIGHT : spriteBottomY;

		float spriteAngle = atan2((sprite.y - player.y), (sprite.x - player.x)) - player.rotationAngle;

		float spriteScreenPosX = tan(spriteAngle) * DISTANCE_TO_PROJECTION_PLANE;
		float spriteLeftX = (RESOLUTION_WINDOW_WIDTH * 0.5) + spriteScreenPosX - (spriteWidth * 0.5);
		float spriteRightX = spriteLeftX + spriteWidth;

		// Query the width and height of the texture

		int textureWidth = upng_get_width(textures[sprite.textureArrayIndex]);
		int textureHeight = upng_get_height(textures[sprite.textureArrayIndex]);


		for (int x = spriteLeftX; x < spriteRightX; x++)
		{
			float texelWidth = (textureWidth / spriteWidth);
			int textureOffsetX = (x - spriteLeftX) * texelWidth;

			for (int y = spriteTopY; y < spriteBottomY; y++)
			{
				int distanceFromTop = y + (spriteHeight * 0.5) - (RESOLUTION_WINDOW_HEIGHT * 0.5);
				int textureOffsetY = distanceFromTop * (textureHeight / spriteHeight);

				
				if (x > 0 && x < RESOLUTION_WINDOW_WIDTH && y > 0 && y < RESOLUTION_WINDOW_HEIGHT)
				{
					color_t* spriteTextureBuffer = (color_t*)upng_get_buffer(textures[sprite.textureArrayIndex]);
					color_t texelColor = spriteTextureBuffer[(textureWidth * textureOffsetY) + textureOffsetX];
					
					if (texelColor != 0xFFFF00FF)
					{
						DrawPixel(x, y, texelColor);
					}
					
				}
			}
		}
		
	}
}
