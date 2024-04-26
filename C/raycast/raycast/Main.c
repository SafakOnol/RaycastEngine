#include <stdio.h>
#include <math.h>
#include <limits.h>

#include <SDL2/SDL.h>
#include "Constants.h"

// MAP
const int map[MAP_NUM_ROWS][MAP_NUM_COLS] = 
{
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};


struct Player
{
	float x, y;
	float w, h;
	int turnDirection; // -1 for Left, +1 for Right
	int walkDirection; // -1 for Left, +1 for Right
	float rotationAngle;
	float walkSpeed;
	float turnSpeed;
} player ;

struct Ray
{
	float rayAngle;
	float wallHitX;
	float wallHitY;
	float distance;
	int bIsVerticalHit;
	int bFacingUp;
	int bFacingDown;
	int bFacingRight;
	int bFacingLeft;
	int wallHitContent;
} rays[NUM_RAYS];

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int bIsGameRunning = FALSE;
int ticks = 0;
float deltaTime = 0;

// --- SDL FUNCTIONS --- //

int InitializeWindow()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "Error initializing SDL.\n");
		return FALSE;
	}
	
	window = SDL_CreateWindow
	(
		"Wolf3D",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_BORDERLESS
	);
	if (!window)
	{
		fprintf(stderr, "Error creating SDL window.\n");
		return FALSE;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer)
	{
		fprintf(stderr, "Error creating SDL renderer.\n");
		return FALSE;
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	return TRUE;
}

void DestroyWindow()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

// --- MAIN FUNCTIONS --- //

void GameSetup()
{
	player.x = WINDOW_WIDTH  * 0.5;
	player.y = WINDOW_HEIGHT * 0.5;
	player.w = 1;
	player.h = 1;
	player.turnDirection = 0;
	player.walkDirection = 0;
	player.rotationAngle = PI * 1.5;
	player.turnSpeed = 135 * (PI / 180);
	player.walkSpeed = 100;

}

void ComputeDeltaTime()
{
	// remaining time to target frame in miliseconds
	int timeToDelay = FRAME_TIME_LENGTH - (SDL_GetTicks() - ticks);

	// check if delay is necessary
	if (timeToDelay > 0 && timeToDelay <= FRAME_TIME_LENGTH)
	{
		SDL_Delay(timeToDelay);
	}

	deltaTime = (SDL_GetTicks() - ticks) * 0.001f; // seconds

	// store the miliseconds of the current frame to use in next update
	ticks = SDL_GetTicks();
}

float NormalizeAngle(float angle)
{
	angle = remainder(angle, TWO_PI);
	if (angle < 0)
	{
		angle = TWO_PI + angle;
	}
	return angle;
}

float DistanceBetweenPoints(float x1, float y1, float x2, float y2)
{
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void CastRay(float rayAngle, int stripID)
{
	rayAngle = NormalizeAngle(rayAngle);

	int bIsRayFacingUp		= rayAngle > PI && rayAngle < TWO_PI;
	int bIsRayFacingDown	= !bIsRayFacingUp;
	int bIsRayFacingRight	= rayAngle < PI * 0.5 || rayAngle > PI * 1.5;
	int bIsRayFacingLeft	= !bIsRayFacingRight;

	float xIntercept, yIntercept;
	float xStep, yStep;


	///////////////////////////////////////
	/// HORIZONTAL RAY-GRID INTERSECTION
	///////////////////////////////////////

	int		bHrznWallHit	= FALSE;
	float	hrznWallHitX	= 0;
	float	hrznWallHitY	= 0;
	int		hrznWallContent	= 0;

	// calculate coordinates of nearest horizontal grid intersection
	// y
	yIntercept = floor(player.y / TILE_SIZE) * TILE_SIZE;
	yIntercept += bIsRayFacingDown ? TILE_SIZE : 0;
	// x
	xIntercept = player.x + (yIntercept - player.y) / tan(rayAngle);

	// calculate step increments
	yStep = TILE_SIZE;
	yStep *= bIsRayFacingUp	? -1 : +1;

	xStep = TILE_SIZE / tan(rayAngle);
	xStep *= (bIsRayFacingLeft	&& xStep > 0)	? -1 : +1;
	xStep *= (bIsRayFacingRight && xStep < 0)	? -1 : +1;

	float nextHrznLineX = xIntercept;
	float nextHrznLineY = yIntercept;

	// Increment steps until hitting a wall
	while (nextHrznLineX >= 0 && nextHrznLineX <= WINDOW_WIDTH && nextHrznLineY >= 0 && nextHrznLineY <= WINDOW_HEIGHT)
	{
		float xTemp = nextHrznLineX;
		float yTemp = nextHrznLineY + (bIsRayFacingUp ? -1 : 0);

		if (CheckCollision(xTemp, yTemp))
		{
			// ray hit a wall!
			hrznWallHitX = nextHrznLineX;
			hrznWallHitY = nextHrznLineY;
			hrznWallContent = map[(int)floor(yTemp / TILE_SIZE)][(int)floor(xTemp / TILE_SIZE)];
			bHrznWallHit = TRUE;
			break;
		}
		else
		{
			nextHrznLineX += xStep;
			nextHrznLineY += yStep;
		}
	}

	///////////////////////////////////////
	/// VERTICAL RAY-GRID INTERSECTION
	///////////////////////////////////////

	int		bVertWallHit	= FALSE;
	float	vertWallHitX	= 0;
	float	vertWallHitY	= 0;
	int		vertWallContent = 0;

	// calculate coordinates of nearest horizontal grid intersection
	// x
	xIntercept = floor(player.x / TILE_SIZE) * TILE_SIZE;
	xIntercept += bIsRayFacingRight ? TILE_SIZE : 0;
	// y
	yIntercept = player.y + (xIntercept - player.x) / tan(rayAngle);

	// calculate step increments
	xStep = TILE_SIZE;
	xStep *= bIsRayFacingLeft ? -1 : +1;

	yStep = TILE_SIZE / tan(rayAngle);
	yStep *= (bIsRayFacingUp	&& yStep > 0) ? -1 : +1;
	yStep *= (bIsRayFacingDown	&& yStep < 0) ? -1 : +1;

	float nextVertLineX = xIntercept;
	float nextVertLineY = yIntercept;

	// Increment steps until hitting a wall
	while (nextVertLineX >= 0 && nextVertLineX <= WINDOW_WIDTH && nextVertLineY >= 0 && nextVertLineY <= WINDOW_HEIGHT)
	{
		float xTemp = nextVertLineX + (bIsRayFacingLeft ? -1 : 0);
		float yTemp = nextVertLineY;

		if (CheckCollision(xTemp, yTemp))
		{
			// ray hit a wall!
			vertWallHitX = nextVertLineX;
			vertWallHitY = nextVertLineY;
			vertWallContent = map[(int)floor(yTemp / TILE_SIZE)][(int)floor(xTemp / TILE_SIZE)];
			bVertWallHit = TRUE;
			break;
		}
		else
		{
			nextVertLineX += xStep;
			nextVertLineY += yStep;
		}
	}

	// CALCULATE THE NEAREST HIT
	float hrznHitDistance = bHrznWallHit
		? DistanceBetweenPoints(player.x, player.y, hrznWallHitX, hrznWallHitY)
		: INT_MAX;

	float vertHitDistance = bVertWallHit
		? DistanceBetweenPoints(player.x, player.y, vertWallHitX, vertWallHitY)
		: INT_MAX;


	if (vertHitDistance < hrznHitDistance)
	{
		rays[stripID].distance			= vertHitDistance;
		rays[stripID].wallHitX			= vertWallHitX;
		rays[stripID].wallHitY			= vertWallHitY;
		rays[stripID].wallHitContent	= vertWallContent;
		rays[stripID].bIsVerticalHit	= TRUE;
	}
	else
	{
		rays[stripID].distance			= hrznHitDistance;
		rays[stripID].wallHitX			= hrznWallHitX;
		rays[stripID].wallHitY			= hrznWallHitY;
		rays[stripID].wallHitContent	= hrznWallContent;
		rays[stripID].bIsVerticalHit	= FALSE;
	}

	rays[stripID].rayAngle		= rayAngle;
	rays[stripID].bFacingUp		= bIsRayFacingUp;
	rays[stripID].bFacingDown	= bIsRayFacingDown;
	rays[stripID].bFacingRight	= bIsRayFacingRight;
	rays[stripID].bFacingLeft	= bIsRayFacingLeft;
}

void CastAllRays()
{
	// start first ray subtracting half of our FOV
	float rayAngle = player.rotationAngle - (FOV_ANGLE * 0.5);

	for (int stripID = 0; stripID < NUM_RAYS; stripID++)
	{
		CastRay(rayAngle, stripID);
		rayAngle += FOV_ANGLE / NUM_RAYS;
	}
}

void RenderMap()
{
	for (int i = 0; i < MAP_NUM_ROWS; i++)
	{
		for (int j = 0; j < MAP_NUM_COLS; j++)
		{
			int tileX = j * TILE_SIZE;
			int tileY = i * TILE_SIZE;
			int tileColorR = map[i][j] != 0 ? 26  : 79;
			int tileColorG = map[i][j] != 0 ? 8   : 157;
			int tileColorB = map[i][j] != 0 ? 65  : 166;

			SDL_SetRenderDrawColor(renderer, tileColorR, tileColorG, tileColorB, 255);
			SDL_Rect mapTileRect = {
				tileX * MINIMAP_SCALE_FACTOR,
				tileY * MINIMAP_SCALE_FACTOR,
				TILE_SIZE * MINIMAP_SCALE_FACTOR,
				TILE_SIZE * MINIMAP_SCALE_FACTOR
			};
			SDL_RenderFillRect(renderer, &mapTileRect);
		}
	}
}


int CheckCollision(float x, float y)
{
	if (x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT)
	{
		//printf("Player position out of bounds: (%f, %f)\n", x, y);
		return TRUE; // return collision
	}

	int mapGridIndexX = (int)floor(x / TILE_SIZE);
	int mapGridIndexY = (int)floor(y / TILE_SIZE);

	printf("Player position: (%f, %f), Map Grid Index: (%d, %d)\n", x, y, mapGridIndexX, mapGridIndexY);

	return map[mapGridIndexY][mapGridIndexX] != 0; // x and y, columns and rows inverted
}

void MovePlayer(float deltaTime)
{
	player.rotationAngle += player.turnDirection * (player.turnSpeed * deltaTime);

	float moveStep = player.walkDirection * (player.walkSpeed * deltaTime);

	float newX = player.x + cos(player.rotationAngle) * moveStep;
	float newY = player.y + sin(player.rotationAngle) * moveStep;

	if (!CheckCollision(newX, newY))
	{
		player.x = newX;
		player.y = newY;
	}		
}


void RenderPlayer()
{
	SDL_SetRenderDrawColor(renderer, 255, 173, 90, 255);
	SDL_Rect playerRect =
	{
		player.x * MINIMAP_SCALE_FACTOR,
		player.y * MINIMAP_SCALE_FACTOR,
		player.w * MINIMAP_SCALE_FACTOR,
		player.h * MINIMAP_SCALE_FACTOR
	};
	SDL_RenderFillRect(renderer, &playerRect);

	SDL_RenderDrawLine
	(
		renderer,
		MINIMAP_SCALE_FACTOR * player.x,
		MINIMAP_SCALE_FACTOR * player.y,
		MINIMAP_SCALE_FACTOR * player.x + cos(player.rotationAngle) * 40,
		MINIMAP_SCALE_FACTOR * player.y + sin(player.rotationAngle) * 40
	);
}

// --- GAME LOOP FUNCTIONS --- //

void HandleInput()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
		case SDL_QUIT:
		{
			bIsGameRunning = FALSE;
			break;
		}
		case SDL_KEYDOWN:
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
				bIsGameRunning = FALSE;
			if (event.key.keysym.sym == SDLK_UP)
				player.walkDirection = +1;
			if (event.key.keysym.sym == SDLK_DOWN)
				player.walkDirection = -1;
			if (event.key.keysym.sym == SDLK_RIGHT)
				player.turnDirection = +1;
			if (event.key.keysym.sym == SDLK_LEFT)
				player.turnDirection = -1;
			break;
		}
		case SDL_KEYUP:
		{
			if (event.key.keysym.sym == SDLK_UP)
				player.walkDirection = 0;
			if (event.key.keysym.sym == SDLK_DOWN)
				player.walkDirection = 0;
			if (event.key.keysym.sym == SDLK_RIGHT)
				player.turnDirection = 0;
			if (event.key.keysym.sym == SDLK_LEFT)
				player.turnDirection = 0;
			break;
		}
	}
}

void Update()
{
	// Start with this
	ComputeDeltaTime();

	MovePlayer(deltaTime);
	CastAllRays();
}

void Render()
{
	SDL_SetRenderDrawColor(renderer, 255, 89, 89, 255); // set color
	SDL_RenderClear(renderer); // clear buffer
	
	RenderMap();
	// RenderRays
	RenderPlayer();


	SDL_RenderPresent(renderer); // swap buffer
}

int main(int argc, char* args[])
{
	bIsGameRunning = InitializeWindow();

	GameSetup();

	while (bIsGameRunning)
	{
		HandleInput();
		Update();
		Render();
	}

	DestroyWindow();

	return 0;
}