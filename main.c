/*******************************************************************************************
*
*   To compile the game, just press F5.
*   Note that compiled executable is placed in the same folder as .c file
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
void PrepareCards(Card *cardArr, int sz);

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1600, screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "Blade");
    // Must initialise cards before use
    CardsInit();

    int buf[10];
    int playerCardsQuantity = 10, opponentCardsQuantity = 10;
    int playerDeckQuantity = 1, opponentDeckQuantity = 1;
    int playerVal = 0, opponentVal = 0;
    // Card array must be dynamic
    Card *playerCards = (Card*)calloc(playerCardsQuantity, sizeof(Card));
    PrepareCards(playerCards, playerCardsQuantity);
    Card *playerDeck = (Card*)calloc(playerDeckQuantity, sizeof(Card));
    *playerDeck = GetRandomCard(buf);

    Card *opponentCards = (Card*)calloc(opponentCardsQuantity, sizeof(Card));
    PrepareCards(opponentCards, opponentCardsQuantity);
    Card *opponentDeck = (Card*)calloc(opponentDeckQuantity, sizeof(Card));
    *opponentDeck = GetRandomCard(buf);

    const float playerYCoordinate = screenHeight - (CARD_HEIGHT + 10.0), opponentYCoordinate = 10.0;
    const float playerYEndpoint = playerYCoordinate + CARD_HEIGHT, opponentYEndpoint = opponentYCoordinate + CARD_HEIGHT;
    const float playerDeckYCoordinate = playerYCoordinate - CARD_HEIGHT - 50.0;
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

            // Alas, begin rendering playerCards...

            if (playerCardsQuantity > 0) {
                playerCards[0].startPoint.x = (screenWidth-(CARD_WIDTH+10.0)*playerCardsQuantity)/2.0;
                playerCards[0].endPoint.x = playerCards[0].startPoint.x + CARD_WIDTH;
                playerCards[0].endPoint.y = playerYEndpoint;
                DrawTexture(playerCards[0].txte, playerCards[0].startPoint.x, playerCards[0].startPoint.y, WHITE);
                if (IsCursorHoverOverCard(&playerCards[0])) {
                    playerCards[0].startPoint.y = playerYCoordinate - 20;
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        if (playerCards[0].effect == NONE || playerCards[0].effect == REVIVE)
                            AddCardToDeck(&playerDeck, &playerDeckQuantity, playerCards[0]);
                        RemoveCardAtIndex(&playerCards, &playerCardsQuantity, 0);
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
                            if (playerCards[i].effect == NONE || playerCards[i].effect == REVIVE)
                                AddCardToDeck(&playerDeck, &playerDeckQuantity, playerCards[i]);
                            RemoveCardAtIndex(&playerCards, &playerCardsQuantity, i);
                        }
                    }
                    else playerCards[i].startPoint.y = playerYCoordinate;
                }
            }

            // End playerCards rendering section
            // Start opponentCards rendering section



            // End opponentCards rendering section
            // Start playerDeck (played cards) rendering section
            int val = 0;

            for (int i = 0, x = screenWidth - 200; i < playerDeckQuantity; i++) {
                DrawTexture(playerDeck[i].txte, x -= CARD_WIDTH/2, playerDeckYCoordinate, WHITE);
                val += playerDeck[i].value;
            }
            playerVal = val;
            char playerDeckValue[3];
            itoa(playerVal, playerDeckValue, 10);
            DrawText("Score", 150, playerDeckYCoordinate, 75, DARKGRAY);
            DrawText(playerDeckValue, 150, playerDeckYCoordinate + CARD_HEIGHT/2, 75, DARKGRAY);

        }
        EndDrawing();
    }
#endif

    // Free everything here
    
    free(playerCards);
    free(playerDeck);
    free(opponentCards);
    free(opponentDeck);

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();                  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void PrepareCards(Card *cardArr, int sz) {
    SetupCardArray(cardArr, sz);
    SortCardArray(cardArr, sz);
}