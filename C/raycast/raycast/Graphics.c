#include "Graphics.h"

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static uint32_t* colorBuffer = NULL;
static SDL_Texture* colorBufferTexture;

bool InitializeWindow(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "Error initializing SDL.\n");
		return false;
	}

	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);
	int fullscreenWidth = display_mode.w;
	int fullscreenHeight = display_mode.h;

	window = SDL_CreateWindow
	(
		"Wolf3D",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		fullscreenWidth,
		fullscreenHeight,
		SDL_WINDOW_BORDERLESS
	);
	if (!window)
	{
		fprintf(stderr, "Error creating SDL window.\n");
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer)
	{
		fprintf(stderr, "Error creating SDL renderer.\n");
		return false;
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// allocate memory for color buffer
	colorBuffer = (uint32_t*)malloc(sizeof(uint32_t) * (uint32_t)RESOLUTION_WINDOW_WIDTH * (uint32_t)RESOLUTION_WINDOW_HEIGHT); // size of uint32_t * w & h in uint32_t

	colorBufferTexture = SDL_CreateTexture
	(
		renderer,
		SDL_PIXELFORMAT_RGBA32,
		SDL_TEXTUREACCESS_STREAMING,
		RESOLUTION_WINDOW_WIDTH,
		RESOLUTION_WINDOW_HEIGHT
	);

	return true;
}


void DestroyWindow(void)
{
	free(colorBuffer);
	SDL_DestroyTexture(colorBufferTexture);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


void ClearColorBuffer(uint32_t color)
{
	for (int i = 0; i < RESOLUTION_WINDOW_WIDTH * RESOLUTION_WINDOW_HEIGHT; i++)
	{
		colorBuffer[i] = color;
	}
}


void RenderColorBuffer(void)
{
	SDL_UpdateTexture
	(
		colorBufferTexture,
		NULL,
		colorBuffer,
		(int)((uint32_t)RESOLUTION_WINDOW_WIDTH * sizeof(uint32_t))
	);

	SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL);

	SDL_RenderPresent(renderer); // swap buffer
}

void DrawPixel(int x, int y, uint32_t color)
{
	colorBuffer[(RESOLUTION_WINDOW_WIDTH * y) + x] = color;
}

