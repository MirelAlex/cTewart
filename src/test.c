#include "raylib.h"
#include <math.h>

#define MAX_HISTORY 200
#define RECT_WIDTH 600
#define RECT_HEIGHT 200

void DrawScaleLabels(Rectangle plotRect, int interval)
{
    // Calculate scale values
    int middle = RECT_HEIGHT / 2;
    int scale = 0;

    // Draw scale labels and grid lines within the rectangle height
    for (int i = 0; i < 6; i++)
    {
        int posY = plotRect.y + middle - interval * i;
        if (posY >= plotRect.y && posY <= plotRect.y + plotRect.height)
        {
            DrawText(TextFormat("%d", scale), plotRect.x - 50, posY - 10, 10, GRAY);
            DrawLine(plotRect.x, posY, plotRect.x + plotRect.width, posY, Fade(GRAY, 0.3f));
        }
        scale -= 20;
    }

    scale = 20;

    for (int i = 1; i < 6; i++)
    {
        int posY = plotRect.y + middle + interval * i;
        if (posY >= plotRect.y && posY <= plotRect.y + plotRect.height)
        {
            DrawText(TextFormat("%d", scale), plotRect.x - 50, posY - 10, 10, GRAY);
            DrawLine(plotRect.x, posY, plotRect.x + plotRect.width, posY, Fade(GRAY, 0.3f));
        }
        scale += 20;
    }
}

int main(void)
{
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    Rectangle plotRect = {screenWidth / 2 - RECT_WIDTH / 2, screenHeight / 2 - RECT_HEIGHT / 2, RECT_WIDTH, RECT_HEIGHT};

    // Initialize array to store history
    int history[MAX_HISTORY] = {0};
    int historyIndex = 0;

    int interval = RECT_HEIGHT / 5; // Initial interval between scale labels and grid lines

    InitWindow(screenWidth, screenHeight, "Raylib Sine Wave Plot with Movable Rectangle");

    SetTargetFPS(60);

    bool dragging = false;
    Vector2 offset = {0};

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

        if (dragging)
        {
            plotRect.x = GetMouseX() - offset.x;
            plotRect.y = GetMouseY() - offset.y;
        }

        // Check for mouse scroll events to change the scale of the grid
        int scroll = GetMouseWheelMove();
        if (scroll != 0)
        {
            interval += scroll * 10;
            if (interval < 10)
                interval = 10; // Prevent interval from becoming negative or too small
        }

        // Calculate new sine wave value
        float newValue = sinf(GetTime()) * (RECT_HEIGHT / 2);
        float scaleFactor = RECT_HEIGHT / 2.0f / (interval * 10); // Scale factor based on the interval

        // Update history array
        history[historyIndex] = (int)(newValue * scaleFactor);
        historyIndex = (historyIndex + 1) % MAX_HISTORY;

        // Draw
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Draw the movable rectangle
        DrawRectangleLinesEx(plotRect, 2, BLUE);

        // Draw scale labels along y-axis
        DrawScaleLabels(plotRect, interval);

        // Draw the scaled sine wave within the rectangle
        for (int i = 0; i < MAX_HISTORY - 1; i++)
        {
            // Calculate positions of the points
            float x1 = i * (plotRect.width - 1) / (MAX_HISTORY - 1) + plotRect.x + 1;
            float y1 = plotRect.y + RECT_HEIGHT / 2 - history[(historyIndex + i) % MAX_HISTORY];
            float x2 = (i + 1) * (plotRect.width - 1) / (MAX_HISTORY - 1) + plotRect.x + 1;
            float y2 = plotRect.y + RECT_HEIGHT / 2 - history[(historyIndex + i + 1) % MAX_HISTORY];

            // Draw line between points
            DrawLine((int)x1, (int)y1, (int)x2, (int)y2, RED);
        }

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}
