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
	.turnSpeed			= 135 * (PI / 180),
	.walkSpeed			= 200
};

void MovePlayer(float deltaTime)
{
	player.rotationAngle += player.turnDirection * (player.turnSpeed * deltaTime);
	NormalizeAngle(&player.rotationAngle);

	float moveStep = player.walkDirection * (player.walkSpeed * deltaTime);
	float strafeStep = player.strafeDirection * (player.walkSpeed * deltaTime);

	float newX = player.x + cos(player.rotationAngle) * moveStep + cos(player.rotationAngle + PI/2) * strafeStep;
	float newY = player.y + sin(player.rotationAngle) * moveStep + sin(player.rotationAngle + PI/2) * strafeStep;

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
