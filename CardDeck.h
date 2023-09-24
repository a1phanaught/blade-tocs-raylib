#ifndef CARDDECK_H
#define CARDDECK_H

#include "BladeCards.h"
#include <stdlib.h>

typedef struct CardDeck {
    Card *deckArr;
    int deckValue, deckQuantity;
    Card deadCard;
} CardDeck;

void ExertCardEffect(Card usedCard, CardDeck *currentDeck, CardDeck *opponentDeck) {
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
            if (opponentDeck->deckArr[opponentDeck->deckQuantity - 1].effect != DEAD) {
                opponentDeck->deadCard = opponentDeck->deckArr[opponentDeck->deckQuantity - 1];
                opponentDeck->deckValue -= opponentDeck->deckArr[opponentDeck->deckQuantity - 1].value;
                opponentDeck->deckArr[opponentDeck->deckQuantity - 1] = dead;
            }
            return;
        // Swap opponent's deck with player's deck
        case MIRROR:
            CardDeck storedDeck = *opponentDeck;
            *opponentDeck = *currentDeck;
            *currentDeck = storedDeck;
            return;
        case BLAST:
            // This one is a special case, should be handled natively in GameScreen.h
            return;
        case FORCE:
            currentDeck->deckValue *= 2;
            return;
        case DEAD:
            // putting this here just to supress the warning message
            return;
    }
    return;
}

#endif