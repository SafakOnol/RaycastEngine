#ifndef RAY_H
#define RAY_H

#include <stdbool.h>
#include <float.h>

#include "Utility.h"
#include "Definitions.h"
#include "Player.h"


typedef struct
{
	float rayAngle;
	float wallHitX;
	float wallHitY;
	float distance;
	bool bIsAVerticalHit;
	int wallHitContent;
} ray_t;

extern ray_t rays[NUM_RAYS];	// Decleration of the array in the type of ray_t
								// This variable is initialized in another file (therefore extern)
bool CheckRayFacingUp(float angle);
bool CheckRayFacingDown(float angle);
bool CheckRayFacingRight(float angle);
bool CheckRayFacingLeft(float angle);

void CastRay(float rayAngle, int stripID);
void CastAllRays(void);
void RenderMiniMapRays(void);

#endif
