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
    bool IsMouseClicked = false;
    const Texture2D boardTxte = MakeBoard(screenWidth, screenHeight);
    
    // Card array must be dynamic
    CardsAtHand playerCards = {(Card*)calloc(10, sizeof(Card)), 10};
    PrepareCards(playerCards.cardArr, playerCards.quantity);
    CardDeck playerDeck = {(Card*)malloc(20 * sizeof(Card)), 0, 0, dead};

    //Card *opponentCards = (Card*)calloc(opponentCardsQuantity, sizeof(Card));
    CardsAtHand opponentCards = {(Card*)calloc(10, sizeof(Card)), 10};
    PrepareCards(opponentCards.cardArr, opponentCards.quantity);
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
            int CPUChosenIndex = GetRandomCardIndexCPU(opponentCards.cardArr, opponentCards.quantity, opponentDeck.deckValue, playerDeck.deckValue);
            if (CPUChosenIndex < 0) {
                GAME_FLAG = CPU_LOST;
                continue;
            }
            ExertCardEffect(opponentCards.cardArr[CPUChosenIndex], &opponentDeck, &playerDeck);
            RemoveCardAtIndex(&opponentCards.cardArr, &opponentCards.quantity, CPUChosenIndex);
            GAME_FLAG = PLAYER_MOVE;
        }

        opponentCards.cardArr[0].startPoint.x = (screenWidth-(CARD_WIDTH+10.0)*opponentCards.quantity)/2.0;
        opponentCards.cardArr[0].endPoint.x = opponentCards.cardArr[0].startPoint.x + CARD_WIDTH;
        opponentCards.cardArr[0].endPoint.y = opponentYEndpoint;

        for (int i = 1; i < opponentCards.quantity; i++) {
            opponentCards.cardArr[i].startPoint.x = opponentCards.cardArr[i-1].endPoint.x + 10.0f;
            opponentCards.cardArr[i].endPoint.x = opponentCards.cardArr[i].startPoint.x + CARD_WIDTH;
            opponentCards.cardArr[i].endPoint.y = opponentYEndpoint;
        }

        if (playerDeck.deckValue == opponentDeck.deckValue) {
            GAME_FLAG = RESET_DECK;
            EndDrawing();
            continue;
        }
        
        for (int i = 0; i < playerCards.quantity; i++) {
            
            if (i == 0)
                playerCards.cardArr[i].startPoint.x = (screenWidth-(CARD_WIDTH+10.0)*playerCards.quantity)/2.0;
            else
                playerCards.cardArr[i].startPoint.x = playerCards.cardArr[i-1].endPoint.x + 10.0f;

            playerCards.cardArr[i].endPoint.x = playerCards.cardArr[i].startPoint.x + CARD_WIDTH;
            playerCards.cardArr[i].endPoint.y = playerYEndpoint;

            if (IsCursorHoverOverCard(&playerCards.cardArr[i]) && GAME_FLAG == PLAYER_MOVE) {
                playerCards.cardArr[i].startPoint.y = playerYCoordinate - 20;
                if (IsMouseClicked) {
                    ExertCardEffect(playerCards.cardArr[i], &playerDeck, &opponentDeck);
                    RemoveCardAtIndex(&playerCards.cardArr, &playerCards.quantity, i);
                    GAME_FLAG = OPPONENT_MOVE;
                }
            }
            else playerCards.cardArr[i].startPoint.y = playerYCoordinate;
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

            DrawTexture(boardTxte, 0, 0, WHITE);

            // Alas determine position of playerCards and render them...

            for (int i = 0; i < playerCards.quantity; i++)
                DrawTexture(playerCards.cardArr[i].txte, playerCards.cardArr[i].startPoint.x, playerCards.cardArr[i].startPoint.y, WHITE);

            // End playerCards rendering section
            // Start determining opponentCards and render them

            for (int i = 0; i < opponentCards.quantity; i++)
                // I only use dead cards texture for opponent cards but that does not mean that the
                // opponent cards are truly dead. I just want to hide what cards are in opponents hand
                // from the players
                DrawTexture(dead.txte, opponentCards.cardArr[i].startPoint.x, opponentCards.cardArr[i].startPoint.y, WHITE);

            // End opponentCards rendering section
            // Start Deck (played cards) rendering section

            for (int i = 0, playerX = screenWidth - 200; i < playerDeck.deckQuantity; i++)
                DrawTexture(playerDeck.deckArr[i].txte, playerX -= CARD_WIDTH/2, playerDeckYCoordinate, WHITE);

            for (int i = 0, opponentX = screenWidth - 200; i < opponentDeck.deckQuantity; i++)
                DrawTexture(opponentDeck.deckArr[i].txte, opponentX -= CARD_WIDTH/2, opponentDeckYCoordinate, WHITE);

            char playerDeckValue[3], opponentDeckValue[3];
            
            itoa(playerDeck.deckValue, playerDeckValue, 10);
            itoa(opponentDeck.deckValue, opponentDeckValue, 10);

            DrawText("Score", 150, playerDeckYCoordinate, 75, WHITE);
            DrawText(playerDeckValue, 150, playerDeckYCoordinate + CARD_HEIGHT/2, 75, WHITE);
            DrawText("Score", 150, opponentDeckYCoordinate, 75, WHITE);
            DrawText(opponentDeckValue, 150, opponentDeckYCoordinate + CARD_HEIGHT/2, 75, WHITE);
            // End deck rendering section
        }
        EndDrawing();
        if (GAME_FLAG == OPPONENT_MOVE) WaitTime(1);
    }
    // Free everything here
    
    free(playerCards.cardArr);
    free(playerDeck.deckArr);
    free(opponentCards.cardArr);
    free(opponentDeck.deckArr);
}

void PrepareCards(Card *cardArr, int sz) {
    SetupCardArray(cardArr, sz);
    SortCardArray(cardArr, sz);
}

#endif