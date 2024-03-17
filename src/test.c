#include "raylib.h"
#include <math.h>

#define MAX_HISTORY 100
#define RECT_WIDTH 800
#define RECT_HEIGHT 200

void DrawScaleLabels(Rectangle plotRect, int interval)
{
    // Calculate scale values
    int middle = RECT_HEIGHT / 2;
    int scale = 0;
    int sc = 20;
    // if      (interval > 60) { sc = 1;}
    // else if (interval > 50) { sc = 5;}
    // else if (interval > 40) { sc = 10;}
    // else                    { sc = 20;}

    // Draw scale labels and grid lines within the rectangle height
    for (int i = 0; i < 12; i++)
    {
        int posY = plotRect.y + middle - interval * i;
        if (posY >= plotRect.y && posY <= plotRect.y + plotRect.height)
        {
            DrawText(TextFormat("%d", scale), plotRect.x - 50, posY - 10, 10, GRAY);
            DrawLine(plotRect.x, posY, plotRect.x + plotRect.width, posY, Fade(GRAY, 0.3f));
        }
        scale += sc;
    }

    scale = -sc;

    for (int i = 1; i < 12; i++)
    {
        int posY = plotRect.y + middle + interval * i;
        if (posY >= plotRect.y && posY <= plotRect.y + plotRect.height)
        {
            DrawText(TextFormat("%d", scale), plotRect.x - 50, posY - 10, 10, GRAY);
            DrawLine(plotRect.x, posY, plotRect.x + plotRect.width, posY, Fade(GRAY, 0.3f));
        }
        scale -= sc;
    }
}

int main(void)
{
    // Initialization
    const int screenWidth = 16*80;
    const int screenHeight = 9*80;

    Rectangle plotRect = {screenWidth / 2 - RECT_WIDTH / 2, screenHeight / 2 - RECT_HEIGHT / 2, RECT_WIDTH, RECT_HEIGHT};

    // Initialize array to store history
    int history[MAX_HISTORY] = {0};
    int historyIndex = 0;

    int interval = RECT_HEIGHT / 5; // Initial interval between scale labels and grid lines

    InitWindow(screenWidth, screenHeight, "Raylib Sine Wave Plot with Movable Rectangle");

    SetTargetFPS(60);

    bool dragging = false;
    bool pause = false;
    Vector2 offset = {0};
    float time = 0.0f;
    while (!WindowShouldClose()) // Main loop
    {
            // Update
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                Vector2 mousePos = GetMousePosition();
                if (CheckCollisionPointRec(mousePos, plotRect))
                {
                    dragging = true;
                    offset.x = mousePos.x - plotRect.x;
                    offset.y = mousePos.y - plotRect.y;
                }
            }
            else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                dragging = false;
            }

            if (IsKeyPressed(KEY_SPACE)){pause = !pause;}
            if (dragging)
            {
                plotRect.x = GetMouseX() - offset.x;
                plotRect.y = GetMouseY() - offset.y;
            }

            // Check for mouse scroll events to change the scale of the grid
            int scroll = GetMouseWheelMove();
            if (scroll != 0)
            {
                interval += scroll * 5;
                if (interval < 10)
                    interval = 10; // Prevent interval from becoming negative or too small
            }

            DrawText(TextFormat("interval = %d", interval), 20, 20, 10, GRAY);
            DrawText(TextFormat("no of intervals = %d", RECT_HEIGHT/interval), 150, 20, 10, GRAY);
            DrawText(TextFormat("scroll = %d", scroll), 20, 40, 10, GRAY);

            // Calculate new sine wave value
            float newValue;
            if (sinf(time*8) > 0 )
            {
                newValue = 50;
            }else{
                newValue = -50;
            }
            DrawText(TextFormat("newValue = %f", newValue),20, screenHeight - 20, 10, GRAY);
            // float scaleFactor = RECT_HEIGHT / 2.0f / (interval * 10); // Scale factor based on the interval

            // Update history array
            if (!pause)
            {
                // only when space is pressed
                history[historyIndex] = (int)(newValue);
                historyIndex = (historyIndex + 1) % MAX_HISTORY;
            }

            // Draw
            BeginDrawing();
                ClearBackground(RAYWHITE);
                // Draw the movable rectangle
                DrawRectangleLinesEx(plotRect, 2, BLUE);
                // Draw scale labels along y-axis
                DrawScaleLabels(plotRect, interval);
                // Draw the scaled sine wave within the rectangle
                float x1, y1;
                float x2, y2 ;
                for (int i = 0; i < MAX_HISTORY - 1; i++)
                {
                    // Calculate positions of the points
                    x1 = i * (plotRect.width - 1) / (MAX_HISTORY - 1) + plotRect.x + 1;
                    y1 = plotRect.y + RECT_HEIGHT / 2 - history[(historyIndex + i) % MAX_HISTORY];
                    x2 = (i + 1) * (plotRect.width - 1) / (MAX_HISTORY - 1) + plotRect.x + 1;
                    y2 = plotRect.y + RECT_HEIGHT / 2 - history[(historyIndex + i + 1) % MAX_HISTORY];
                    // Draw line between points
                    DrawLine((int)x1, (int)y1, (int)x2, (int)y2, RED);
                    // Show buffer values down as text
                    if (i%4 == 0)
                    {
                        history[(historyIndex + i) % MAX_HISTORY] > 0 ?
                        DrawText(TextFormat("%d", history[(historyIndex + i) % MAX_HISTORY]),(int) x1, plotRect.y + RECT_HEIGHT, 5, GREEN) :
                        DrawText(TextFormat("%d", history[(historyIndex + i) % MAX_HISTORY]),(int) x1, plotRect.y + RECT_HEIGHT, 5, RED);
                        // indexes
                        DrawText(TextFormat("%d", historyIndex + i),(int) x1, plotRect.y + RECT_HEIGHT + 20, 5, GRAY);
                        if (i==0){DrawText("^ historyIndex", (int) x1, plotRect.y + RECT_HEIGHT + 40, 5, GRAY);}

                        // values at indexes
                        // DrawText(TextFormat("%d", historyIndex + i),(int) x1, plotRect.y + RECT_HEIGHT + 40, 5, GRAY);
                    }

                }
                DrawText(TextFormat("%d", history[0]),(int) x1 + 20, (int)y1, 10, GRAY);



            EndDrawing();
            if (!pause)
            {
                // only when space is pressed
                time += 0.01;
            }
    }

    // De-Initialization
    CloseWindow();

    return 0;
}
