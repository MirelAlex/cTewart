#include "raylib.h"
#include <math.h>

#define MAX_HISTORY 100
#define RECT_WIDTH 300
#define RECT_HEIGHT 200

typedef struct {
    Rectangle rect;
    int history[MAX_HISTORY];
    float buffer[MAX_HISTORY]; // Store unaltered input values
    int historyIndex;
    int interval;
    float zoom;
} PlotWidget;

void InitializePlotWidget(PlotWidget* widget, Rectangle rect) {
    widget->rect = rect;
    widget->historyIndex = 0;
    widget->interval = RECT_HEIGHT / 5; // Initial interval between scale labels and grid lines
    widget->zoom = 1.0f; // Initial zoom level
}

void DrawScaleLabels(Rectangle plotRect, int interval, float zoom)
{
    // Calculate scale values
    int middle = RECT_HEIGHT / 2;
    int scale = 0;

    // Adjust interval based on zoom level
    int zoomedInterval = interval / zoom;

    // Draw scale labels and grid lines within the rectangle height
    for (int i = 0; i < 6; i++)
    {
        int posY = plotRect.y + middle - zoomedInterval * i;
        if (posY >= plotRect.y && posY <= plotRect.y + plotRect.height)
        {
            DrawText(TextFormat("%d", scale), plotRect.x - 50, posY - 10, 10, GRAY);
            DrawLine(plotRect.x, posY, plotRect.x + plotRect.width, posY, Fade(GRAY, 0.3f));
        }
        scale += 20;
    }

    scale = -20;

    for (int i = 1; i < 6; i++)
    {
        int posY = plotRect.y + middle + zoomedInterval * i;
        if (posY >= plotRect.y && posY <= plotRect.y + plotRect.height)
        {
            DrawText(TextFormat("%d", scale), plotRect.x - 50, posY - 10, 10, GRAY);
            DrawLine(plotRect.x, posY, plotRect.x + plotRect.width, posY, Fade(GRAY, 0.3f));
        }
        scale -= 20;
    }
}

void UpdatePlotWidget(PlotWidget* widget, float inputValue) {
    // Store unaltered input value in buffer
    widget->buffer[widget->historyIndex] = inputValue;

    // Calculate new sine wave value
    float newValuePlot = sinf(inputValue) * (RECT_HEIGHT / 2) / widget->zoom;

    // Update history array
    widget->history[widget->historyIndex] = (int)newValuePlot;
    widget->historyIndex = (widget->historyIndex + 1) % MAX_HISTORY;

    // Rescale history values with the new zoom level FAIL
    // for (int i = widget->historyIndex; i < MAX_HISTORY; i++) {
    //     if (widget->zoom != 0)
    //     {
    //         /* code */
    //         widget->history[i] = (int)(widget->history[i] / widget->zoom);
    //     }

    // }
}

void DrawPlotWidget(PlotWidget* widget) {
    // Draw the movable rectangle
    DrawRectangleLinesEx(widget->rect, 2, BLUE);

    // Draw scale labels along y-axis
    DrawScaleLabels(widget->rect, widget->interval, widget->zoom);

    // Draw the scaled sine wave within the rectangle
    for (int i = 0; i < MAX_HISTORY - 1; i++)
    {
        // Calculate positions of the points
        float x1 = i * (widget->rect.width - 1) / (MAX_HISTORY - 1) + widget->rect.x + 1;
        float y1 = widget->rect.y + RECT_HEIGHT / 2 - widget->history[(widget->historyIndex + i) % MAX_HISTORY];
        float x2 = (i + 1) * (widget->rect.width - 1) / (MAX_HISTORY - 1) + widget->rect.x + 1;
        float y2 = widget->rect.y + RECT_HEIGHT / 2 - widget->history[(widget->historyIndex + i + 1) % MAX_HISTORY];

        // Draw line between points
        if ((y1 > widget->rect.y) && (y2 > widget->rect.y) &&
            (y1 < widget->rect.y+RECT_HEIGHT) && (y2 < widget->rect.y+RECT_HEIGHT))
        {
            DrawLine((int)x1, (int)y1, (int)x2, (int)y2, RED);
        }
    }
}

int main(void)
{
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    Rectangle plotRect = {screenWidth / 2 - RECT_WIDTH / 2, screenHeight / 2 - RECT_HEIGHT / 2, RECT_WIDTH, RECT_HEIGHT};
    Rectangle plotRect1 = {screenWidth / 4 - RECT_WIDTH / 2, screenHeight / 4 - RECT_HEIGHT / 2, RECT_WIDTH, RECT_HEIGHT};

    PlotWidget widget;
    PlotWidget widget1;
    InitializePlotWidget(&widget, plotRect);
    InitializePlotWidget(&widget1, plotRect1);

    InitWindow(screenWidth, screenHeight, "Raylib Sine Wave Plot with Movable Rectangle");

    SetTargetFPS(60);

    bool dragging = false;
    Vector2 offset = {0};

    while (!WindowShouldClose()) // Main loop
    {
        // Update
        Vector2 mousePos = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (CheckCollisionPointRec(mousePos, widget.rect))
            {
                dragging = true;
                offset.x = mousePos.x - widget.rect.x;
                offset.y = mousePos.y - widget.rect.y;
            }
        }
        else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            dragging = false;
        }

        if (dragging)
        {
            widget.rect.x = GetMouseX() - offset.x;
            widget.rect.y = GetMouseY() - offset.y;
        }

        // Check for mouse scroll events to change the zoom level
        int scroll = GetMouseWheelMove();

        if (CheckCollisionPointRec(mousePos, widget.rect) && scroll != 0)
        {
            widget.zoom += scroll * 0.1f; // Adjust zoom level
            if (widget.zoom < 0.1f)
                widget.zoom = 0.1f; // Prevent zoom level from becoming too small
        }

        // Update the widget with new input value
        float inputValue = GetTime(); //
        UpdatePlotWidget(&widget, inputValue);
        UpdatePlotWidget(&widget1, inputValue);

        // Draw
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Draw the plot widget
        DrawPlotWidget(&widget);
        DrawPlotWidget(&widget1);

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}
