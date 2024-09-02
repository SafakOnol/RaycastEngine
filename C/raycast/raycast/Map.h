#ifndef MAP_H
#define MAP_H

#include <stdbool.h>
#include <stdint.h>

#include "Definitions.h"
#include "Graphics.h"

#define MAP_NUM_ROWS 13
#define MAP_NUM_COLS 20

bool CheckWallCollision(float x, float y);
bool CheckMapBoundary(float x, float y);

void RenderMap(void);

int GetMapAt(int i, int j);



#endif // !MAP_H

