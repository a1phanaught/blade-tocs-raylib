#ifndef CARDDECK_H
#define CARDDECK_H

#include "BladeCards.h"
#include <stdlib.h>

typedef struct CardDeck {
    Card *deckArr;
    int deckValue, deckQuantity;
    Card deadCard;
} CardDeck;

void ExertCardEffect(Card usedCard, CardDeck *currentDeck, CardDeck *exertedDeck) {
    switch (usedCard.effect) {
        case NONE:
        case REVIVE:
            if (usedCard.effect == REVIVE && currentDeck->deckArr[currentDeck->deckQuantity - 1].effect == DEAD) {
                // Revive dead card (if exists)
                currentDeck->deckArr[currentDeck->deckQuantity - 1] = currentDeck->deadCard;
                return;
            }
            AddCardToDeck(&(currentDeck->deckArr), &(currentDeck->deckQuantity), usedCard);
            currentDeck->deckValue += usedCard.value;
            return;
        case BOLT:
            if (exertedDeck->deckArr[exertedDeck->deckQuantity - 1].effect != DEAD) {
                exertedDeck->deadCard = exertedDeck->deckArr[exertedDeck->deckQuantity - 1];
                exertedDeck->deckValue -= exertedDeck->deckArr[exertedDeck->deckQuantity - 1].value;
                exertedDeck->deckArr[exertedDeck->deckQuantity - 1] = dead;
            }
            return;
        // Swap opponent's deck with player's deck
        case MIRROR:
            CardDeck storedDeck = *exertedDeck;
            *exertedDeck = *currentDeck;
            *currentDeck = storedDeck;
            return;
        case BLAST:
            return;
        case FORCE:
            return;
        case DEAD:
            // putting this here just to supress the warning message
            return;
    }
    return;
}

#endif