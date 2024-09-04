#include "Graphics.h"

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static color_t* colorBuffer = NULL;
static SDL_Texture* colorBufferTexture;

wallColorFadeFactor = 2.0f;

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
	colorBuffer = (color_t*)malloc(sizeof(color_t) * (color_t)RESOLUTION_WINDOW_WIDTH * (color_t)RESOLUTION_WINDOW_HEIGHT); // size of uint32_t * w & h in uint32_t

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


void ClearColorBuffer(color_t color)
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
		(int)((color_t)RESOLUTION_WINDOW_WIDTH * sizeof(color_t))
	);

	SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL);

	SDL_RenderPresent(renderer); // swap buffer
}

void AdjustColorIntensity(color_t* color, float factor)
{
	color_t A = (*color & 0xFF000000);
	color_t R = (*color & 0x00FF0000) * factor;
	color_t G = (*color & 0x0000FF00) * factor;
	color_t B = (*color & 0x000000FF) * factor;
	
	*color = A | (R & 0x00FF0000) | (G & 0x0000FF00) | (B & 0x000000FF);
}

void DrawPixel(int x, int y, color_t color)
{
	colorBuffer[(RESOLUTION_WINDOW_WIDTH * y) + x] = color;
}

void DrawRect(int x, int y, int width, int height, color_t color)
{
	for (int i = x; i < x + width + 1; i++)
	{
		for (int j = y; j < y + height + 1; j++)
		{
			DrawPixel(i, j, color);
		}

	}
}

void DrawLine(int x0, int y0, int x1, int y1, color_t color) // DDA algorithm
{
	// find rise(delta_y) and run(delta_x) values
	int deltaX = (x1 - x0);
	int deltaY = (y1 - y0);

	// get absolute value of the rise and run, compare the sizes and chose the longer one as side length (base to calculate increment values)
	int baseLength = abs(deltaX) >= abs(deltaY) ? abs(deltaX) : abs(deltaY);

	// calculate increment values (cast one of the values to float in order to get a floating point as a result of the division
	float xIncrement = deltaX / (float)baseLength;
	float yIncrement = deltaY / (float)baseLength;

	// setup starting values
	float currentX = x0;
	float currentY = y0;

	// calculate and draw pixels along the line step by step
	for (int i = 0; i < baseLength; i++)
	{
		DrawPixel(round(currentX), round(currentY), color);
		currentX += xIncrement;
		currentY += yIncrement;
	}
};

