#include "Player.h"


player_t player =
{
	.x					= MAP_NUM_COLS * TILE_SIZE * 0.5,
	.y					= MAP_NUM_ROWS * TILE_SIZE * 0.85,
	.w					= 1,
	.h					= 1,
	.turnDirection		= 0,
	.walkDirection		= 0,
	.strafeDirection	= 0,
	.rotationAngle		= PI * 1.5,
	.turnSpeed			= 90 * (PI / 180),
	.walkSpeed			= 200
};

void MovePlayer(float deltaTime)
{
	player.rotationAngle += player.turnDirection * (player.turnSpeed * deltaTime);
	NormalizeAngle(&player.rotationAngle);

	float forwardStep	= player.walkDirection * (player.walkSpeed * deltaTime);
	float sideStep		= player.strafeDirection * (player.walkSpeed * deltaTime);

	// Calculate movement vector
	float moveAngle = player.rotationAngle;
	float strafeAngle = player.rotationAngle + PI / 2;

	float deltaX = cos(moveAngle) * forwardStep + cos(strafeAngle) * sideStep;
	float deltaY = sin(moveAngle) * forwardStep + sin(strafeAngle) * sideStep;

	// Normalize movement if moving diagonally
	if (player.walkDirection != 0 && player.strafeDirection != 0)
	{
		float length = sqrt(deltaX * deltaX + deltaY * deltaY);
		if (length > 0)
		{
			float normalizedLength = player.walkSpeed * deltaTime / length;
			deltaX *= normalizedLength;
			deltaY *= normalizedLength;
		}
	}

	float newX = player.x + deltaX;
	float newY = player.y + deltaY;

	if (!CheckWallCollision(newX, newY))
	{
		player.x = newX;
		player.y = newY;
	}
};

void RenderMiniMapPlayer()
{
	DrawRect
	(
		player.x * MINIMAP_SCALE_FACTOR,
		player.y * MINIMAP_SCALE_FACTOR,
		player.w * MINIMAP_SCALE_FACTOR,
		player.h * MINIMAP_SCALE_FACTOR,
		0xFFFFFFFF
	);


	/*SDL_RenderDrawLine
	(
		renderer,
		MINIMAP_SCALE_FACTOR * player.x,
		MINIMAP_SCALE_FACTOR * player.y,
		MINIMAP_SCALE_FACTOR * player.x + cos(player.rotationAngle) * 10,
		MINIMAP_SCALE_FACTOR * player.y + sin(player.rotationAngle) * 10
	);*/
};
