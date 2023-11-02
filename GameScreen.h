#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include "raylib.h"
#include "BladeCards.h"
#include "GameFunctions.h"
#include "CardDeck.h"

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------

enum GAME_FLAGS {PLAYER_MOVE, CPU_MOVE, RESET_DECK, PLAYER_LOST, CPU_LOST} GAME_FLAG;
int PLAYER_VALUE = 0, CPU_VALUE = 0;

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

    CardsAtHand CPUCards = {(Card*)calloc(10, sizeof(Card)), 10};
    PrepareCards(CPUCards.cardArr, CPUCards.quantity);
    CardDeck CPUDeck = {(Card*)malloc(20 * sizeof(Card)), 0, 0, dead};

    const float playerYCoordinate = screenHeight - (CARD_HEIGHT + 10.0), CPUYCoordinate = 10.0;
    const float playerYEndpoint = playerYCoordinate + CARD_HEIGHT, CPUYEndpoint = CPUYCoordinate + CARD_HEIGHT;
    const float playerDeckYCoordinate = playerYCoordinate - CARD_HEIGHT - 50.0,
    CPUDeckYCoordinate = CPUYCoordinate + CARD_HEIGHT + 50.0;
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Is mouse clicked on this frame?
        IsMouseClicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

        if (GAME_FLAG == RESET_DECK) {
            playerDeck.deckQuantity = CPUDeck.deckQuantity = 1;
            playerDeck.deckArr[0] = GetRandomCard(buf);
            CPUDeck.deckArr[0] = GetRandomCard(buf);
            playerDeck.deckValue = playerDeck.deckArr[0].value;
            CPUDeck.deckValue = CPUDeck.deckArr[0].value;
            GAME_FLAG = playerDeck.deckValue > CPUDeck.deckValue ? CPU_MOVE : PLAYER_MOVE;

        }

        if (playerDeck.deckValue == CPUDeck.deckValue) {
            // Skip to the next frame to cancel mouse click, ignore updating cards' positions
            GAME_FLAG = RESET_DECK;
            EndDrawing();
            continue;
        }

        if (GAME_FLAG == CPU_MOVE) {
            int CPUChosenIndex = GetRandomCardIndexCPU(CPUCards.cardArr, CPUCards.quantity, CPUDeck.deckValue, playerDeck.deckValue);
            if (CPUChosenIndex < 0) {
                GAME_FLAG = CPU_LOST;
                continue;
            }
            Card tempCardStorer = playerCards.cardArr[CPUChosenIndex];
            ExertCardEffect(CPUCards.cardArr[CPUChosenIndex], &CPUDeck, &playerDeck, &playerCards);
            RemoveCardAtIndex(&CPUCards.cardArr, &CPUCards.quantity, CPUChosenIndex);
            // Do not compare card deck value and use below method instead as comparing values
            // could cause a bug on player's turn
            if(tempCardStorer.effect != BLAST)
                GAME_FLAG = PLAYER_MOVE;
        }

        CPUCards.cardArr[0].startPoint.x = (screenWidth-(CARD_WIDTH+10.0)*CPUCards.quantity)/2.0;
        CPUCards.cardArr[0].endPoint.x = CPUCards.cardArr[0].startPoint.x + CARD_WIDTH;
        CPUCards.cardArr[0].endPoint.y = CPUYEndpoint;

        for (int i = 1; i < CPUCards.quantity; i++) {
            CPUCards.cardArr[i].startPoint.x = CPUCards.cardArr[i-1].endPoint.x + 10.0f;
            CPUCards.cardArr[i].endPoint.x = CPUCards.cardArr[i].startPoint.x + CARD_WIDTH;
            CPUCards.cardArr[i].endPoint.y = CPUYEndpoint;
        }

        if (playerDeck.deckValue == CPUDeck.deckValue) {
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
                    Card tempCardStorer = playerCards.cardArr[i];
                    ExertCardEffect(playerCards.cardArr[i], &playerDeck, &CPUDeck, &CPUCards);
                    RemoveCardAtIndex(&playerCards.cardArr, &playerCards.quantity, i);
                    // If player deck value is less than the opponent even after playing a card
                    if (playerDeck.deckValue < CPUDeck.deckValue) {
                        GAME_FLAG = PLAYER_LOST;
                        continue;
                    }
                    // Do not compare card deck value and use below method instead as comparing values
                    // could cause a bug on player's turn
                    if (tempCardStorer.effect != BLAST)
                        GAME_FLAG = CPU_MOVE;
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
            // Start determining CPUCards and render them

            for (int i = 0; i < CPUCards.quantity; i++)
                // I only use dead cards texture for CPU cards but that does not mean that the
                // CPU cards are truly dead. I just want to hide what cards are in CPUs' hand
                // from the players
                DrawTexture(dead.txte, CPUCards.cardArr[i].startPoint.x, CPUCards.cardArr[i].startPoint.y, WHITE);

            // End CPUCards rendering section
            // Start Deck (played cards) rendering section

            for (int i = 0, playerX = screenWidth - 200; i < playerDeck.deckQuantity; i++)
                DrawTexture(playerDeck.deckArr[i].txte, playerX -= CARD_WIDTH/2, playerDeckYCoordinate, WHITE);

            for (int i = 0, CPUX = screenWidth - 200; i < CPUDeck.deckQuantity; i++)
                DrawTexture(CPUDeck.deckArr[i].txte, CPUX -= CARD_WIDTH/2, CPUDeckYCoordinate, WHITE);

            char playerDeckValue[3], CPUDeckValue[3];
            
            itoa(playerDeck.deckValue, playerDeckValue, 10);
            itoa(CPUDeck.deckValue, CPUDeckValue, 10);

            DrawText("Score", 150, playerDeckYCoordinate, 75, WHITE);
            DrawText(playerDeckValue, 150, playerDeckYCoordinate + CARD_HEIGHT/2, 75, WHITE);
            DrawText("Score", 150, CPUDeckYCoordinate, 75, WHITE);
            DrawText(CPUDeckValue, 150, CPUDeckYCoordinate + CARD_HEIGHT/2, 75, WHITE);
            // End deck rendering section
        }
        EndDrawing();
        if (GAME_FLAG == CPU_MOVE) WaitTime(1);
    }
    // Free everything here
    
    free(playerCards.cardArr);
    free(playerDeck.deckArr);
    free(CPUCards.cardArr);
    free(CPUDeck.deckArr);
}

void PrepareCards(Card *cardArr, int sz) {
    SetupCardArray(cardArr, sz);
    SortCardArray(cardArr, sz);
}

#endif