#include "Map.h"

static const int map[MAP_NUM_ROWS][MAP_NUM_COLS] =
{
	{1, 1, 1, 8, 1, 1, 8, 1, 1, 8, 1, 1, 8 ,1, 1, 1, 1, 1, 1, 1},
	{4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5, 5, 5, 1},
	{4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5, 0, 5, 1},
	{4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5, 0, 5, 1},
	{4, 0, 4, 0, 2, 2, 2, 2, 0, 6, 6, 6, 6, 6, 0, 1, 5, 0, 5, 1},
	{4, 0, 4, 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 1, 5, 0, 5, 1},
	{4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{4, 0, 4, 0, 0, 0, 0, 0, 9, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1},
	{4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 7, 0, 1, 7, 7, 7, 7, 7},
	{4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 7, 0, 1, 7, 0, 0, 0, 7},
	{4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 7, 0, 1, 7, 0, 0, 0, 7},
	{4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 7, 0, 0, 0, 0, 0, 0, 7},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 7, 7, 7, 7, 7, 7, 7}
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

void RenderMap(void)
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