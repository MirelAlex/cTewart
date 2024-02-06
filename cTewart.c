#include <stdio.h>
#include "raylib.h"

#include <time.h> // Required for: time_t, tm, time(), localtime(), strftime()
#include <math.h>

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
    static float pct = 0.01f;
    // Initialization
    //---------------------------------------------------------

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Stewart Platform");

    // Define the camera to look into our 3D world
    Camera camera = {0};
    camera.position = (Vector3){10.0f, 10.0f, 10.0f}; // Camera position
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};      // Camera looking at point
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                              // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;           // Camera projection type

    Vector3 basePlatform[6]; // Array to store the positions of the six vertices of the base hexagonal platform
    Vector3 upperPlatform[6]; // Array to store the positions of the six vertices of the upper hexagonal platform
    Vector3 actuators[6]; // Array to store the positions of the six actuators

    for (int i = 0; i < 6; ++i)
    {
        float angle = 2.0f * PI * i / 6.0f;
        basePlatform[i] = (Vector3){4.0f * cos(angle), 0, 4.0f * sin(angle)}; // Lower the base platform to make it thicker
        upperPlatform[i] = basePlatform[i];
        upperPlatform[i].y += 3.0f; // Initial height of the upper platform
        actuators[i] = basePlatform[i];
        actuators[i].y += 1.5f; // Adjust height of actuators
    }

    DisableCursor(); // Limit cursor to relative movement inside the window

    SetTraceLogLevel(LOG_NONE);
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //----------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera, CAMERA_THIRD_PERSON);

        // Move the upper platform (for example, oscillation)
        float oscillationHeight = 1.0f * sin(GetTime()); // Adjust the amplitude and frequency as needed
        for (int i = 0; i < 6; ++i)
        {
            upperPlatform[i].y = basePlatform[i].y + 3.0f + oscillationHeight;
        }

        // Update actuators' positions based on the movement of the upper platform
        for (int i = 0; i < 6; ++i)
        {
            actuators[i].y = upperPlatform[i].y - 1.5f; // Connect the actuators to the upper platform
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        // Draw the base hexagonal platform using lines
        for (int i = 0; i < 6; ++i)
        {
            int nextIndex = (i + 1) % 6;
            DrawLine3D(basePlatform[i], basePlatform[nextIndex], BLUE);
        }

        // Draw the upper hexagonal platform using lines
        for (int i = 0; i < 6; ++i)
        {
            int nextIndex = (i + 1) % 6;
            DrawLine3D(upperPlatform[i], upperPlatform[nextIndex], GREEN);
        }
        // float y = (exp(sin(2 * PI * pct) - 1)) / (pow(exp(1),2) - 1);
        // Vector3 breath_pos = (Vector3){0.0f, y * 10, 0.0f};
        // PLAT.pos = Vector3Add(THIS.T0, breath_pos);
        pct += 0.005f;
        if (pct > 1.0f) {pct=0.0f;};

        // Draw the actuators connected to the base and upper platforms using lines
        for (int i = 0; i < 6; ++i)
        {
            DrawLine3D(basePlatform[i], actuators[i], RED); // Connect the actuators to the base
            DrawLine3D(upperPlatform[i], actuators[i], RED); // Connect the actuators to the upper platform
        }

        DrawGrid(10, 1.0f);

        EndMode3D();

        DrawText("Stewart Platform Simulation", 10, 10, 20, BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //---------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //----------------------------------------------------------

    return 0;
}