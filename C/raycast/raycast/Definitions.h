#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <math.h>

#define PI 3.14159265
#define TWO_PI 6.28318530

#define TILE_SIZE 64

#define NUM_TEXTURES 9

#define MINIMAP_SCALE_FACTOR 0.15



///////////////////////////
/// RESOLUTION SETTINGS ///
///////////////////////////

#define RESOLUTION_WINDOW_WIDTH 1024
#define RESOLUTION_WINDOW_HEIGHT 576

/*
	## 4:3
	640	 * 480
	800	 * 600
	1024 * 768
	1152 * 864
	1280 * 960

	## 16 : 9
	640  * 360
	800  * 450
	1024 * 576
	1152 * 648
	1280 * 720
	1600 * 900
	1920 * 1080


*/

#define FOV_ANGLE (60 * PI / 180)

#define NUM_RAYS RESOLUTION_WINDOW_WIDTH

#define DISTANCE_TO_PROJECTION_PLANE ((RESOLUTION_WINDOW_WIDTH * 0.5) / (float)tan(FOV_ANGLE * 0.5))

#define FPS 60
#define FRAME_TIME_LENGTH (1000 / FPS)

#endif