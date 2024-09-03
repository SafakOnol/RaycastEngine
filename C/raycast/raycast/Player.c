#include "Player.h"


player_t player =
{
	.x = MAP_NUM_COLS * TILE_SIZE * 0.5,
	.y = MAP_NUM_ROWS * TILE_SIZE * 0.5,
	.w = 1,
	.h = 1,
	.turnDirection = 0,
	.walkDirection = 0,
	.rotationAngle = PI * 0.5,
	.turnSpeed = 135 * (PI / 180),
	.walkSpeed = 200
};

void MovePlayer(float deltaTime)
{
	player.rotationAngle += player.turnDirection * (player.turnSpeed * deltaTime);

	float moveStep = player.walkDirection * (player.walkSpeed * deltaTime);

	float newX = player.x + cos(player.rotationAngle) * moveStep;
	float newY = player.y + sin(player.rotationAngle) * moveStep;

	if (!CheckWallCollision(newX, newY))
	{
		player.x = newX;
		player.y = newY;
	}
};

void RenderPlayer()
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
