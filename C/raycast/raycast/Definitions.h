#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <math.h>

#define PI 3.14159265
#define TWO_PI 6.28318530

#define TILE_SIZE 64

#define NUM_TEXTURES 9

#define MINIMAP_SCALE_FACTOR 0.15

#define RESOLUTION_WINDOW_WIDTH 1280
#define RESOLUTION_WINDOW_HEIGHT 720

#define FOV_ANGLE (90 * PI / 180)

#define NUM_RAYS RESOLUTION_WINDOW_WIDTH

#define DISTANCE_TO_PROJECTION_PLANE ((RESOLUTION_WINDOW_WIDTH * 0.5) / (float)tan(FOV_ANGLE * 0.5))

#define FPS 30
#define FRAME_TIME_LENGTH (1000 / FPS)

#endif