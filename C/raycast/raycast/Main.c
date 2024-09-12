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
	const Uint8* keystate = SDL_GetKeyboardState(NULL);

	int walkDirection = 0;
	int turnDirection = 0;
	int strafeDirection = 0;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			bIsGameRunning = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				bIsGameRunning = false;
			break;
		}
	}

	// Turn direction handling 
	if (keystate[SDL_SCANCODE_LEFT])
		turnDirection = -1;
	if (keystate[SDL_SCANCODE_RIGHT])
		turnDirection = 1;

	// Walk direction handling
    if (keystate[SDL_SCANCODE_W])
        walkDirection =  1;
    if (keystate[SDL_SCANCODE_S])
        walkDirection = -1;

    // Strafe direction handling
    if (keystate[SDL_SCANCODE_A])
        strafeDirection = -1;
    if (keystate[SDL_SCANCODE_D])
        strafeDirection =  1;

    // Update player directions after all key state checks
    player.walkDirection	= walkDirection;
    player.strafeDirection	= strafeDirection;
    player.turnDirection	= turnDirection;

    // Debug output for movement states
    //printf("Walk: %d, Strafe: %d, Turn: %d\n", player.walkDirection, player.strafeDirection, player.turnDirection);

    // Flush key events to clear the event queue after processing inputs
    SDL_FlushEvent(SDL_KEYDOWN);
    SDL_FlushEvent(SDL_KEYUP);
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