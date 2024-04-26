#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

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

	deltaTime = (SDL_GetTicks() - ticks) / 1000.f; // seconds

	// store the miliseconds of the current frame to use in next update
	ticks = SDL_GetTicks();
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