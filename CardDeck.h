#ifndef CARDDECK_H
#define CARDDECK_H

#include "BladeCards.h"
#include <stdlib.h>

typedef struct CardDeck {
    Card *deckArr;
    int deckValue, deckQuantity;
} CardDeck;

#endif