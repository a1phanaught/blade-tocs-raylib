#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include "raylib.h"
#include "BladeCards.h"
#include "GameFunctions.h"
#include "CardDeck.h"

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------

enum GAME_FLAGS {PLAYER_MOVE, OPPONENT_MOVE, RESET_DECK, PLAYER_LOST, CPU_LOST} GAME_FLAG;
int PLAYER_VALUE = 0, OPPONENT_VALUE = 0;

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------

void PrepareCards(Card *cardArr, int sz);

void PlayGame(int screenWidth, int screenHeight) {

    // Must initialise cards before use
    CardsInit();

    int buf[10];
    int playerCardsQuantity = 10, opponentCardsQuantity = 10;
    bool IsMouseClicked = false;
    
    // Card array must be dynamic
    Card *playerCards = (Card*)calloc(playerCardsQuantity, sizeof(Card));
    PrepareCards(playerCards, playerCardsQuantity);
    CardDeck playerDeck = {(Card*)malloc(20 * sizeof(Card)), 0, 0, dead};

    Card *opponentCards = (Card*)calloc(opponentCardsQuantity, sizeof(Card));
    PrepareCards(opponentCards, opponentCardsQuantity);
    CardDeck opponentDeck = {(Card*)malloc(20 * sizeof(Card)), 0, 0, dead};

    const float playerYCoordinate = screenHeight - (CARD_HEIGHT + 10.0), opponentYCoordinate = 10.0;
    const float playerYEndpoint = playerYCoordinate + CARD_HEIGHT, opponentYEndpoint = opponentYCoordinate + CARD_HEIGHT;
    const float playerDeckYCoordinate = playerYCoordinate - CARD_HEIGHT - 50.0,
    opponentDeckYCoordinate = opponentYCoordinate + CARD_HEIGHT + 50.0;
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        //UpdateDrawFrame();
        IsMouseClicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

        if (GAME_FLAG == RESET_DECK) {
            playerDeck.deckQuantity = opponentDeck.deckQuantity = 1;
            playerDeck.deckArr[0] = GetRandomCard(buf);
            opponentDeck.deckArr[0] = GetRandomCard(buf);
            playerDeck.deckValue = playerDeck.deckArr[0].value;
            opponentDeck.deckValue = opponentDeck.deckArr[0].value;
            GAME_FLAG = playerDeck.deckValue > opponentDeck.deckValue ? OPPONENT_MOVE : PLAYER_MOVE;

        }

        if (playerDeck.deckValue == opponentDeck.deckValue) {
            GAME_FLAG = RESET_DECK;
            EndDrawing();
            continue;
        }

        if (GAME_FLAG == OPPONENT_MOVE) {
            int CPUChosenIndex = GetRandomCardIndexCPU(opponentCards, opponentCardsQuantity, opponentDeck.deckValue, playerDeck.deckValue);
            if (CPUChosenIndex < 0) {
                GAME_FLAG = CPU_LOST;
                continue;
            }
            ExertCardEffect(opponentCards[CPUChosenIndex], &opponentDeck, &playerDeck);
            RemoveCardAtIndex(&opponentCards, &opponentCardsQuantity, CPUChosenIndex);
            GAME_FLAG = PLAYER_MOVE;
        }

        opponentCards[0].startPoint.x = (screenWidth-(CARD_WIDTH+10.0)*opponentCardsQuantity)/2.0;
        opponentCards[0].endPoint.x = opponentCards[0].startPoint.x + CARD_WIDTH;
        opponentCards[0].endPoint.y = opponentYEndpoint;

        for (int i = 1; i < opponentCardsQuantity; i++) {
            opponentCards[i].startPoint.x = opponentCards[i-1].endPoint.x + 10.0f;
            opponentCards[i].endPoint.x = opponentCards[i].startPoint.x + CARD_WIDTH;
            opponentCards[i].endPoint.y = opponentYEndpoint;
        }

        if (playerDeck.deckValue == opponentDeck.deckValue) {
            GAME_FLAG = RESET_DECK;
            EndDrawing();
            continue;
        }
        
        for (int i = 0; i < playerCardsQuantity; i++) {
            
            if (i == 0)
                playerCards[i].startPoint.x = (screenWidth-(CARD_WIDTH+10.0)*playerCardsQuantity)/2.0;
            else
                playerCards[i].startPoint.x = playerCards[i-1].endPoint.x + 10.0f;

            playerCards[i].endPoint.x = playerCards[i].startPoint.x + CARD_WIDTH;
            playerCards[i].endPoint.y = playerYEndpoint;

            if (IsCursorHoverOverCard(&playerCards[i]) && GAME_FLAG == PLAYER_MOVE) {
                playerCards[i].startPoint.y = playerYCoordinate - 20;
                if (IsMouseClicked) {
                    ExertCardEffect(playerCards[i], &playerDeck, &opponentDeck);
                    RemoveCardAtIndex(&playerCards, &playerCardsQuantity, i);
                    GAME_FLAG = OPPONENT_MOVE;
                }
            }
            else playerCards[i].startPoint.y = playerYCoordinate;
        }

        // Reminder that you're still within a loop, so don't worry too much about updating the position of cards...
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            /*switch (GAME_FLAG) {
                // Do something here...
                case CPU_LOST:
                    continue;
                case PLAYER_LOST:
                    continue;
            }*/

            // Alas determine position of playerCards and render them...

            for (int i = 0; i < playerCardsQuantity; i++)
                DrawTexture(playerCards[i].txte, playerCards[i].startPoint.x, playerCards[i].startPoint.y, WHITE);

            // End playerCards rendering section
            // Start determining opponentCards and render them

            for (int i = 0; i < opponentCardsQuantity; i++)
                // I only use dead cards texture for opponent cards but that does not mean that the
                // opponent cards are truly dead. I just want to hide what cards are in opponents hand
                // from the players
                DrawTexture(dead.txte, opponentCards[i].startPoint.x, opponentCards[i].startPoint.y, WHITE);

            // End opponentCards rendering section
            // Start Deck (played cards) rendering section

            for (int i = 0, playerX = screenWidth - 200; i < playerDeck.deckQuantity; i++)
                DrawTexture(playerDeck.deckArr[i].txte, playerX -= CARD_WIDTH/2, playerDeckYCoordinate, WHITE);

            for (int i = 0, opponentX = screenWidth - 200; i < opponentDeck.deckQuantity; i++)
                DrawTexture(opponentDeck.deckArr[i].txte, opponentX -= CARD_WIDTH/2, opponentDeckYCoordinate, WHITE);

            char playerDeckValue[3], opponentDeckValue[3];
            
            itoa(playerDeck.deckValue, playerDeckValue, 10);
            itoa(opponentDeck.deckValue, opponentDeckValue, 10);

            DrawText("Score", 150, playerDeckYCoordinate, 75, DARKGRAY);
            DrawText(playerDeckValue, 150, playerDeckYCoordinate + CARD_HEIGHT/2, 75, DARKGRAY);
            DrawText("Score", 150, opponentDeckYCoordinate, 75, DARKGRAY);
            DrawText(opponentDeckValue, 150, opponentDeckYCoordinate + CARD_HEIGHT/2, 75, DARKGRAY);

            // End deck rendering section
        }
        EndDrawing();
        if (GAME_FLAG == OPPONENT_MOVE) WaitTime(1);
    }
    // Free everything here
    
    free(playerCards);
    free(playerDeck.deckArr);
    free(opponentCards);
    free(opponentDeck.deckArr);
}

void PrepareCards(Card *cardArr, int sz) {
    SetupCardArray(cardArr, sz);
    SortCardArray(cardArr, sz);
}

#endif