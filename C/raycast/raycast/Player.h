#ifndef PLAYER_H
#define PLAYER_H

#include "Definitions.h"
#include "Map.h" // TODO : Find a way to adjust player start location without including map in player. 
#include "Graphics.h"

typedef struct
{
	float x, y;
	float w, h;
	int turnDirection; // -1 for Left, +1 for Right
	int walkDirection; // -1 for Left, +1 for Right
	float rotationAngle;
	float walkSpeed;
	float turnSpeed;
} player_t;

extern player_t player;

void MovePlayer(float deltaTime);

void RenderPlayer(void);

#endif