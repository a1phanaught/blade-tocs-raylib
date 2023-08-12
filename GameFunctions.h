#ifndef GAMEFUNCTIONS_H
#define GAMEFUNCTIONS_H

#include "BladeCards.h"

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

#endif // GAMEFUNCTIONS_H