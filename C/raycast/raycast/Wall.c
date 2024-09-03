#include "Wall.h"

void RenderWallProjection(void)
{
	for (int x = 0; x < NUM_RAYS; x++)
	{
		// ray math
		float perpendicularDistance = rays[x].distance * (float)cos(rays[x].rayAngle - player.rotationAngle);

		float projectedWallHeight = (TILE_SIZE / perpendicularDistance) * DISTANCE_TO_PROJECTION_PLANE;

		// calculate Wall Dimentions to draw
		int wallStripHeight = (int)projectedWallHeight;

		int wallTopPixel = (RESOLUTION_WINDOW_HEIGHT * 0.5) - (wallStripHeight * 0.5);
		wallTopPixel = wallTopPixel < 0 ? 0 : wallTopPixel; // clamp top pixel to top of the screen

		int wallBottomPixel = (RESOLUTION_WINDOW_HEIGHT * 0.5) + (wallStripHeight * 0.5);
		wallBottomPixel = wallBottomPixel > RESOLUTION_WINDOW_HEIGHT ? RESOLUTION_WINDOW_HEIGHT : wallBottomPixel;

		// render ceiling
		for (int y = 0; y < wallTopPixel; y++)
		{
			// colorBuffer[(RESOLUTION_WINDOW_WIDTH * c) + x] = 0x403C3C;
			DrawPixel(x, y, 0x403C3C);
		}

		// calculate textureOffsetX
		// TODO: add link to math drawing
		int textureOffsetX;
		if (rays[x].bIsAVerticalHit)
		{
			textureOffsetX = (int)rays[x].wallHitY % TILE_SIZE;
		}
		else
		{
			textureOffsetX = (int)rays[x].wallHitX % TILE_SIZE;
		}

		int textureArrayIndex = rays[x].wallHitContent - 1; // 0 is used for empty space, therefore map content and texture array has 1 difference

		int texture_width = WallTextures[textureArrayIndex].width;
		int texture_height = WallTextures[textureArrayIndex].height;

		// render the wall from top to bottom
		for (int y = wallTopPixel; y < wallBottomPixel; y++)
		{
			int distanceToWallTop = y + (wallStripHeight * 0.5) - (RESOLUTION_WINDOW_HEIGHT * 0.5);
			int textureOffsetY = distanceToWallTop * ((float)texture_height / wallStripHeight); // set first offset pixel to the first pixel of the wall

			color_t texelColor = WallTextures[textureArrayIndex].texture_buffer[(texture_width * textureOffsetY) + textureOffsetX];
			DrawPixel(x, y, texelColor);
		}

		// render floor
		for (int y = wallBottomPixel; y < RESOLUTION_WINDOW_HEIGHT; y++)
		{
			DrawPixel(x, y, 0x706C6C);
		}
	}

}