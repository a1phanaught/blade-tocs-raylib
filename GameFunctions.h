#ifndef GAMEFUNCTIONS_H
#define GAMEFUNCTIONS_H

#include "BladeCards.h"
#include <stdlib.h>

Card GetRandomCard(void) {
    int determinant = GetRandomValue(1,11);

    switch (determinant) {
        case 1: return one;    case 2: return two;
        case 3: return three;  case 4: return four;
        case 5: return five;   case 6: return six;
        case 7: return seven;  case 8: return blast;
        case 9: return bolt;   case 10: return force;
        case 11: return mirror;
    }

    return back;
}

void SetupCardArray(Card *cardArr, int cardQuantity) {
    for (int i = 0; i < cardQuantity; i++) {
        cardArr[i] = GetRandomCard();
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

void RemoveCardsAtIndex(Card **array, int *size, int indexToRemove) {
    if (indexToRemove < 0 || indexToRemove >= *size) {
        //printf("Invalid index to remove\n");
        return;
    }

    // Shift elements after the indexToRemove one position to the left
    for (int i = indexToRemove; i < (*size) - 1; i++) {
        (*array)[i] = (*array)[i + 1];
    }

    // Decrease the size of the array
    (*size)--;

    // Resize the dynamic array
    *array = (Card *)realloc(*array, (*size) * sizeof(Card));
}

void AddCardToDeck(Card **array, int *size, Card value) {
    // Check if the array is NULL, initialize it if it is.
    /*if (*array == NULL) {
        *size = 0;
        *array = malloc(sizeof(int)); // Allocate memory for one integer.
        if (*array == NULL) {
            perror("Memory allocation failed");
            exit(1);
        }
    }*/

    // Resize the array to accommodate the new value.
    *size += 1;
    Card *temp = realloc(*array, (*size) * sizeof(Card));
    if (temp == NULL) {
        perror("Memory reallocation failed");
        exit(1);
    } else {
        *array = temp;
    }

    // Add the value to the end of the array.
    (*array)[*size - 1] = value;
}

#endif // GAMEFUNCTIONS_H