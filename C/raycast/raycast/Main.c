#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <float.h>

#include <SDL2/SDL.h>

#include "Definitions.h"
#include "Textures.h"
#include "Graphics.h"
#include "Map.h"
#include "Ray.h"
#include "Player.h"

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
	// Ask upng library to decode the textures
	LoadWallTextures();
}







/// <Color Palet>
/// #63345E 
/// #AC61B9 
/// #B7C1DE 
/// #0B468C 
/// #092047 
/// </Color Palet>

void RenderWallProjection()
{
	for (int x = 0; x < NUM_RAYS; x++)
	{
		// ray math
		float perpendicularDistance = rays[x].distance * (float)cos(rays[x].rayAngle - player.rotationAngle);

		float projectedWallHeight = (TILE_SIZE / perpendicularDistance) * DISTANCE_TO_PROJECTION_PLANE;

		// calculate Wall Dimentions to draw
		int wallStripHeight = (int)projectedWallHeight;

		int wallTopPixel = (RESOLUTION_WINDOW_HEIGHT * 0.5) - (wallStripHeight * 0.5);
		wallTopPixel = wallTopPixel < 0 ? 0 : wallTopPixel; // clamp top pixel to top of the screen

		int wallBottomPixel = (RESOLUTION_WINDOW_HEIGHT * 0.5) + (wallStripHeight * 0.5);
		wallBottomPixel = wallBottomPixel > RESOLUTION_WINDOW_HEIGHT ? RESOLUTION_WINDOW_HEIGHT : wallBottomPixel;

		// render ceiling
		for (int y = 0; y < wallTopPixel; y++)
		{
			// colorBuffer[(RESOLUTION_WINDOW_WIDTH * c) + x] = 0x403C3C;
			DrawPixel(x, y, 0x403C3C);
		}

		// calculate textureOffsetX
		// TODO: add link to math drawing
		int textureOffsetX;
		if (rays[x].bIsAVerticalHit)
		{
			textureOffsetX = (int)rays[x].wallHitY % TILE_SIZE;
		}
		else
		{
			textureOffsetX = (int)rays[x].wallHitX % TILE_SIZE;
		}

		int textureArrayIndex = rays[x].wallHitContent - 1; // 0 is used for empty space, therefore map content and texture array has 1 difference

		int texture_width	= WallTextures[textureArrayIndex].width;
		int texture_height	= WallTextures[textureArrayIndex].height;

		// render the wall from top to bottom
		for (int y = wallTopPixel; y < wallBottomPixel; y++)
		{
			int distanceToWallTop = y + (wallStripHeight * 0.5) - (RESOLUTION_WINDOW_HEIGHT * 0.5);
			int textureOffsetY = distanceToWallTop * ((float)texture_height / wallStripHeight); // set first offset pixel to the first pixel of the wall
			
			uint32_t texelColor = WallTextures[textureArrayIndex].texture_buffer[(texture_width * textureOffsetY) + textureOffsetX];
			DrawPixel(x, y, texelColor);
		}
		
		// render floor
		for (int y = wallBottomPixel; y < RESOLUTION_WINDOW_HEIGHT; y++)
		{
			DrawPixel(x, y, 0x706C6C);
		}
	}

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
	// Start with compute delta time
	ComputeDeltaTime();

	MovePlayer(deltaTime);
	CastAllRays();
}

void Render()
{
	ClearColorBuffer(0xFF000000);

	RenderWallProjection();

	// Test Rectangle
	//DrawRect(100, 100, 100, 100, 0xFFFFFFFF);
	
	// Display Minimap
	RenderMap();
	//RenderRays();
	RenderPlayer();

	RenderColorBuffer();
}

void ReleaseResources(void)
{
	FreeWallTextures();
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

	return 0;
}