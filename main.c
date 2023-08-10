/*******************************************************************************************
*
*   raylib [core] example - Basic 3d example
*
*   Welcome to raylib!
*
*   To compile example, just press F5.
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2023 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "BladeCards.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void);          // Update and draw one frame

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "raylib");
    CardsInit();

    one.startPoint = (Vector2){10.0f, 10.0f};
    two.startPoint = (Vector2){200.0f, 10.0f};

    /*Card one;
    one.img = LoadImage("./img/1.png");
    ImageResize(&one.img, 180, 240);
    Texture2D oneTxte = LoadTextureFromImage(one.img);
    UnloadImage(one.img);
    one.startPoint = (Vector2){10.0f, 10.0f};

    Card two;
    two.img = LoadImage("./img/2.png");
    ImageResize(&two.img, 180, 240);
    Texture2D twoTxte = LoadTextureFromImage(two.img);
    UnloadImage(two.img);
    two.startPoint = (Vector2){200.0f, 10.0f};*/
    //--------------------------------------------------------------------------------------

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        //UpdateDrawFrame();
        Vector2 mouseXY = GetMousePosition();
        if (mouseXY.x < one.startPoint.x + 180 && mouseXY.y < one.startPoint.y + 240) one.startPoint.y = 50.0f;
        else one.startPoint.y = 10.0f;

        if (mouseXY.x < two.startPoint.x + 180 && mouseXY.y < two.startPoint.y + 240 && mouseXY.x > one.startPoint.x + 180) two.startPoint.y = 50.0f;
        else two.startPoint.y = 10.0f;

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            DrawTexture(one.txte, one.startPoint.x, one.startPoint.y, WHITE);
            DrawTexture(two.txte, two.startPoint.x, two.startPoint.y, WHITE);
        }
        EndDrawing();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();                  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

// Update and draw game frame
static void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------
    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("This is a raylib example", 10, 40, 20, DARKGRAY);
        DrawFPS(10, 10);

    EndDrawing();
    //----------------------------------------------------------------------------------
}
