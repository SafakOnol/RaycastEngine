#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <float.h>

#include <SDL2/SDL.h>

#include "Constants.h"
#include "Textures.h"


// MAP
const int map[MAP_NUM_ROWS][MAP_NUM_COLS] = 
{
	{1, 1, 1, 8, 1, 1, 8, 1, 1, 8, 1, 1, 8 ,1, 1, 1, 1, 1, 1, 1},
	{4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5, 5, 5, 1},
	{4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5, 0, 5, 1},
	{4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5, 0, 5, 1},
	{4, 0, 4, 0, 2, 2, 2, 2, 0, 6, 6, 6, 6, 6, 0, 1, 5, 0, 5, 1},
	{4, 0, 4, 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 1, 5, 0, 5, 1},
	{4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{4, 0, 4, 0, 0, 0, 0, 0, 9, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1},
	{4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 7, 0, 1, 7, 7, 7, 7, 7},
	{4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 7, 0, 1, 7, 0, 0, 0, 7},
	{4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 7, 0, 1, 7, 0, 0, 0, 7},
	{4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 7, 0, 0, 0, 0, 0, 0, 7},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 7, 7, 7, 7, 7, 7, 7}
};

// --- WRAPPERS --- //

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
	bool bIsAVerticalHit;
	int wallHitContent;
} rays[NUM_RAYS];

// --- GLOBAL VARIABLES --- //

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
bool bIsGameRunning = false;
int ticks = 0;
float deltaTime = 0;

uint32_t* colorBuffer = NULL;
SDL_Texture* colorBufferTexture;


// --- SDL FUNCTIONS --- //

bool InitializeWindow()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "Error initializing SDL.\n");
		return false;
	}
	
	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);
	int fullscreenWidth		= display_mode.w;
	int fullscreenHeight	= display_mode.h;

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
	return true;
}

void DestroyWindow()
{
	FreeWallTextures();
	free(colorBuffer);
	SDL_DestroyTexture(colorBufferTexture);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

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

// --- MAIN FUNCTIONS --- //

void GameSetup()
{
	player.x = MAP_NUM_COLS * TILE_SIZE * 0.5;
	player.y = MAP_NUM_ROWS * TILE_SIZE * 0.5;
	player.w = 1;
	player.h = 1;
	player.turnDirection = 0;
	player.walkDirection = 0;
	player.rotationAngle = PI * 0.5;
	player.turnSpeed = 135 * (PI / 180);
	player.walkSpeed = 200;

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

	// create texture with a blue-black square pattern
	/*wallTexture = (uint32_t*) malloc(sizeof(uint32_t) * (uint32_t)TEXTURE_WIDTH * (uint32_t)TEXTURE_HEIGHT);
	for (int x = 0; x < TEXTURE_WIDTH; x++)
	{
		for (int y = 0; y < TEXTURE_HEIGHT; y++)
		{
			wallTexture[(TEXTURE_WIDTH * y) + x] = (x % 8 && y % 8) ? 0xFF0000FF : 0xFF000000;
		}
	}*/

	// Ask upng library to decode the textures
	LoadWallTextures();
}

bool CheckCollision(float x, float y)
{
	if (x < 0 || x >= MAP_NUM_COLS * TILE_SIZE || y < 0 || y >= MAP_NUM_ROWS * TILE_SIZE)
	{
		//printf("Player position out of bounds: (%f, %f)\n", x, y);
		return true; // return collision
	}

	int mapGridIndexX = (int)floor(x / TILE_SIZE);
	int mapGridIndexY = (int)floor(y / TILE_SIZE);

	//printf("Player position: (%f, %f), Map Grid Index: (%d, %d)\n", x, y, mapGridIndexX, mapGridIndexY);

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
		MINIMAP_SCALE_FACTOR * player.x + cos(player.rotationAngle) * 10,
		MINIMAP_SCALE_FACTOR * player.y + sin(player.rotationAngle) * 10
	);
}

void CastRay(float rayAngle, int stripID)
{
	rayAngle = NormalizeAngle(rayAngle);

	bool bIsRayFacingUp		= rayAngle > PI && rayAngle < TWO_PI;
	bool bIsRayFacingDown	= !bIsRayFacingUp;
	bool bIsRayFacingRight	= rayAngle < PI * 0.5 || rayAngle > PI * 1.5;
	bool bIsRayFacingLeft	= !bIsRayFacingRight;

	float xIntercept, yIntercept;
	float xStep, yStep;


	///////////////////////////////////////
	/// HORIZONTAL RAY-GRID INTERSECTION
	///////////////////////////////////////

	bool	bIsAHorizontalWallHit	= false;
	float	horizontalWallHitX		= 0;
	float	horizontalWallHitY		= 0;
	int		horizontalWallContent	= 0;

	// calculate coordinates of nearest horizontal grid intersection
	// y
	yIntercept = floor(player.y / TILE_SIZE) * TILE_SIZE;
	yIntercept += bIsRayFacingDown ? TILE_SIZE : 0;
	// x
	xIntercept = player.x + (yIntercept - player.y) / (float)tan(rayAngle);

	// calculate step increments
	yStep = TILE_SIZE;
	yStep *= bIsRayFacingUp	? -1 : 1;

	xStep = TILE_SIZE / (float)tan(rayAngle);
	xStep *= (bIsRayFacingLeft	&& xStep > 0)	? -1 : 1;
	xStep *= (bIsRayFacingRight && xStep < 0)	? -1 : 1;

	float nextHorizontalLineX = xIntercept;
	float nextHorizontalLineY = yIntercept;

	// Increment steps until hitting a wall
	while (nextHorizontalLineX >= 0 && nextHorizontalLineX <= MAP_NUM_COLS * TILE_SIZE && nextHorizontalLineY >= 0 && nextHorizontalLineY <= MAP_NUM_ROWS * TILE_SIZE)
	{
		float xTemp = nextHorizontalLineX;
		float yTemp = nextHorizontalLineY + (bIsRayFacingUp ? -1 : 0);

		if (CheckCollision(xTemp, yTemp))
		{
			// ray hit a wall!
			horizontalWallHitX = nextHorizontalLineX;
			horizontalWallHitY = nextHorizontalLineY;
			horizontalWallContent = map[(int)floor(yTemp / TILE_SIZE)][(int)floor(xTemp / TILE_SIZE)];
			bIsAHorizontalWallHit = true;
			break;
		}
		else
		{
			nextHorizontalLineX += xStep;
			nextHorizontalLineY += yStep;
		}
	}

	///////////////////////////////////////
	/// VERTICAL RAY-GRID INTERSECTION
	///////////////////////////////////////

	bool	bIsAVerticalWallHit	= false;
	float	verticalWallHitX	= 0;
	float	verticalWallHitY	= 0;
	int		verticalWallContent = 0;

	// calculate coordinates of nearest horizontal grid intersection
	// x
	xIntercept = floor(player.x / TILE_SIZE) * TILE_SIZE;
	xIntercept += bIsRayFacingRight ? TILE_SIZE : 0;
	// y
	yIntercept = player.y + (xIntercept - player.x) * (float)tan(rayAngle);

	// calculate step increments
	xStep = TILE_SIZE;
	xStep *= bIsRayFacingLeft ? -1 : 1;

	yStep = TILE_SIZE * (float)tan(rayAngle);
	yStep *= (bIsRayFacingUp	&& yStep > 0) ? -1 : 1;
	yStep *= (bIsRayFacingDown	&& yStep < 0) ? -1 : 1;

	float nextVerticalLineX = xIntercept;
	float nextVerticalLineY = yIntercept;

	// Increment steps until hitting a wall
	while (nextVerticalLineX >= 0 && nextVerticalLineX <= MAP_NUM_COLS * TILE_SIZE && nextVerticalLineY >= 0 && nextVerticalLineY <= MAP_NUM_ROWS * TILE_SIZE)
	{
		float xTemp = nextVerticalLineX + (bIsRayFacingLeft ? -1 : 0);
		float yTemp = nextVerticalLineY;

		if (CheckCollision(xTemp, yTemp))
		{
			// ray hit a wall!
			verticalWallHitX = nextVerticalLineX;
			verticalWallHitY = nextVerticalLineY;
			verticalWallContent = map[(int)floor(yTemp / TILE_SIZE)][(int)floor(xTemp / TILE_SIZE)];
			bIsAVerticalWallHit = true;
			break;
		}
		else
		{
			nextVerticalLineX += xStep;
			nextVerticalLineY += yStep;
		}
	}

	// CALCULATE THE NEAREST HIT
	float horizontalHitDistance = bIsAHorizontalWallHit
		? DistanceBetweenPoints(player.x, player.y, horizontalWallHitX, horizontalWallHitY)
		: FLT_MAX;

	float verticalHitDistance = bIsAVerticalWallHit
		? DistanceBetweenPoints(player.x, player.y, verticalWallHitX, verticalWallHitY)
		: FLT_MAX;


	if (verticalHitDistance < horizontalHitDistance)
	{
		rays[stripID].distance			= verticalHitDistance;
		rays[stripID].wallHitX			= verticalWallHitX;
		rays[stripID].wallHitY			= verticalWallHitY;
		rays[stripID].wallHitContent	= verticalWallContent;
		rays[stripID].bIsAVerticalHit	= true;
		rays[stripID].rayAngle = rayAngle;
	}
	else
	{
		rays[stripID].distance			= horizontalHitDistance;
		rays[stripID].wallHitX			= horizontalWallHitX;
		rays[stripID].wallHitY			= horizontalWallHitY;
		rays[stripID].wallHitContent	= horizontalWallContent;
		rays[stripID].bIsAVerticalHit	= false;
		rays[stripID].rayAngle = rayAngle;
	}

	
}

void CastAllRays()
{
	for (int col = 0; col < NUM_RAYS; col++)
	{
		float rayAngle = player.rotationAngle + atan((col - NUM_RAYS * 0.5) / DISTANCE_TO_PROJECTION_PLANE);
		CastRay(rayAngle, col);
	}
}

/// <Color Palet>
/// #63345E 
/// #AC61B9 
/// #B7C1DE 
/// #0B468C 
/// #092047 
/// </Color Palet>

void RenderMap()
{
	for (int i = 0; i < MAP_NUM_ROWS; i++)
	{
		for (int j = 0; j < MAP_NUM_COLS; j++)
		{
			int tileX = j * TILE_SIZE;
			int tileY = i * TILE_SIZE;
			int tileColorR = map[i][j] != 0 ? 255 : 0;
			int tileColorG = map[i][j] != 0 ? 255 : 0;
			int tileColorB = map[i][j] != 0 ? 255 : 0;

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

void RenderRays()
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
	for (int i = 0; i < NUM_RAYS; i++)
	{
		SDL_RenderDrawLine
		(
			renderer,
			MINIMAP_SCALE_FACTOR * player.x,
			MINIMAP_SCALE_FACTOR * player.y,
			MINIMAP_SCALE_FACTOR * rays[i].wallHitX,
			MINIMAP_SCALE_FACTOR * rays[i].wallHitY
		);
	}
}

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
		for (int c = 0; c < wallTopPixel; c++)
		{
			colorBuffer[(RESOLUTION_WINDOW_WIDTH * c) + x] = 0x403C3C;
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
			colorBuffer[(RESOLUTION_WINDOW_WIDTH * y) + x] = texelColor;

			// if no texture, fall back to default colors
			// colorBuffer[(WINDOW_WIDTH * y) + i] = rays[i].bIsVerticalHit ?  0xFFB7C1DE : 0xFFA7B1CE;	
		}
		
		// render floor
		for (int f = wallBottomPixel; f < RESOLUTION_WINDOW_HEIGHT; f++)
		{
			colorBuffer[(RESOLUTION_WINDOW_WIDTH * f) + x] = 0x706C6C;
		}
	}

}

void ClearColorBuffer(uint32_t color)
{
	for (int i = 0; i < RESOLUTION_WINDOW_WIDTH * RESOLUTION_WINDOW_HEIGHT; i++)
	{
		colorBuffer[i] = color;
	}
}

void RenderColorBuffer()
{
	SDL_UpdateTexture
	(
		colorBufferTexture,
		NULL,
		colorBuffer,
		(int)((uint32_t)RESOLUTION_WINDOW_WIDTH * sizeof(uint32_t))
	);

	SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL);
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
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // set color
	SDL_RenderClear(renderer); // clear buffer

	RenderWallProjection();
	
	RenderColorBuffer();
	ClearColorBuffer(0xFF000000);

	// Display Minimap
	RenderMap();
	RenderRays();
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