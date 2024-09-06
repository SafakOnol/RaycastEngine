#include "Ray.h"

ray_t rays[NUM_RAYS]; 

bool CheckRayFacingUp(float angle)
{
	return angle > PI && angle < TWO_PI;;
}
bool CheckRayFacingDown(float angle)
{
	return !CheckRayFacingUp(angle);
}
bool CheckRayFacingRight(float angle)
{
	return angle < PI * 0.5 || angle > PI * 1.5;
}
bool CheckRayFacingLeft(float angle)
{
	return !CheckRayFacingRight(angle);
}
;

void CastRay(float rayAngle, int stripID)
{
	NormalizeAngle(&rayAngle);

	float xIntercept, yIntercept;
	float xStep, yStep;


	///////////////////////////////////////
	/// HORIZONTAL RAY-GRID INTERSECTION
	///////////////////////////////////////

	bool	bIsAHorizontalWallHit = false;
	float	horizontalWallHitX = 0;
	float	horizontalWallHitY = 0;
	int		horizontalWallContent = 0;

	// calculate coordinates of nearest horizontal grid intersection
	// y
	yIntercept = floor(player.y / TILE_SIZE) * TILE_SIZE;
	yIntercept += CheckRayFacingDown(rayAngle) ? TILE_SIZE : 0;
	// x
	xIntercept = player.x + (yIntercept - player.y) / (float)tan(rayAngle);

	// calculate step increments
	yStep = TILE_SIZE;
	yStep *= CheckRayFacingUp(rayAngle) ? -1 : 1;

	xStep = TILE_SIZE / (float)tan(rayAngle);
	xStep *= (CheckRayFacingLeft(rayAngle) && xStep > 0) ? -1 : 1;
	xStep *= (CheckRayFacingRight(rayAngle) && xStep < 0) ? -1 : 1;

	float nextHorizontalLineX = xIntercept;
	float nextHorizontalLineY = yIntercept;

	// Increment steps until hitting a wall
	while (CheckMapBoundary(nextHorizontalLineX, nextHorizontalLineY))
	{
		float xTemp = nextHorizontalLineX;
		float yTemp = nextHorizontalLineY + (CheckRayFacingUp(rayAngle) ? -1 : 0);

		if (CheckWallCollision(xTemp, yTemp))
		{
			// ray hit a wall!
			horizontalWallHitX = nextHorizontalLineX;
			horizontalWallHitY = nextHorizontalLineY;
			horizontalWallContent = GetMapAt((int)floor(yTemp / TILE_SIZE), (int)floor(xTemp / TILE_SIZE));
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

	bool	bIsAVerticalWallHit = false;
	float	verticalWallHitX = 0;
	float	verticalWallHitY = 0;
	int		verticalWallContent = 0;

	// calculate coordinates of nearest horizontal grid intersection
	// x
	xIntercept = floor(player.x / TILE_SIZE) * TILE_SIZE;
	xIntercept += CheckRayFacingRight(rayAngle) ? TILE_SIZE : 0;
	// y
	yIntercept = player.y + (xIntercept - player.x) * (float)tan(rayAngle);

	// calculate step increments
	xStep = TILE_SIZE;
	xStep *= CheckRayFacingLeft(rayAngle) ? -1 : 1;

	yStep = TILE_SIZE * (float)tan(rayAngle);
	yStep *= (CheckRayFacingUp(rayAngle) && yStep > 0) ? -1 : 1;
	yStep *= (CheckRayFacingDown(rayAngle) && yStep < 0) ? -1 : 1;

	float nextVerticalLineX = xIntercept;
	float nextVerticalLineY = yIntercept;

	// Increment steps until hitting a wall
	while (CheckMapBoundary(nextVerticalLineX, nextVerticalLineY))
	{
		float xTemp = nextVerticalLineX + (CheckRayFacingLeft(rayAngle) ? -1 : 0);
		float yTemp = nextVerticalLineY;

		if (CheckWallCollision(xTemp, yTemp))
		{
			// ray hit a wall!
			verticalWallHitX = nextVerticalLineX;
			verticalWallHitY = nextVerticalLineY;
			verticalWallContent = GetMapAt((int)floor(yTemp / TILE_SIZE), (int)floor(xTemp / TILE_SIZE));
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
		rays[stripID].distance = verticalHitDistance;
		rays[stripID].wallHitX = verticalWallHitX;
		rays[stripID].wallHitY = verticalWallHitY;
		rays[stripID].wallHitContent = verticalWallContent;
		rays[stripID].bIsAVerticalHit = true;
		rays[stripID].rayAngle = rayAngle;
	}
	else
	{
		rays[stripID].distance = horizontalHitDistance;
		rays[stripID].wallHitX = horizontalWallHitX;
		rays[stripID].wallHitY = horizontalWallHitY;
		rays[stripID].wallHitContent = horizontalWallContent;
		rays[stripID].bIsAVerticalHit = false;
		rays[stripID].rayAngle = rayAngle;
	}


};

void CastAllRays(void)
{
	for (int col = 0; col < NUM_RAYS; col++)
	{
		// eliminate angle distortion
		float rayAngle = player.rotationAngle + atan((col - NUM_RAYS * 0.5) / DISTANCE_TO_PROJECTION_PLANE);
		CastRay(rayAngle, col);
	}
};

void RenderMiniMapRays()
{
	for (int i = 0; i < NUM_RAYS; i += 25)
	{
		DrawLine
		(
			player.x * MINIMAP_SCALE_FACTOR,
			player.y * MINIMAP_SCALE_FACTOR,
			rays[i].wallHitX * MINIMAP_SCALE_FACTOR,
			rays[i].wallHitY * MINIMAP_SCALE_FACTOR,
			0xFF0000FF
		);
	}
};

