#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "Definitions.h"
#include "Texture.h"
#include "Graphics.h"
#include "Map.h"
#include "Ray.h"
#include "Player.h"
#include "Wall.h"
#include "Sprite.h"

// --- GLOBAL VARIABLES --- //

bool bIsGameRunning = false;
int ticks = 0;
float deltaTime = 0;

// --- HELPER FUNCTIONS --- //

void ComputeDeltaTime() // cricital!
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

// --- MAIN FUNCTIONS --- //

void GameSetup()
{
	LoadTextures();
}

/// <Color Palet>
/// #63345E 
/// #AC61B9 
/// #B7C1DE 
/// #0B468C 
/// #092047 
/// </Color Palet>

void RenderMiniMap(void)
{
	RenderMiniMapGrid();
	RenderMiniMapRays();
	RenderMiniMapPlayer();
	RenderMiniMapSprites();
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
			bIsGameRunning = false;
			break;
		}
		case SDL_KEYDOWN:
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
				bIsGameRunning = false;
			if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
				player.walkDirection = +1;
			if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
				player.walkDirection = -1;
			if (event.key.keysym.sym == SDLK_RIGHT)
				player.turnDirection = +1;
			if (event.key.keysym.sym == SDLK_LEFT)
				player.turnDirection = -1;
			if (event.key.keysym.sym == SDLK_d)  // Strafe right
				player.strafeDirection = +1;
			if (event.key.keysym.sym == SDLK_a)  // Strafe left
				player.strafeDirection = -1;
			break;
		}
		case SDL_KEYUP:
		{
			if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
				player.walkDirection = 0;
			if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
				player.walkDirection = 0;
			if (event.key.keysym.sym == SDLK_RIGHT)
				player.turnDirection = 0;
			if (event.key.keysym.sym == SDLK_LEFT)
				player.turnDirection = 0;
			if (event.key.keysym.sym == SDLK_d)
				player.strafeDirection = 0;
			if (event.key.keysym.sym == SDLK_a)
				player.strafeDirection = 0;
			break;
		}
	}
}

void Update()
{
	// Start with compute delta time
	ComputeDeltaTime();

	MovePlayer(deltaTime);
	CastAllRays();
}

void Render()
{
	ClearColorBuffer(0xFF000000);

	RenderWallProjection();
	RenderSpriteProjection();
	RenderMiniMap();
	RenderColorBuffer();
}

void ReleaseResources(void)
{
	FreeTextures();
	DestroyWindow();
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
		
	ReleaseResources();

	return EXIT_SUCCESS;
}