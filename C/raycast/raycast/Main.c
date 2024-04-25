#include <stdio.h>
#include <SDL2/SDL.h>
#include "Constants.h"


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int bIsGameRunning = FALSE;

float playerX, playerY;

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

void Setup()
{
	// TODO:
	// Initialize game objects
	playerX = WINDOW_WIDTH * 0.5;
	playerY = WINDOW_HEIGHT * 0.5;
}

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
			{
				bIsGameRunning = FALSE;
				break;
			}
		}
	}
}

void Update()
{
	playerX += 0.0001f;
}

void Render()
{
	SDL_SetRenderDrawColor(renderer, 35, 30, 35, 255); // set color
	SDL_RenderClear(renderer); // clear buffer
	
	// draw player
	SDL_SetRenderDrawColor(renderer, 222, 238, 234, 255);
	SDL_Rect pRect = { playerX, playerY, 20, 20 };
	SDL_RenderFillRect(renderer, &pRect);

	SDL_RenderPresent(renderer); // swap buffer
}

int main(int argc, char* args[])
{
	bIsGameRunning = InitializeWindow();

	Setup();

	while (bIsGameRunning)
	{
		HandleInput();
		Update();
		Render();
	}

	DestroyWindow();

	return 0;
}