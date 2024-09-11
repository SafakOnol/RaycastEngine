#include "Map.h"

static const int map[MAP_NUM_ROWS][MAP_NUM_COLS] =
{
	{ 41, 41, 32, 41, 41, 57, 30, 57, 57, 57, 57, 57, 55},
	{ 41,  0,  0,  0, 41,  0,  0,  0,  0,  0,  0,  0, 56},
	{ 41,  0,  0,  0, 41,  0,  0,  0,  0, 25,  0, 26, 57},
	{ 41,  0,  0,  0, 41,  0,  0,  0,  0,  0,  0,  0, 58},
	{ 41, 41,  0, 41, 41,  0,  0,  0,  0, 25,  0, 26, 59},
	{ 60,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 60},
	{ 55,  0,  0,  0,  0,  0,  0,  0,  0, 25,  0, 26, 55},
	{ 56,  0,  0, 55, 55, 55,  0,  0,  0,  0,  0,  0, 56},
	{ 57,  0,  0, 55, 23, 55,  0,  0,  0,  0,  0,  0, 57},
	{ 58,  0,  0, 55,  0, 55,  0,  0,  0,  0,  0,  0, 58},
	{ 59,  0,  0, 55,  0, 55,  0,  0,  0,  0,  0,  0, 59},
	{ 60,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 60},
	{ 55,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 55},
	{ 56,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 56},
	{ 57, 55, 57, 56, 57, 57, 58, 57, 56, 57, 60, 57, 55}
};

bool CheckWallCollision(float x, float y)
{
	if (x < 0 || x >= MAP_NUM_COLS * TILE_SIZE || y < 0 || y >= MAP_NUM_ROWS * TILE_SIZE)
	{
		//printf("Player position out of bounds: (%f, %f)\n", x, y);
		return true; // return collision
	}

	int mapGridIndexX = (int)floor(x / TILE_SIZE);
	int mapGridIndexY = (int)floor(y / TILE_SIZE);

	//printf("Player position: (%f, %f), Map Grid Index: (%d, %d)\n", x, y, mapGridIndexX, mapGridIndexY);

	return map[mapGridIndexY][mapGridIndexX] != 0; // x and y, columns and rows inverted
}
bool CheckMapBoundary(float x, float y)
{
	return (x >= 0 && x <= MAP_NUM_COLS * TILE_SIZE && y >= 0 && y <= MAP_NUM_ROWS * TILE_SIZE);
};

int GetMapAt(int i, int j)
{
	return map[i][j];
};

void RenderMiniMapGrid(void)
{
	for (int i = 0; i < MAP_NUM_ROWS; i++)
	{
		for (int j = 0; j < MAP_NUM_COLS; j++)
		{
			int tileX = j * TILE_SIZE;
			int tileY = i * TILE_SIZE;
			/*int tileColorR = map[i][j] != 0 ? 255 : 0;
			int tileColorG = map[i][j] != 0 ? 255 : 0;
			int tileColorB = map[i][j] != 0 ? 255 : 0;
			uint32_t tileColor = (255 << 24) | (tileColorR << 16) | (tileColorG << 8) | tileColorB;*/
			color_t tileColor = map[i][j] != 0 ? 0xFFFFFFFF : 0x00000000;

			DrawRect
			(
				tileX * MINIMAP_SCALE_FACTOR,
				tileY * MINIMAP_SCALE_FACTOR,
				TILE_SIZE * MINIMAP_SCALE_FACTOR,
				TILE_SIZE * MINIMAP_SCALE_FACTOR,
				tileColor
			);
		}
	}
};