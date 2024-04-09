// RAYCAST ENGINE PROJECT - JS PROTOTYPING // SAFAK ONOL 04/2024
// THIS PROJECT USES p5.js library

// ----- PROJECT CONSTANTS ----- ///
const TILE_SIZE = 32;
const MAP_NUM_ROWS = 11;
const MAP_NUM_COLS = 15;

const WINDOW_WIDTH  = MAP_NUM_COLS * TILE_SIZE;
const WINDOW_HEIGHT = MAP_NUM_ROWS * TILE_SIZE;

const PLAYER_DIRECTION_LENGTH = 30;

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
        this.radius = 6;
        this.turnDirection = 0 ; // -1 if left, +1 if right
        this.walkDirection = 0;  // -1 if back, +1 if front
        this.rotationAngle = Math.PI / 2;
        this.moveSpeed = 2.0;
        this.rotationSpeed = 10 * (Math.PI / 180); // 2 degrees converted to Radians
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
        stroke("red");
        line
        (
            this.x,
            this.y,
            this.x + Math.cos(this.rotationAngle) * PLAYER_DIRECTION_LENGTH,
            this.y + Math.sin(this.rotationAngle) * PLAYER_DIRECTION_LENGTH
        );
    }
}



// ----- GLOBAL VARIABLES ----- //
var grid = new MyMap();
var player = new Player();

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
}

function draw()
{
    update(); // update everything before render!

    // TODO: render all objects here
    grid.render();
    player.render();
}