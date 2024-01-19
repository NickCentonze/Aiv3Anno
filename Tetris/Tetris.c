#include "raylib.h"
#include <time.h>
#include <string.h>
#include "Tetris.h"
#include <stdio.h>
#include <string.h>
#include "Tetris_Inizialization.h"
#include "draw_reset_manager.h"

void drawTetromino(const Color currentColor, const int startOffsetX, const int startOffsetY, const int tetrominoStartX, const int tetrominoStartY, const int *tetromino)
{
    for(int y = 0; y < TETROMINO_SIZE; y++)
    {
        for(int x = 0; x < TETROMINO_SIZE; x++)
        {
            const int offset = y * TETROMINO_SIZE + x;

            if(tetromino[offset] == 1)
            {
                DrawRectangle((x + tetrominoStartX) * TILE_SIZE + startOffsetX, (y + tetrominoStartY) * TILE_SIZE + startOffsetY, TILE_SIZE, TILE_SIZE, currentColor);
            }
        }
    }
}

int main(int argc, char** argv, char** environ)
{
    const int windowWidth = 600; 
    const int windowHeight = 700; 

    const int startOffsetX = (windowWidth / 2) - ((STAGE_WIDTH * TILE_SIZE) / 2);
    const int startOffsetY = (windowHeight / 2) - ((STAGE_HEIGHT * TILE_SIZE) / 2);

    const int tetrominoStartX = STAGE_WIDTH / 2;
    const int tetrominoStartY = 0;

    int currentTetrominoX = tetrominoStartX;
    int currentTetrominoY = tetrominoStartY;

    time_t unixTime;

    time(&unixTime);

    SetRandomSeed(unixTime);

    int currentTetrominoType = GetRandomValue(0, 6);
    int currentRotation = 0;

    const float moveTetrominoDownTimer = 1.f;
    float timeToMoveTetrominoDown = moveTetrominoDownTimer;
    int currentColor = GetRandomValue(0, 7);

    TraceLog(LOG_INFO, "Number of arguments : %d", argc);

    for(int i = 0; i < argc; i++)
    {
        TraceLog(LOG_INFO, "Argument : %d = %s", i, argv[i]);
    }

    while(*environ != 0)
    {
        TraceLog(LOG_INFO, "Argument : %s", *environ);
        environ++;
    }

    InitWindow(windowWidth, windowHeight, "Title");
    InitAudioDevice();

    Font font = LoadFontEx("resources/Tetris.ttf", 64 , 0 , 0);
    Music tetris_bgMusic = LoadMusicStream("resources/tetrisMusic.wav");
    float timePlayed = 0;
    SetMusicVolume(tetris_bgMusic, 0.05);
    PlayMusicStream(tetris_bgMusic);

    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        //Music Management
        UpdateMusicStream(tetris_bgMusic);

        timePlayed = GetMusicTimePlayed(tetris_bgMusic) / GetMusicTimeLength(tetris_bgMusic);
        if (timePlayed > 1)
        {
            timePlayed = 1;
        }
        if (!IsMusicStreamPlaying(tetris_bgMusic))
        {
           StopMusicStream(tetris_bgMusic);
           PlayMusicStream(tetris_bgMusic);
        }
        
        //Score drawing Management

        DrawTextEx(font, "Score", (Vector2){450, 10}, 40, 2, WHITE);
        char scoreText[10];
        sprintf(scoreText, "%d", Score);
        Vector2 textSize = MeasureTextEx(font, scoreText, 40, 2);
        DrawTextEx(font, scoreText, (Vector2){499, 60}, 40, 2, BLACK);


        //GameSpeed on track
        DrawTextEx(font, "GameSpeed", (Vector2){10, 10}, 40, 2, WHITE);
        char globalSpeed[10];
        sprintf(globalSpeed, "%0.1f", GlobalSpeed);
        Vector2 textSizeSpeed = MeasureTextEx(font, globalSpeed, 40, 2);
        DrawTextEx(font, globalSpeed, (Vector2){10, 60}, 40, 2, BLACK);

        //timeToMoveTetrominoDown - GlobalSpeed;
        timeToMoveTetrominoDown -= GetFrameTime();

        if (IsKeyPressed(KEY_SPACE))
        {
            const int lastRotation = currentRotation;

            currentRotation++;

            if (currentRotation > 3)
            {
                currentRotation = 0;
            }

            if (CheckCollision(currentTetrominoX,currentTetrominoY,tetrominoTypes[currentTetrominoType][currentRotation]))
            {
                currentRotation = lastRotation;
            }
        }

        if (IsKeyPressed(KEY_RIGHT))
        {
            // No need to check overflow, wall is your protector
            if (!CheckCollision(currentTetrominoX+1,currentTetrominoY,tetrominoTypes[currentTetrominoType][currentRotation]))
            {
                currentTetrominoX++;
            }
        }
        if (IsKeyPressed(KEY_LEFT))
        {
            // No need to check overflow, wall is your protector
            if (!CheckCollision(currentTetrominoX-1,currentTetrominoY,tetrominoTypes[currentTetrominoType][currentRotation]))
            {
                currentTetrominoX--;
            }
        }


        if(timeToMoveTetrominoDown-GlobalSpeed <= 0 || IsKeyPressed(KEY_DOWN))
        {            
            
            if(!CheckCollision(currentTetrominoX,currentTetrominoY+1,tetrominoTypes[currentTetrominoType][currentRotation]))
            {  
                currentTetrominoY++;
                timeToMoveTetrominoDown = moveTetrominoDownTimer;
                UpdateScore(0,1);
            }

            else
            {
                // const int currentTetrominoOffset = currentTetrominoY * STAGE_WIDTH + currentTetrominoX;
                
                // stage[currentTetrominoOffset] = 1;

                for(int y = 0; y < TETROMINO_SIZE; y++)
                {
                    for(int x = 0; x < TETROMINO_SIZE; x++)
                    {
                        const int offset = y * TETROMINO_SIZE + x;

                        const int *tetromino = tetrominoTypes[currentTetrominoType][currentRotation];

                        if(tetromino[offset] == 1)
                        {
                            const int offset_stage = (y + currentTetrominoY) * STAGE_WIDTH + (x + currentTetrominoX);

                            stage[offset_stage] = currentColor+1;
                        }
                    }
                }

                DeleteLines();

                currentTetrominoX = tetrominoStartX;
                currentTetrominoY = tetrominoStartY;

                currentTetrominoType = GetRandomValue(0, 6);
                currentRotation = 0;
                currentColor = GetRandomValue(0, 7);
            }
        }

        BeginDrawing();
        ClearBackground(GRAY);

        for(int y = 0; y < STAGE_HEIGHT; y++)
        {
            for(int x = 0; x < STAGE_WIDTH; x++)
            {
                const int offset = y * STAGE_WIDTH + x;
                const int color = stage[offset];

                if(stage[offset] != 0)
                {
                    DrawRectangle(x * TILE_SIZE + startOffsetX, y * TILE_SIZE + startOffsetY, TILE_SIZE, TILE_SIZE, colorTypes[color-1]);
                }

                DrawRectangleLines(x * TILE_SIZE + startOffsetX, y * TILE_SIZE + startOffsetY, TILE_SIZE, TILE_SIZE, BLACK);
            }
        }
        
        drawTetromino(colorTypes[currentColor],startOffsetX, startOffsetY, currentTetrominoX, currentTetrominoY, tetrominoTypes[currentTetrominoType][currentRotation]);

        

        EndDrawing();
    }

    UnloadMusicStream(tetris_bgMusic);

    CloseAudioDevice();

    CloseWindow();

    return 0;
}