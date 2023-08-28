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

#include <stdlib.h>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
//static void UpdateDrawFrame(void);          // Update and draw one frame

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

    int playerCardsQuantity = 10;
    int playerDeckQuantity = 0;
    // Card array must be dynamic
    Card *playerCards = (Card*)calloc(playerCardsQuantity, sizeof(Card));
    Card *playerDeck = (Card*)malloc(playerDeckQuantity);

    SetupCardArray(playerCards, playerCardsQuantity);
    SortCardArray(playerCards, playerCardsQuantity);
    const float playerYCoordinate = screenHeight - (CARD_HEIGHT + 10.0);
    const float playerYEndpoint = playerYCoordinate + CARD_HEIGHT;
    const float deckYCoordinate = playerYCoordinate - CARD_HEIGHT - 50.0;
    const float deckYEndpoint = deckYCoordinate + CARD_HEIGHT;
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

        // Reminder that you're still within a loop, so don't worry too much about updating the position of cards...
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            // The first card is important because it acts as a positioning determinant for all of the other cards.
            // If no more card is in the player's hand, skip the entire playerCards rendering section.
            if (playerCardsQuantity > 0) {
                playerCards[0].startPoint.x = (screenWidth-(CARD_WIDTH+10.0)*playerCardsQuantity)/2.0;
                playerCards[0].endPoint.x = playerCards[0].startPoint.x + CARD_WIDTH;
                playerCards[0].endPoint.y = playerYEndpoint;
                DrawTexture(playerCards[0].txte, playerCards[0].startPoint.x, playerCards[0].startPoint.y, WHITE);
                if (IsCursorHoverOverCard(&playerCards[0])) {
                    playerCards[0].startPoint.y = playerYCoordinate - 20;
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        AddCardToDeck(&playerDeck, &playerDeckQuantity, playerCards[0]);
                        RemoveCardsAtIndex(&playerCards, &playerCardsQuantity, 0);
                    }
                }
                else playerCards[0].startPoint.y = playerYCoordinate;
                
                for (int i = 1; i < playerCardsQuantity; i++) {
                    playerCards[i].startPoint.x = playerCards[i-1].endPoint.x + 10.0f;
                    playerCards[i].endPoint.x = playerCards[i].startPoint.x + CARD_WIDTH;
                    playerCards[i].endPoint.y = playerYEndpoint;

                    DrawTexture(playerCards[i].txte, playerCards[i].startPoint.x, playerCards[i].startPoint.y, WHITE);
                    if (IsCursorHoverOverCard(&playerCards[i])) {
                        playerCards[i].startPoint.y = playerYCoordinate - 20;
                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                            AddCardToDeck(&playerDeck, &playerDeckQuantity, playerCards[i]);
                            RemoveCardsAtIndex(&playerCards, &playerCardsQuantity, i);
                        }
                    }
                    else playerCards[i].startPoint.y = playerYCoordinate;
                }
            }
            // End playerCards rendering section

            // Start playerDeck (played cards) rendering section
            for (int i = 0, x = screenWidth - 500; i < playerDeckQuantity; i++) {
                DrawTexture(playerDeck[i].txte, x -= CARD_WIDTH/2, deckYCoordinate, WHITE);
            }
            
        }
        EndDrawing();
    }
#endif

    // Free everything here
    
    free(playerCards);
    free(playerDeck);

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();                  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

// Update and draw game frame
/*static void UpdateDrawFrame(void)
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
}*/
