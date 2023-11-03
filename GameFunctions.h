#ifndef GAMEFUNCTIONS_H
#define GAMEFUNCTIONS_H

#include "BladeCards.h"
#include <stdlib.h>
#include <stdio.h>

// Get random card, but ensure that no specific type of card appears
// more than twice on player's hand.
Card GetRandomCard(int *determinantArr) {
    int determinant = GetRandomValue(1,8);

    while (determinantArr[determinant-1] >= 2)
        determinant = determinant > 7 ? 1 : determinant + 1;

    // Hashtable ftw (^ _ ^)b
    determinantArr[determinant-1]++;

    switch (determinant) {
        case 1: return one;    case 2: return two;
        case 3: return three;  case 4: return four;
        case 5: return five;   case 6: return six;
        case 7: return seven;
        case 8:
            switch (GetRandomValue(1,4)) {
                case 1: return blast; case 2: return bolt;
                case 3: return force; case 4: return mirror;
            }
    }
    return dead;
}

void SetupCardArray(Card *cardArr, int cardQuantity) {
    int *detArr = (int*)calloc(11, sizeof(int));
    for (int i = 0; i < cardQuantity; i++) {
        cardArr[i] = GetRandomCard(detArr);
    }
    return;
}

void swapCards(Card *a, Card *b) {
    Card temp = *a;
    *a = *b;
    *b = temp;
}

void SortCardArray(Card *cardArr, int cardsQuantity) {
    for (int i = 0; i < cardsQuantity - 1; i++) {
        for (int j = 0; j < cardsQuantity - i - 1; j++) {
            if (cardArr[j].indexValue > cardArr[j + 1].indexValue) {
                swapCards(&cardArr[j], &cardArr[j + 1]);
            }
        }
    }
}

bool IsCursorHoverOverCard(Card *card) {
    Vector2 mouseCoordinate = GetMousePosition();

    if (mouseCoordinate.x > card->startPoint.x && mouseCoordinate.y > card->startPoint.y &&
    mouseCoordinate.x < card->endPoint.x && mouseCoordinate.y < card->endPoint.y)
        return true;

    return false;
}

void RemoveCardAtIndex(Card **array, int *size, int indexToRemove) {
    if (indexToRemove < 0 || indexToRemove >= *size) {
        return;
    }

    // Shift elements after the indexToRemove one position to the left
    for (int i = indexToRemove; i < (*size) - 1; i++) {
        (*array)[i] = (*array)[i + 1];
    }

    // Decrease the size of the array
    (*size)--;

    // Resize the dynamic array
    //*array = (Card *)realloc(*array, (*size) * sizeof(Card));
}

void AddCardToDeck(Card **array, int *size, Card value) {

    // Resize the array to accommodate the new value.
    *size += 1;

    // Add the value to the end of the array.
    (*array)[*size - 1] = value;
}

// Make sure to pick a card that has a higher accumulated value than playerVal
int GetRandomCardIndexCPU(Card *cardArr, int sz, int opponentVal, int playerVal) {
    //if (*(cardArr + sz - 1) == Card.back);

    int determinant = GetRandomValue(0,sz-1);
    int limit = determinant;

    while ((cardArr[determinant].effect == NONE) && cardArr[determinant].value + opponentVal <= playerVal) {
        determinant++;
        if (determinant > sz-1) determinant = 0;
        if (limit == determinant) return -1;
    }

    return determinant;
}

Texture2D MakeBoard(int width, int height) {
    Image img = LoadImage("./img/board.png");
    ImageResize(&img, width, height);
    Texture2D boardTxte = LoadTextureFromImage(img);
    UnloadImage(img);
    return boardTxte;
}

char *WhatWasCPUDoing(Card usedCard) {
    switch (usedCard.effect) {
        case NONE:
            int usedCardValue = usedCard.value;
            char *tempStr = (char*)calloc(20, sizeof(char));
            sprintf(tempStr, "CPU used a card = %d", usedCardValue);
            return tempStr;
        case REVIVE:
            return "CPU revived a card!";
        case BLAST:
            return "CPU removed one of your playing cards!";
        case BOLT:
            return "CPU destroyed one of your played cards!";
        case FORCE:
            return "CPU doubled the total value of its cards!";
        case MIRROR:
            return "CPU swapped its played cards with yours!";
        case DEAD:
            // Suppress warning message
            return "";
    }
    return "";
}

#endif // GAMEFUNCTIONS_H