// RAYCAST ENGINE PROJECT - JS PROTOTYPING // SAFAK ONOL 04/2024
// THIS PROJECT USES p5.js library

// ----- PROJECT CONSTANTS ----- ///
const TILE_SIZE = 64;
const MAP_NUM_ROWS = 16;
const MAP_NUM_COLS = 16;

const WINDOW_WIDTH  = MAP_NUM_COLS * TILE_SIZE;
const WINDOW_HEIGHT = MAP_NUM_ROWS * TILE_SIZE;

const PLAYER_DIRECTION_LENGTH = 10;

const FOV = 60 * (Math.PI / 180);

const WALL_STRIP_WIDTH = 1; // Wall thickness in px
const NUM_RAYS = WINDOW_WIDTH / WALL_STRIP_WIDTH;

const MINIMAP_SCALE_FACTOR = 0.2;

// ----- PROJECT CLASSES ----- // 
class MyMap
{
    constructor()
    {
        this.grid = 
        [   // 1 = wall (collision), 0 = empty (no collision)
            [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
            [1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1],
            [1, 0, 0, 1, 0, 0, 2, 2, 2, 2, 0, 0, 1, 0, 0, 1],
            [1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1],
            [1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1],
            [1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1],
            [1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1],
            [1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            [1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1],
            [1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            [1, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            [1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            [1, 0, 0, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1] 
        ];
    }

    hasCollisionAt(x, y)
    {
        if (x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT)
        {
            return true; // return collision
        }

        var mapGridIndexX = Math.floor(x / TILE_SIZE);
        var mapGridIndexY = Math.floor(y / TILE_SIZE);

        return this.grid[mapGridIndexY][mapGridIndexX] != 0; // x and y, columns and rows inverted
    }

    getGridDataAt(x, y)
    {
        if (x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT)
        {
            return 0;
        }
        var mapGridIndexX = Math.floor(x / TILE_SIZE);
        var mapGridIndexY = Math.floor(y / TILE_SIZE);
        return this.grid[mapGridIndexY][mapGridIndexX];
    }


    render()
    {
        for (var i = 0; i < MAP_NUM_ROWS; i++)
        {
            for (var j = 0; j < MAP_NUM_COLS; j++)
            {
                var tileX = j * TILE_SIZE;
                var tileY = i * TILE_SIZE;
                var tileColor = this.grid[i][j] == 0 ? "#fff" : "#222";
                stroke("#222");
                fill(tileColor);
                rect
                (
                    MINIMAP_SCALE_FACTOR * tileX, 
                    MINIMAP_SCALE_FACTOR * tileY, 
                    MINIMAP_SCALE_FACTOR * TILE_SIZE, 
                    MINIMAP_SCALE_FACTOR * TILE_SIZE
                );
            }
        }
    }
}

class Player
{
    constructor()
    {
        // start player in the middle of the map
        this.x = WINDOW_WIDTH * 0.5;
        this.y = WINDOW_HEIGHT * 0.4; 

        //
        this.radius = 5;
        this.turnDirection = 0 ; // -1 if left, +1 if right
        this.walkDirection = 0;  // -1 if back, +1 if front
        this.rotationAngle = Math.PI / 2;
        this.moveSpeed = 4.0;
        this.rotationSpeed = 2 * (Math.PI / 180); // 2 degrees converted to Radians
    }

    update()
    {

        // update player position based on turnDirection and walkDirection
 
        this.rotationAngle += this.turnDirection * this.rotationSpeed;

        var moveStep = this.walkDirection * this.moveSpeed;

        var newPlayerX = this.x + moveStep * Math.cos(this.rotationAngle);
        var newPlayerY = this.y + moveStep * Math.sin(this.rotationAngle);

        // Move player only if the new position isn't colliding with a wall
        if(!grid.hasCollisionAt(newPlayerX, newPlayerY))
        {
            this.x = newPlayerX;
            this.y = newPlayerY;
        }
  
    }

    render()
    {
        // Player Position
        noStroke();
        fill("blue");
        circle
        (
            MINIMAP_SCALE_FACTOR * this.x, 
            MINIMAP_SCALE_FACTOR * this.y, 
            MINIMAP_SCALE_FACTOR * this.radius
        );
        // Player Direction Indicator
        stroke("blue");
        line
        (
            MINIMAP_SCALE_FACTOR * this.x,
            MINIMAP_SCALE_FACTOR * this.y,
            MINIMAP_SCALE_FACTOR * this.x + Math.cos(this.rotationAngle) * PLAYER_DIRECTION_LENGTH,
            MINIMAP_SCALE_FACTOR * this.y + Math.sin(this.rotationAngle) * PLAYER_DIRECTION_LENGTH
        );
    }
}

class Ray
{
    constructor(rayAngle)
    {
        this.rayAngle = normalizeAngle(rayAngle);
        this.wallHitX = 0;
        this.wallHitY = 0;
        this.distance = 0;
        this.wasHitVertical = false;
        this.hitWallColor = 0;

        this.isRayFacingDown    =  this.rayAngle > 0 && this.rayAngle < Math.PI;
        this.isRayFacingUp      = !this.isRayFacingDown;
        this.isRayFacingRight   =  this.rayAngle < 0.5 * Math.PI || this.rayAngle > 1.5 * Math.PI;
        this.isRayFacingLeft    = !this.isRayFacingRight;
    }

    cast()
    {
        var xIntercept, yIntercept;
        var xStep, yStep;

        ////////////////////////////////////////////
        /// HORIZONTAL RAY-GRID INTERSECTION CODE
        ////////////////////////////////////////////

        var hitIsOnHorizontalWall = false;
        var horizontalWallHitX = 0;
        var horizontalWallHitY = 0;
        var horizontalWallColor = 0;

        // Find the y-coordinate of the closest horizontal grid intersection
        yIntercept = Math.floor(player.y / TILE_SIZE) * TILE_SIZE;
        yIntercept += this.isRayFacingDown ? TILE_SIZE : 0;

        // Find the x-coordinate of the closest horizontal grid intersection
        xIntercept = player.x + ((yIntercept - player.y) / Math.tan(this.rayAngle));

        yStep = TILE_SIZE;
        yStep *= this.isRayFacingUp ? -1 : 1;

        xStep = TILE_SIZE / Math.tan(this.rayAngle);
        xStep *= (this.isRayFacingLeft && xStep > 0) ? -1 : 1;
        xStep *= (this.isRayFacingRight && xStep < 0) ? -1 : 1;

        var nextHorizontalTileBorderX = xIntercept;
        var nextHorizontalTileBorderY = yIntercept;

        // Increment xStep and yStep until ray hits a wall
        while(  nextHorizontalTileBorderX >= 0 && nextHorizontalTileBorderX <= WINDOW_WIDTH && 
                nextHorizontalTileBorderY >= 0 && nextHorizontalTileBorderY <= WINDOW_HEIGHT )
        {
            var wallGridData = grid.getGridDataAt
            (
                nextHorizontalTileBorderX,
                nextHorizontalTileBorderY + (this.isRayFacingUp ? -1 : 0) // if ray is facing up, push 1 px to place inside the grid cell
            );
            if (wallGridData != 0)
            {
                hitIsOnHorizontalWall   = true;
                horizontalWallHitX      = nextHorizontalTileBorderX;
                horizontalWallHitY      = nextHorizontalTileBorderY;
                horizontalWallColor     = wallGridData;

                break;
            }
            else
            {
                nextHorizontalTileBorderX += xStep;
                nextHorizontalTileBorderY += yStep;
            }
        }
    

        ///////////////////////////////////////////
        /// VERTICAL RAY-GRID INTERSECTION CODE
        ///////////////////////////////////////////

        var hitIsOnVerticalWall = false;
        var verticalWallHitX = 0;
        var verticalWallHitY = 0;
        var verticalWallColor = 0;

        
        // Find the x-coordinate of the closest vertical grid intersection
        xIntercept = Math.floor(player.x / TILE_SIZE) * TILE_SIZE;
        xIntercept += this.isRayFacingRight ? TILE_SIZE : 0;

        // Find the y-coordinate of the closest vertical grid intersection
        yIntercept = player.y + ((xIntercept - player.x) * Math.tan(this.rayAngle));

        xStep = TILE_SIZE;
        xStep *= this.isRayFacingLeft ? -1 : 1;

        yStep = TILE_SIZE * Math.tan(this.rayAngle);
        yStep *= (this.isRayFacingUp && yStep > 0) ? -1 : 1;
        yStep *= (this.isRayFacingDown && yStep < 0) ? -1 : 1;

        var nextVerticalTileBorderX = xIntercept;
        var nextVerticalTileBorderY = yIntercept;

        // Increment xStep and yStep until ray hits a wall
        while(  nextVerticalTileBorderX >= 0 && nextVerticalTileBorderX <= WINDOW_WIDTH && 
                nextVerticalTileBorderY >= 0 && nextVerticalTileBorderY <= WINDOW_HEIGHT )
        {
            var wallGridData = grid.getGridDataAt(
                nextVerticalTileBorderX + (this.isRayFacingLeft ? -1 : 0), // if ray facing left, push 1px left to place incide grid cell
                nextVerticalTileBorderY
            );
            if (grid.hasCollisionAt(nextVerticalTileBorderX - (this.isRayFacingLeft ? 1 : 0), nextVerticalTileBorderY))
            {
                hitIsOnVerticalWall = true;
                verticalWallHitX    = nextVerticalTileBorderX;
                verticalWallHitY    = nextVerticalTileBorderY;
                verticalWallColor   = wallGridData;

                break;
            }
            else
            {
                nextVerticalTileBorderX += xStep;
                nextVerticalTileBorderY += yStep;
            }
        }

        /// CALCULATE THE NEAREST HIT
        var horizontalHitDistance = (hitIsOnHorizontalWall)
            ? distanceBetweenPoints(player.x, player.y, horizontalWallHitX, horizontalWallHitY)
            : Number.MAX_VALUE;
        var verticalHitDistance = (hitIsOnVerticalWall)
            ? distanceBetweenPoints(player.x, player.y, verticalWallHitX, verticalWallHitY)
            : Number.MAX_VALUE;

        // only store the nearest hit point values
        if (verticalHitDistance < horizontalHitDistance)
        {
            this.wallHitX       = verticalWallHitX;
            this.wallHitY       = verticalWallHitY;
            this.distance       = verticalHitDistance;
            this.hitWallColor   = verticalWallColor;
            this.wasHitVertical = true;
        }
        else
        {
            this.wallHitX       = horizontalWallHitX;
            this.wallHitY       = horizontalWallHitY;
            this.distance       = horizontalHitDistance;
            this.hitWallColor   = horizontalWallColor;
            this.wasHitVertical = false;
        }
    }

    render()
    {
        stroke("rgba(255, 0, 0, 0.1)");
        line
        (
            MINIMAP_SCALE_FACTOR * player.x,
            MINIMAP_SCALE_FACTOR * player.y,
            MINIMAP_SCALE_FACTOR * this.wallHitX,
            MINIMAP_SCALE_FACTOR * this.wallHitY
        );

        // hit marker
        //
        // stroke("rgba(80, 159, 60, 1)");
        // fill(0,0,0,0);
        // circle
        // (
        //     MINIMAP_SCALE_FACTOR * this.wallHitX, 
        //     MINIMAP_SCALE_FACTOR * this.wallHitY, 
        //     MINIMAP_SCALE_FACTOR * 1
        // );
    }

}

// ----- GLOBAL VARIABLES ----- //
var grid = new MyMap();
var player = new Player();
var rays = [];

// ----- MAIN FUNCTIONS ----- //
function keyPressed()
{
    if (keyCode == UP_ARROW)
    {
        player.walkDirection = +1;
    }
    else if (keyCode == DOWN_ARROW)
    {
        player.walkDirection = -1;
    }
    else if (keyCode == RIGHT_ARROW)
    {
        player.turnDirection = +1;
    }
    else if (keyCode == LEFT_ARROW)
    {
        player.turnDirection = -1;
    }
}

function keyReleased()
{
    if (keyCode == UP_ARROW)
    {
        player.walkDirection = 0;
    }
    else if (keyCode == DOWN_ARROW)
    {
        player.walkDirection = 0;
    }
    else if (keyCode == RIGHT_ARROW)
    {
        player.turnDirection = 0;
    }
    else if (keyCode == LEFT_ARROW)
    {
        player.turnDirection = 0;
    }
}

function castAllRays()
{
    // start first ray by subtracting half of the FOV from rotationAngle
    var rayAngle = player.rotationAngle - (FOV / 2);

    rays = [];

    // loop all columns casting the rays
    for (var col = 0; col < NUM_RAYS; col++)
    //for (var i = 0; i < 1; i++)
    {
        var ray = new Ray(rayAngle);
        ray.cast();

        rays.push(ray); // add the ray to the list of rays
        rayAngle += FOV / NUM_RAYS;
    }

}

function normalizeAngle(angle)
{
    angle = angle % (2 * Math.PI);
    if (angle < 0)
    {
        angle += (2 * Math.PI);
    }
    return angle;
}

function distanceBetweenPoints(x1, y1, x2, y2)
{
    return Math.sqrt((x2-x1) * (x2-x1) + (y2-y1) * (y2-y1));
}

function renderBackground()
{
    // ceiling
    fill("rgba(234, 144, 108, 1)");
    noStroke();
    rect( 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // ground
    fill("rgba(43, 42, 76, 1)");
    noStroke();
    rect( 0, WINDOW_HEIGHT * 0.5, WINDOW_WIDTH, WINDOW_HEIGHT);
}

function renderWallProjection3D()
{
    // Loop every ray in the array rays[]
    for (var i = 0; i < NUM_RAYS; i++)
    {
        var ray = rays[i];
        
        //var rayDistance = ray.distance;

        // fix fisheye effect due to difference between rotation angle vs individual ray angle
        var correctedWallDistance = ray.distance * Math.cos(ray.rayAngle - player.rotationAngle);

        // the distance between player and projection plane
        var distanceToProjectionPlane = (WINDOW_WIDTH * 0.5) / Math.tan(FOV * 0.5);

        // projected wall height
        var wallStripHeight = (TILE_SIZE / correctedWallDistance) * distanceToProjectionPlane;

        
        // calculate the color fade depending on the wall distance

        var fadeFactor = 2;
        var alpha = 1.0;//distanceToProjectionPlane / (fadeFactor * correctedWallDistance) ;

        //////////////////////////////
        /// 1 = RGB(179, 19,   18)
        /// 2 = RGB(71,  147,  175)
        /// 3 = RGB(161, 195,  152)
        //////////////////////////////

        var colorR = 179;
        var colorG = 19;
        var colorB = 18;

        if (ray.hitWallColor == 2)
        {
            colorR = 71;
            colorG = 147;
            colorB = 175;
        }

        if (ray.hitWallColor == 3)
        {
            colorR = 161;
            colorG = 195;
            colorB = 152;
        }

        // else if (ray.wallGridData == 2)
        // {
        //     colorR = 71;
        //     colorG = 147
        //     colorB = 175;
        // }

        // else if (ray.wallGridData == 3)
        // {
        //     colorR = 161;
        //     colorG = 195;
        //     colorB = 152;
        // }

        var intensityModifier = ray.wasHitVertical ? 1 : 0.8;

        var colorIntensity = distanceToProjectionPlane / (fadeFactor * correctedWallDistance);

        var renderR = Math.floor(Math.min(Math.floor(colorR * colorIntensity), colorR) * intensityModifier);
        var renderG = Math.floor(Math.min(Math.floor(colorG * colorIntensity), colorG) * intensityModifier);
        var renderB = Math.floor(Math.min(Math.floor(colorB * colorIntensity), colorB) * intensityModifier);

        // render a rectangle with the calculated wall height and fade according to distance
        fill("rgba(" + renderR + ", " + renderG + ", " + renderB + ", " + alpha + ")");
        noStroke();
        rect
        (
            i * WALL_STRIP_WIDTH,
            (WINDOW_HEIGHT * 0.5) - (wallStripHeight * 0.5),
            WALL_STRIP_WIDTH,
            wallStripHeight
        );

    }
}

// ----- GAME LOOP FUNCTIONS ----- // 
function setup()
{
    // TODO: initialize all objects here

    createCanvas(WINDOW_WIDTH, WINDOW_HEIGHT);
}

function update()
{
    // TODO: update all game objects here
    player.update();  
    castAllRays();
}

function draw()
{
    clear("#111");
    
    update(); // update everything before render!

    // TODO: render all objects here
    renderBackground();
    renderWallProjection3D();

    grid.render(); // Minimap
    
    
    for (ray of rays)
    {
        ray.render();
    }

    player.render();

    
}