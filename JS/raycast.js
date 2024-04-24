// RAYCAST ENGINE PROJECT - JS PROTOTYPING // SAFAK ONOL 04/2024
// THIS PROJECT USES p5.js library

// ----- PROJECT CONSTANTS ----- ///
const TILE_SIZE = 64;
const MAP_NUM_ROWS = 11;
const MAP_NUM_COLS = 15;

const WINDOW_WIDTH  = MAP_NUM_COLS * TILE_SIZE;
const WINDOW_HEIGHT = MAP_NUM_ROWS * TILE_SIZE;

const PLAYER_DIRECTION_LENGTH = 30;

const FOV = 60 * (Math.PI / 180);

const WALL_STRIP_WIDTH = 1; // Wall thickness in px
const NUM_RAYS = WINDOW_WIDTH / WALL_STRIP_WIDTH;


// ----- PROJECT CLASSES ----- // 
class MyMap
{
    constructor()
    {
        this.grid = 
        [   // 1 = wall (collision), 0 = empty (no collision)
            [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1],
            [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1],
            [1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            [1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]       
        ];
    }

    hasWallCollisionAt(x, y)
    {
        if (x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT)
        {
            return true; // return collision
        }

        var mapGridIndexX = Math.floor(x / TILE_SIZE);
        var mapGridIndexY = Math.floor(y / TILE_SIZE);

        return this.grid[mapGridIndexY][mapGridIndexX] != 0; // x and y, columns and rows inverted
    }

    render()
    {
        for (var i = 0; i < MAP_NUM_ROWS; i++)
        {
            for (var j = 0; j < MAP_NUM_COLS; j++)
            {
                var tileX = j * TILE_SIZE;
                var tileY = i * TILE_SIZE;
                var tileColor = this.grid[i][j] == 1 ? "#222" : "#fff";
                stroke("#222");
                fill(tileColor);
                rect(tileX, tileY, TILE_SIZE, TILE_SIZE);
            }
        }
    }
}

class Player
{
    constructor()
    {
        // start player in the middle of the map
        this.x = WINDOW_WIDTH / 2;
        this.y = WINDOW_HEIGHT / 2; 

        //
        this.radius = 5;
        this.turnDirection = 0 ; // -1 if left, +1 if right
        this.walkDirection = 0;  // -1 if back, +1 if front
        this.rotationAngle = Math.PI / 2;
        this.moveSpeed = 2.0;
        this.rotationSpeed = 2 * (Math.PI / 180); // 2 degrees converted to Radians
    }

    update()
    {
        // TODO:
        // update player position based on turnDirection and walkDirection
        //console.log(this.turnDirection);
        this.rotationAngle += this.turnDirection * this.rotationSpeed;

        var moveStep = this.walkDirection * this.moveSpeed;

        var newPlayerX = this.x + moveStep * cos(this.rotationAngle);
        var newPlayerY = this.y + moveStep * sin(this.rotationAngle);

        // Move player only if the new position isn't colliding with a wall
        if(!grid.hasWallCollisionAt(newPlayerX, newPlayerY))
        {
            this.x = newPlayerX;
            this.y = newPlayerY;
        }
  
    }

    render()
    {
        // Player Position
        noStroke();
        fill("red");
        circle(this.x, this.y, this.radius);
        // Player Direction Indicator
        // stroke("red");
        // line
        // (
        //     this.x,
        //     this.y,
        //     this.x + Math.cos(this.rotationAngle) * PLAYER_DIRECTION_LENGTH,
        //     this.y + Math.sin(this.rotationAngle) * PLAYER_DIRECTION_LENGTH
        // );
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

        this.isRayFacingDown    = this.rayAngle > 0 && this.rayAngle < Math.PI;
        this.isRayFacingUp      = !this.isRayFacingDown;
        this.isRayFacingRight   = this.rayAngle < 0.5 * Math.PI || this.rayAngle > 1.5 * Math.PI;
        this.isRayFacingLeft    = !this.isRayFacingRight;
    }

    cast(columnId)
    {
        var xIntercept, yIntercept;
        var xStep, yStep;

        ////////////////////////////////////////////
        /// HORIZONTAL RAY-GRID INTERSECTION CODE
        ////////////////////////////////////////////

        var foundHorizontalWallHit = false;
        var horizontalWallHitX = 0;
        var horizontalWallHitY = 0;

        
        // Find the y-coordinate of the closest horizontal grid intersection
        yIntercept = Math.floor(player.y / TILE_SIZE) * TILE_SIZE;
        yIntercept += this.isRayFacingDown ? TILE_SIZE : 0;

        console.log("isRayFacingRight?", this.isRayFacingRight);
        ///////////////////////////////////
        // Alternative Approach- SAFAK ONOL

        // if (this.isRayFacingUp)
        // {
        //     yIntercept = Math.floor(player.y / TILE_SIZE) * TILE_SIZE;
        // }
        // else if(this.isRayFacingDown)
        // {
        //     yIntercept = Math.ceil(player.y / TILE_SIZE) * TILE_SIZE;
        // }
        ///////////////////////////

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
            if (grid.hasWallCollisionAt(nextHorizontalTileBorderX, nextHorizontalTileBorderY - (this.isRayFacingUp ? 1 : 0)))
            {
                foundHorizontalWallHit = true;
                horizontalWallHitX = nextHorizontalTileBorderX;
                horizontalWallHitY = nextHorizontalTileBorderY;

                break;
            }
            else
            {
                nextHorizontalTileBorderX += xStep;
                nextHorizontalTileBorderY += yStep;
            }
        }
    

       ////////////////////////////////////////////
        /// VERTICAL RAY-GRID INTERSECTION CODE
        ////////////////////////////////////////////

        var foundVerticalWallHit = false;
        var verticalWallHitX = 0;
        var verticalWallHitY = 0;

        
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
            if (grid.hasWallCollisionAt(nextVerticalTileBorderX - (this.isRayFacingLeft ? 1 : 0), nextVerticalTileBorderY))
            {
                foundVerticalWallHit = true;
                verticalWallHitX = nextVerticalTileBorderX;
                verticalWallHitY = nextVerticalTileBorderY;

                break;
            }
            else
            {
                nextVerticalTileBorderX += xStep;
                nextVerticalTileBorderY += yStep;
            }
        }

        /// CALCULATE THE NEAREST HIT
        var horizontalHitDistance = (foundHorizontalWallHit)
            ? distanceBetweenPoints(player.x, player.y, horizontalWallHitX, horizontalWallHitY)
            : Number.MAX_VALUE;
        var verticalHitDistance = (foundVerticalWallHit)
            ? distanceBetweenPoints(player.x, player.y, verticalWallHitX, verticalWallHitY)
            : Number.MAX_VALUE;

        // only store the nearest hit point values
        this.wallHitX = (horizontalHitDistance < verticalHitDistance) ? horizontalWallHitX : verticalWallHitX;
        this.wallHitY = (horizontalHitDistance < verticalHitDistance) ? horizontalWallHitY : verticalWallHitY;
        this.distance = (horizontalHitDistance < verticalHitDistance) ? horizontalHitDistance : verticalHitDistance;
        this.wasHitVertical = (verticalHitDistance < horizontalHitDistance);
    }

    render()
    {
        stroke("rgba(255, 0, 0, 0.2)");
        line
        (
            player.x,
            player.y,
            this.wallHitX,
            this.wallHitY
        );

        stroke("rgba(80, 159, 60, 1)");
        fill(0,0,0,0);
        circle(this.wallHitX, this.wallHitY, 5);
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
    var columnId = 0;

    // start first ray by subtracting half of the FOV from rotationAngle
    var rayAngle = player.rotationAngle - (FOV / 2);

    rays = [];

    // loop all columns casting the rays
    for (var i = 0; i < NUM_RAYS; i++)
    //for (var i = 0; i < 1; i++)
    {
        var ray = new Ray(rayAngle);
        ray.cast(columnId);

        rays.push(ray); // add the ray to the list of rays
        rayAngle += FOV / NUM_RAYS;
        columnId++;
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
    update(); // update everything before render!

    // TODO: render all objects here
    grid.render();
    
    
    for (ray of rays)
    {
        ray.render();
    }

    player.render();

    
}