#include "Tetris.h"
#include <string.h>

extern int stage[];
extern int Score;
extern float GlobalSpeed;

void ResetLines(int startLineY)
{
    for (int y = startLineY; y >= 0; y--)
    {
        for (int x = 1; x < STAGE_WIDTH - 1; x++)
        {
            const int offset = y * STAGE_WIDTH + x;
            const int offset_below = (y+1) * STAGE_WIDTH + x;

            if (stage[offset_below] == 0 && stage[offset] > 0)
            {
                stage[offset_below] = stage[offset];
                stage[offset] = 0;
            }
        }
    }   
}

void UpdateScore(int rowsDone, int moveDownPoints)
{
    switch (rowsDone)
    {
    case 1:
        Score += 100;
        break;

    default: 
        break;
    }

    Score += moveDownPoints; 
}



void DeleteLines()
{

    int rowsCleared = 0;

    for (int y = 0; y < STAGE_HEIGHT - 1; y++)
    {
        int checkLine = 1;

        for (int x = 1; x < STAGE_WIDTH - 1; x++)
        {
            const int offset = y * STAGE_WIDTH + x;

            if (stage[offset] == 0)
            {
                checkLine = 0;
                break;
            }
        }

        if(checkLine)
        {
            const int offset = y * STAGE_WIDTH + 1;
            memset(stage+offset,0,(STAGE_WIDTH-2)* sizeof(int));
            rowsCleared++;
            ResetLines(y);

            if (GlobalSpeed >= 0.9f)
            {
                GlobalSpeed=0.9f;
            }
            else
            {
                GlobalSpeed+=0.1f;
            }
            
        }
            UpdateScore(rowsCleared, 0);
    }   
}