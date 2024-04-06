#include "raylib.h"
#define BGRAY \
    CLITERAL(Color) { 25, 25, 25, 255 }

#define MAX_SCORE 9
#define BRICKS_PER_LINE 54
#define BRICKS_SIZE 20

//------------------------------------
// Types and Structures Definition
//------------------------------------

typedef enum GameScreen {
    LOGO,
    TITLE,
    GAMEPLAY
} GameScreen;

// Ball structure
typedef struct Ball {
    Vector2 position;
    Vector2 speed;
    float radius;
    bool active;
} Ball;

// Bricks structure
typedef struct Brick {
    Vector2 position;
    Vector2 size;
    Rectangle bounds;
    bool rightSide;
} Brick;

//------------------------------------
// Program main entry point
//------------------------------------

int main() {
    //------------------------------------
    // Initialization
    //------------------------------------
    const int screenWidth = 1080;
    const int screenHeight = 1080;

    const int boundValue = (screenWidth / BRICKS_SIZE) / 2;

    InitWindow(screenWidth, screenHeight, "Idle Pong");

    Texture2D rayLogo = LoadTexture("resources/raylib_logo.png");
    Texture2D cpLogo = LoadTexture("resources/CuriosityPathGames_logo.png");
    Texture2D texBrick = LoadTexture("resources/brick.png");

    GameScreen screen = LOGO;

    int framesCounter = 0;    // General purpose frames counter
    bool gamePaused = false;  // Game paused state toggle

    Ball leftBall = {0};
    Ball rightBall = {0};

    Brick bricks[BRICKS_PER_LINE][BRICKS_PER_LINE] = {0};

    int randX, randY;
    // Generate random values for the range -10 to -5 and -5 to 10
    if (GetRandomValue(0, 1) == 0) {
        randX = GetRandomValue(-10, -5);
    } else {
        randX = GetRandomValue(5, 10);
    }
    if (GetRandomValue(0, 1) == 0) {
        randY = GetRandomValue(-10, -5);
    } else {
        randY = GetRandomValue(5, 10);
    }

    leftBall.speed = (Vector2){randX, randY};
    rightBall.speed = (Vector2){randX, randY};

    leftBall.radius = 10.0f;
    leftBall.active = true;
    leftBall.position = (Vector2){(screenHeight / 2) / 2, screenHeight / 2};
    leftBall.speed = (Vector2){randX, randY};

    rightBall.radius = 10.0f;
    rightBall.active = true;
    rightBall.position = (Vector2){(screenHeight / 2) + ((screenHeight / 2) / 2), screenHeight / 2};
    rightBall.speed = (Vector2){randX, randY};

    // Initialize bricks
    for (int j = 0; j < BRICKS_PER_LINE; j++) {
        for (int i = 0; i < BRICKS_PER_LINE; i++) {
            bricks[j][i].size = (Vector2){BRICKS_SIZE, BRICKS_SIZE};
            bricks[j][i].position = (Vector2){i * bricks[j][i].size.x, j * bricks[j][i].size.y};
            bricks[j][i].bounds = (Rectangle){bricks[j][i].position.x, bricks[j][i].position.y, bricks[j][i].size.x, bricks[j][i].size.y};

            if (i >= boundValue) {
                bricks[j][i].rightSide = true;
            } else {
                bricks[j][i].rightSide = false;
            }
        }
    }

    SetTargetFPS(360);

    // Main game loop
    while (!WindowShouldClose())  // Detect window close button or ESC key
    {
        //------------------------------------
        // Update
        //------------------------------------
        switch (screen) {
            case LOGO: {
                // Update LOGO
                framesCounter++;

                if (framesCounter > 1) {  // default value: 200
                    screen = TITLE;       // Change to TITLE screen after 3 seconds
                    framesCounter = 0;
                }

            } break;
            case TITLE: {
                // Update TITLE
                framesCounter++;
                screen = GAMEPLAY;

            } break;
            case GAMEPLAY: {
                // Update GAMEPLAY
                if (IsKeyPressed('P')) {
                    gamePaused = !gamePaused;  // Pause game option
                }

                if (!gamePaused) {
                    // Ball movement logic
                    leftBall.position.x += leftBall.speed.x;
                    leftBall.position.y += leftBall.speed.y;

                    rightBall.position.x += rightBall.speed.x;
                    rightBall.position.y += rightBall.speed.y;

                    // Collision logic: ball vs screen-limits
                    if (((leftBall.position.x + leftBall.radius) >= screenWidth) || ((leftBall.position.x - leftBall.radius) <= 0)) leftBall.speed.x *= -1;
                    if (((leftBall.position.y - leftBall.radius) <= 0) || ((leftBall.position.y + leftBall.radius) >= screenHeight)) leftBall.speed.y *= -1;

                    if (((rightBall.position.x + rightBall.radius) >= screenWidth) || ((rightBall.position.x - rightBall.radius) <= 0)) rightBall.speed.x *= -1;
                    if (((rightBall.position.y - rightBall.radius) <= 0) || ((rightBall.position.y + rightBall.radius) >= screenHeight)) rightBall.speed.y *= -1;

                    // Collision logic: ball vs bricks
                    for (int j = 0; j < BRICKS_PER_LINE; j++) {
                        for (int i = 0; i < BRICKS_PER_LINE; i++) {
                            if (bricks[j][i].rightSide && (CheckCollisionCircleRec(leftBall.position, leftBall.radius, bricks[j][i].bounds))) {
                                bricks[j][i].rightSide = false;
                                leftBall.speed.x *= -1;
                                break;
                            } else if (!bricks[j][i].rightSide && (CheckCollisionCircleRec(rightBall.position, rightBall.radius, bricks[j][i].bounds))) {
                                bricks[j][i].rightSide = true;
                                rightBall.speed.x *= -1;
                                break;
                            }
                        }
                    }
                } else {
                    DrawText("GAME PAUSED", screenWidth / 2 - 160, screenHeight / 2, 52, WHITE);
                }

            } break;
            default:
                break;
        }

        //------------------------------------
        // Draw
        //------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        switch (screen) {
            case LOGO: {
                if (framesCounter < 100) {
                    DrawTexture(cpLogo, screenWidth / 2 - cpLogo.width / 2, screenHeight / 2 - cpLogo.height / 2, WHITE);
                } else {
                    DrawText("Made with", screenWidth / 2 - 120, screenHeight / 2 - 200, 50, WHITE);
                    DrawTexture(rayLogo, screenWidth / 2 - rayLogo.width / 2, screenHeight / 2 - rayLogo.height / 2, WHITE);
                }
            } break;

            case TITLE: {
                // DrawText("Press ENTER", screenWidth / 2 - 120, screenHeight / 2 - 200, 50, BLACK);
            } break;

            case GAMEPLAY: {
                for (int j = 0; j < BRICKS_PER_LINE; j++) {
                    for (int i = 0; i < BRICKS_PER_LINE; i++) {
                        if (bricks[j][i].rightSide) {
                            DrawTextureEx(texBrick, bricks[j][i].position, 0.0f, 1.0f, BLACK);
                        } else {
                            DrawTextureEx(texBrick, bricks[j][i].position, 0.0f, 1.0f, WHITE);
                        }
                    }
                }

                DrawCircleV(leftBall.position, leftBall.radius, BLACK);    // left side ball
                DrawCircleV(rightBall.position, rightBall.radius, WHITE);  // right side ball
            } break;

            default:
                break;
        }

        EndDrawing();
    }

    //------------------------------------
    // De-Initialization
    //------------------------------------
    CloseWindow();

    return 0;
}