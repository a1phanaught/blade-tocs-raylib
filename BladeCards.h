#ifndef BLADECARDS_H
#define BLADECARDS_H

#include "raylib.h"

enum effects{NONE, BLAST, BOLT, FORCE, MIRROR, REVIVE, DEAD};

typedef struct Card {
    Texture2D txte;
    int value, indexValue;
    enum effects effect;
    // Card startPoint is where the upper left (LHS) corner of the card is located
    // Card endPoint is where the lower right (RHS) corner of the card is located
    // The reason I do this is to detect mouse hover and mouse clicks on the card
    Vector2 startPoint, endPoint;
} Card;

typedef struct CardsAtHand {
    Card *cardArr;
    int quantity;
} CardsAtHand;

Card one, two, three, four, five, six, seven;
Card blast, bolt, force, mirror, dead;

// Use this to scale Card size in the game.
const float TEXTURE_SCALING = 0.60;

// Do not adjust anything in this section ---------------------
const int ACTUAL_CARD_WIDTH = 241;
const int ACTUAL_CARD_HEIGHT = 319;
const int CARD_WIDTH = ACTUAL_CARD_WIDTH * TEXTURE_SCALING;
const int CARD_HEIGHT = ACTUAL_CARD_HEIGHT * TEXTURE_SCALING;
//-------------------------------------------------------------

// (Card) {Texture2D, value, indexValue, effect}
void CardsInit(void) {
    Image img = LoadImage("./img/1.png");
    ImageResize(&img, CARD_WIDTH, CARD_HEIGHT);
    one = (Card){LoadTextureFromImage(img), 1, 1, REVIVE};

    img = LoadImage("./img/2.png");
    ImageResize(&img, CARD_WIDTH, CARD_HEIGHT);
    two = (Card){LoadTextureFromImage(img), 2, 2, NONE};

    img = LoadImage("./img/3.png");
    ImageResize(&img, CARD_WIDTH, CARD_HEIGHT);
    three = (Card){LoadTextureFromImage(img), 3, 3, NONE};

    img = LoadImage("./img/4.png");
    ImageResize(&img, CARD_WIDTH, CARD_HEIGHT);
    four = (Card){LoadTextureFromImage(img), 4, 4, NONE};

    img = LoadImage("./img/5.png");
    ImageResize(&img, CARD_WIDTH, CARD_HEIGHT);
    five = (Card){LoadTextureFromImage(img), 5, 5, NONE};

    img = LoadImage("./img/6.png");
    ImageResize(&img, CARD_WIDTH, CARD_HEIGHT);
    six = (Card){LoadTextureFromImage(img), 6, 6, NONE};

    img = LoadImage("./img/7.png");
    ImageResize(&img, CARD_WIDTH, CARD_HEIGHT);
    seven = (Card){LoadTextureFromImage(img), 7, 7, NONE};

    img = LoadImage("./img/blast.png");
    ImageResize(&img, CARD_WIDTH, CARD_HEIGHT);
    blast = (Card){LoadTextureFromImage(img), 1, 0, BLAST};
    
    img = LoadImage("./img/bolt.png");
    ImageResize(&img, CARD_WIDTH, CARD_HEIGHT);
    bolt = (Card){LoadTextureFromImage(img), 1, 0, BOLT};

    img = LoadImage("./img/force.png");
    ImageResize(&img, CARD_WIDTH, CARD_HEIGHT);
    force = (Card){LoadTextureFromImage(img), 1, 0, FORCE};

    img = LoadImage("./img/mirror.png");
    ImageResize(&img, CARD_WIDTH, CARD_HEIGHT);
    mirror = (Card){LoadTextureFromImage(img), 1, 0, MIRROR};

    img = LoadImage("./img/back.png");
    ImageResize(&img, CARD_WIDTH, CARD_HEIGHT);
    dead = (Card){LoadTextureFromImage(img), 1, 0, DEAD};

    UnloadImage(img);
}

#endif // BLADECARDS_H