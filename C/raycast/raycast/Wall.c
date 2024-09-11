#include "Wall.h"

void RenderWallProjection(void)
{
	for (int x = 0; x < NUM_RAYS; x++)
	{
		// Calculate the perpendicular distance to avoid fish-eye distortion
		float perpendicularDistance = rays[x].distance * (float)cos(rays[x].rayAngle - player.rotationAngle);

		// Projected wall height
		float wallHeight	= (TILE_SIZE / perpendicularDistance) * DISTANCE_TO_PROJECTION_PLANE;

		// calculate Wall Dimentions to draw
		int wallTopY			= (RESOLUTION_WINDOW_HEIGHT * 0.5) - (wallHeight * 0.5);
		wallTopY				= wallTopY < 0 ? 0 : wallTopY; // clamp top pixel to top of the screen
		int wallBottomY			= (RESOLUTION_WINDOW_HEIGHT * 0.5) + (wallHeight * 0.5);
		wallBottomY				= wallBottomY > RESOLUTION_WINDOW_HEIGHT ? RESOLUTION_WINDOW_HEIGHT : wallBottomY;

		// render ceiling
		for (int y = 0; y < wallTopY; y++)
		{
			// colorBuffer[(RESOLUTION_WINDOW_WIDTH * c) + x] = 0x403C3C;
			DrawPixel(x, y, 0x403C3C);
		}

		// calculate textureOffsetX
		// TODO: add link to math drawing
		int textureOffsetX;
		if (rays[x].bIsAVerticalHit)
		{
			textureOffsetX		= (int)rays[x].wallHitY % TILE_SIZE;
		}
		else
		{
			textureOffsetX		= (int)rays[x].wallHitX % TILE_SIZE;
		}

		int textureArrayIndex	= rays[x].textureArrayIndex - 1; // 0 is used for empty space, therefore map content and texture array has 1 difference

		// Query texture width and height from upng
		int texture_width		= upng_get_width(textures[textureArrayIndex]);
		int texture_height		= upng_get_height(textures[textureArrayIndex]);

		// render the wall from top y to bottom y
		for (int y = wallTopY; y < wallBottomY; y++)
		{
			int distanceToWallTop		= y + (wallHeight * 0.5) - (RESOLUTION_WINDOW_HEIGHT * 0.5);
			int textureOffsetY			= distanceToWallTop * ((float)texture_height / wallHeight); // set first offset pixel to the first pixel of the wall

			// set the color of the wall from texture file using upng buffer
			color_t* wallTextureBuffer	= (color_t*)upng_get_buffer(textures[textureArrayIndex]);
			color_t texelColor			= wallTextureBuffer[(texture_width * textureOffsetY) + textureOffsetX];

			// shade the farther walls
			float colorIntensity = MIN((DISTANCE_TO_PROJECTION_PLANE / (wallColorFadeFactor * rays[x].distance)), 1);
			AdjustColorIntensity(&texelColor, colorIntensity);

			// Shade the horizontal placed walls
			float intensityModifier = rays[x].bIsAVerticalHit ? 1 : 0.8;
			AdjustColorIntensity(&texelColor, intensityModifier);

			DrawPixel(x, y, texelColor);
		}

		// render floor
		for (int y = wallBottomY; y < RESOLUTION_WINDOW_HEIGHT; y++)
		{
			DrawPixel(x, y, 0x706C6C);
		}
	}

}