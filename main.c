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

enum FLAGS {PLAYER_MOVE, OPPONENT_MOVE, RESET_DECK} FLAG;
int PLAYER_VALUE = 0, OPPONENT_VALUE = 0;

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------

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
    int playerDeckQuantity = 0, opponentDeckQuantity = 0;
    
    // Card array must be dynamic
    Card *playerCards = (Card*)calloc(playerCardsQuantity, sizeof(Card));
    PrepareCards(playerCards, playerCardsQuantity);
    Card *playerDeck = (Card*)malloc(20 * sizeof(Card));
    //*playerDeck = GetRandomCard(buf);
    AddCardToDeck(&playerDeck, &playerDeckQuantity, GetRandomCard(buf));

    Card *opponentCards = (Card*)calloc(opponentCardsQuantity, sizeof(Card));
    PrepareCards(opponentCards, opponentCardsQuantity);
    Card *opponentDeck = (Card*)malloc(20 * sizeof(Card));
    //*opponentDeck = /*GetRandomCard(buf);*/four;
    AddCardToDeck(&opponentDeck, &opponentDeckQuantity, four);

    const float playerYCoordinate = screenHeight - (CARD_HEIGHT + 10.0), opponentYCoordinate = 10.0;
    const float playerYEndpoint = playerYCoordinate + CARD_HEIGHT, opponentYEndpoint = opponentYCoordinate + CARD_HEIGHT;
    const float playerDeckYCoordinate = playerYCoordinate - CARD_HEIGHT - 50.0,
    opponentDeckYCoordinate = opponentYCoordinate + CARD_HEIGHT + 50.0;
    //--------------------------------------------------------------------------------------

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    PLAYER_VALUE = playerDeck[0].value;
    OPPONENT_VALUE = opponentDeck[0].value;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        //UpdateDrawFrame();

        // Reminder that you're still within a loop, so don't worry too much about updating the position of cards...
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            if (PLAYER_VALUE == OPPONENT_VALUE) FLAG = RESET_DECK;

            // The first card is important because it acts as a positioning determinant for all of the other cards.
            // If no more card is in the player's hand, skip the entire playerCards rendering section.

            // Alas determine position of playerCards and render them...

            if (playerCardsQuantity > 0) {
                playerCards[0].startPoint.x = (screenWidth-(CARD_WIDTH+10.0)*playerCardsQuantity)/2.0;
                playerCards[0].endPoint.x = playerCards[0].startPoint.x + CARD_WIDTH;
                playerCards[0].endPoint.y = playerYEndpoint;
                if (IsCursorHoverOverCard(&playerCards[0]) && FLAG == PLAYER_MOVE) {
                    playerCards[0].startPoint.y = playerYCoordinate - 20;
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        if (playerCards[0].effect == NONE || playerCards[0].effect == REVIVE) {
                            AddCardToDeck(&playerDeck, &playerDeckQuantity, playerCards[0]);
                            PLAYER_VALUE += playerCards[0].value;
                        }
                        RemoveCardAtIndex(&playerCards, &playerCardsQuantity, 0);
                        FLAG = OPPONENT_MOVE;
                    }
                }
                else playerCards[0].startPoint.y = playerYCoordinate;
                
                for (int i = 1; i < playerCardsQuantity; i++) {
                    playerCards[i].startPoint.x = playerCards[i-1].endPoint.x + 10.0f;
                    playerCards[i].endPoint.x = playerCards[i].startPoint.x + CARD_WIDTH;
                    playerCards[i].endPoint.y = playerYEndpoint;

                    if (IsCursorHoverOverCard(&playerCards[i]) && FLAG == PLAYER_MOVE) {
                        playerCards[i].startPoint.y = playerYCoordinate - 20;
                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                            if (playerCards[i].effect == NONE || playerCards[i].effect == REVIVE) {
                                AddCardToDeck(&playerDeck, &playerDeckQuantity, playerCards[i]);
                                PLAYER_VALUE += playerCards[i].value;
                            }
                            RemoveCardAtIndex(&playerCards, &playerCardsQuantity, i);
                            FLAG = OPPONENT_MOVE;
                        }
                    }
                    else playerCards[i].startPoint.y = playerYCoordinate;
                }

                for (int i = 0; i < playerCardsQuantity; i++)
                    DrawTexture(playerCards[i].txte, playerCards[i].startPoint.x, playerCards[i].startPoint.y, WHITE);
            }

            // End playerCards rendering section
            // Start determining opponentCards and render them

            if (opponentCardsQuantity > 0) {

                if (FLAG == OPPONENT_MOVE) {    
                    int CPUChosenIndex = GetRandomCardIndexCPU(opponentCards, opponentCardsQuantity, OPPONENT_VALUE, PLAYER_VALUE);
                    if (opponentCards[CPUChosenIndex].effect == NONE || opponentCards[CPUChosenIndex].effect == REVIVE) {
                        AddCardToDeck(&opponentDeck, &opponentDeckQuantity, opponentCards[CPUChosenIndex]);
                        OPPONENT_VALUE += opponentCards[CPUChosenIndex].value;
                    }
                    RemoveCardAtIndex(&opponentCards, &opponentCardsQuantity, CPUChosenIndex);
                    FLAG = PLAYER_MOVE;
                    WaitTime(1.00);
                }

                opponentCards[0].startPoint.x = (screenWidth-(CARD_WIDTH+10.0)*opponentCardsQuantity)/2.0;
                opponentCards[0].endPoint.x = opponentCards[0].startPoint.x + CARD_WIDTH;
                opponentCards[0].endPoint.y = opponentYEndpoint;

                for (int i = 1; i < opponentCardsQuantity; i++) {
                    opponentCards[i].startPoint.x = opponentCards[i-1].endPoint.x + 10.0f;
                    opponentCards[i].endPoint.x = opponentCards[i].startPoint.x + CARD_WIDTH;
                    opponentCards[i].endPoint.y = opponentYEndpoint;
                }

                for (int i = 0; i < opponentCardsQuantity; i++)
                    DrawTexture(back.txte, opponentCards[i].startPoint.x, opponentCards[i].startPoint.y, WHITE);
            }

            // End opponentCards rendering section
            // Start Deck (played cards) rendering section

            for (int i = 0, playerX = screenWidth - 200, opponentX = screenWidth - 200; i < playerDeckQuantity; i++) {
                DrawTexture(playerDeck[i].txte, playerX -= CARD_WIDTH/2, playerDeckYCoordinate, WHITE);
                DrawTexture(opponentDeck[i].txte, opponentX -= CARD_WIDTH/2, opponentDeckYCoordinate, WHITE);
            }

            char playerDeckValue[3], opponentDeckValue[3];
            
            itoa(PLAYER_VALUE, playerDeckValue, 10);
            itoa(OPPONENT_VALUE, opponentDeckValue, 10);

            DrawText("Score", 150, playerDeckYCoordinate, 75, DARKGRAY);
            DrawText(playerDeckValue, 150, playerDeckYCoordinate + CARD_HEIGHT/2, 75, DARKGRAY);
            DrawText("Score", 150, opponentDeckYCoordinate, 75, DARKGRAY);
            DrawText(opponentDeckValue, 150, opponentDeckYCoordinate + CARD_HEIGHT/2, 75, DARKGRAY);

            // End deck rendering section

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