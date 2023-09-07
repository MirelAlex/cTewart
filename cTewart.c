/*******************************************************************************************
 *
 *   raylib [shapes] example - bouncing ball
 *
 *   Example originally created with raylib 2.5, last time updated with raylib 2.5
 *
 *   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
 *   BSD-like license that allows static linking with closed source software
 *
 *   Copyright (c) 2013-2023 Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/

#include <stdio.h>
#include "raylib.h"

#include <time.h> // Required for: time_t, tm, time(), localtime(), strftime()

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

// Custom logging function
void CustomLog(int msgType, const char *text, va_list args)
{
    char timeStr[64] = {0};
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("[%s] ", timeStr);

    switch (msgType)
    {
    case LOG_INFO:
        printf("[INFO] : ");
        break;
    case LOG_ERROR:
        printf("[ERROR]: ");
        break;
    case LOG_WARNING:
        printf("[WARN] : ");
        break;
    case LOG_DEBUG:
        printf("[DEBUG]: ");
        break;
    default:
        break;
    }

    vprintf(text, args);
    printf("\n");
}
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //---------------------------------------------------------

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "cTewart");

    Vector2 ballPosition = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    Vector2 ballSpeed = {0.0f, 4.0f};
    int ballRadius = 20;

    bool pause = 0;
    int framesCounter = 0;

    // Set custom logger
    SetTraceLogCallback(CustomLog);
    SetTraceLogLevel(LOG_ALL);
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //----------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //-----------------------------------------------------
        if (IsKeyPressed(KEY_SPACE))
            pause = !pause;

        if (!pause)
        {
            ballPosition.x += ballSpeed.x;
            ballPosition.y += ballSpeed.y;

            // Check walls collision for bouncing
            if ((ballPosition.x >= (GetScreenWidth() - ballRadius)) || (ballPosition.x <= ballRadius))
                ballSpeed.x *= -1.0f;
            if ((ballPosition.y >= (GetScreenHeight() - ballRadius)) || (ballPosition.y <= ballRadius))
                ballSpeed.y *= -1.0f;
        }
        else
            framesCounter++;
        //-----------------------------------------------------

        // Draw
        //-----------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawCircleV(ballPosition, (float)ballRadius, MAROON);
        DrawText("PRESS SPACE to PAUSE BALL MOVEMENT", 10, GetScreenHeight() - 25, 20, LIGHTGRAY);
        char xstr[20];
        char ystr[20];
        sprintf(xstr, "x: %.2f", ballPosition.x);
        sprintf(ystr, "y: %.2f", ballPosition.y);
        DrawText(xstr, 10, 50, 20, BLACK);
        DrawText(ystr, 10, 80, 20, BLACK);
        char xdotstr[20];
        char ydotstr[20];
        sprintf(xdotstr, "x.: %.2f", ballSpeed.x);
        sprintf(ydotstr, "y.: %.2f", ballSpeed.y);
        DrawText(xdotstr, 10, 110, 20, BLACK);
        DrawText(ydotstr, 10, 140, 20, BLACK);

        // On pause, we draw a blinking message
        if (pause && ((framesCounter / 30) % 2))
        {
            DrawText("PAUSED", 350, 200, 30, GRAY);
            TraceLog(LOG_INFO, "paused");
        }
        DrawFPS(10, 10);

        EndDrawing();
        //-----------------------------------------------------
    }

    // De-Initialization
    //---------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //----------------------------------------------------------

    return 0;
}