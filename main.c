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
#include "GameFunctions.h"

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
    // Must initialise cards before use
    CardsInit();

    int cardsQuantity = 10;
    Card playerCards[cardsQuantity];
    SetupCardArray(playerCards, cardsQuantity);
    SortCardArray(playerCards, cardsQuantity);
    const float playerYCoordinate = 10.0f;
    const float playerYEndpoint = playerYCoordinate + CARD_HEIGHT;

    /*Card ichi = one;
    Card ni = two;

    ichi.startPoint = (Vector2){10.0f, 10.0f};
    ni.startPoint = (Vector2){200.0f, 10.0f};*/
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
        /*if (mouseXY.x < ichi.startPoint.x + 180 && mouseXY.y < ichi.startPoint.y + 240) ichi.startPoint.y = 50.0f;
        else ichi.startPoint.y = 10.0f;

        if (mouseXY.x < ni.startPoint.x + 180 && mouseXY.y < ni.startPoint.y + 240 && mouseXY.x > ichi.startPoint.x + 180) ni.startPoint.y = 50.0f;
        else ni.startPoint.y = 10.0f;*/

        // Reminder that you're still within a loop, so don't worry too much about updating the position of cards...
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            /*DrawTexture(ichi.txte, ichi.startPoint.x, ichi.startPoint.y, WHITE);
            DrawTexture(ni.txte, ni.startPoint.x, ni.startPoint.y, WHITE);*/
            // DrawTextureRec(playerCards[0].txte, cardRec, (Vector2){10, 10}, WHITE);
            playerCards[0].startPoint.x = 10.0f;
            playerCards[0].endPoint.x = playerCards[0].startPoint.x + CARD_WIDTH;
            playerCards[0].endPoint.y = playerYEndpoint;
            if (IsCursorHoverOverCard(&playerCards[0])) playerCards[0].startPoint.y = playerYCoordinate + 20;
            else playerCards[0].startPoint.y = playerYCoordinate;
            DrawTexture(playerCards[0].txte, playerCards[0].startPoint.x, playerCards[0].startPoint.y, WHITE);
            
            for (int i = 1; i < cardsQuantity; i++) {
                playerCards[i].startPoint.x = playerCards[i-1].endPoint.x + 10.0f;
                playerCards[i].endPoint.x = playerCards[i].startPoint.x + CARD_WIDTH;
                playerCards[i].endPoint.y = playerYEndpoint;
                if (IsCursorHoverOverCard(&playerCards[i])) playerCards[i].startPoint.y = playerYCoordinate + 20;
                else playerCards[i].startPoint.y = playerYCoordinate;
                DrawTexture(playerCards[i].txte, playerCards[i].startPoint.x, playerCards[i].startPoint.y, WHITE);
                //playerXCoordinate += (float)CARD_WIDTH;
            }
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
