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

    // Define the camera to look into our 3d world
    Camera camera = {0};
    camera.position = (Vector3){10.0f, 10.0f, 10.0f}; // Camera position
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};      // Camera looking at point
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                              // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;           // Camera projection type

    Vector3 cubePosition = {0.0f, 0.0f, 0.0f};
    Vector2 cubeScreenPosition = {0.0f, 0.0f};

    DisableCursor(); // Limit cursor to relative movement inside the window

    // Set custom logger
    SetTraceLogCallback(CustomLog);
    SetTraceLogLevel(LOG_ALL);
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //----------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera, CAMERA_THIRD_PERSON);

        // Calculate cube screen space position (with a little offset to be in top)
        cubeScreenPosition = GetWorldToScreen((Vector3){cubePosition.x, cubePosition.y + 2.5f, cubePosition.z}, camera);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
        DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);

        DrawGrid(10, 1.0f);
        // TraceLog(LOG_INFO, "paused");

        EndMode3D();

        DrawText("Enemy: 100 / 100", (int)cubeScreenPosition.x - MeasureText("Enemy: 100/100", 20) / 2, (int)cubeScreenPosition.y, 20, BLACK);

        DrawText(TextFormat("Cube position in screen space coordinates: [%i, %i]", (int)cubeScreenPosition.x, (int)cubeScreenPosition.y), 10, 10, 20, LIME);
        DrawText("Text 2d should be always on top of the cube", 10, 40, 20, GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //---------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //----------------------------------------------------------

    return 0;
}