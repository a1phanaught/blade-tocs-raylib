#ifndef BLADECARDS_H
#define BLADECARDS_H

#include "raylib.h"

enum effects{BLAST, BOLT, FORCE, MIRROR};

typedef struct Card {
    Texture2D txte;
    // Card startPoint is where the upper left corner of the card is located
    // Card endPoint is where the lower right corner of the card is located
    // The reason I do this is to detect mouse hover and mouse clicks on the card
    int value;
    int indexValue;
    //char* effect;
    enum effects effect;
} Card;

Card one, two, three, four, five, six, seven;
Card blast, bolt, force, mirror, back;

const float TEXTURE_SCALING = 0.60;

const int ACTUAL_CARD_WIDTH = 241;
const int ACTUAL_CARD_HEIGHT = 319;
const int CARD_WIDTH = ACTUAL_CARD_WIDTH * TEXTURE_SCALING;
const int CARD_HEIGHT = ACTUAL_CARD_HEIGHT * TEXTURE_SCALING;

void CardsInit(void) {
    Image img = LoadImage("./img/1.png");
    ImageResize(&img, CARD_WIDTH, CARD_HEIGHT);
    one.txte = LoadTextureFromImage(img);
    one.value = 1;
    one.indexValue = one.value;

    img = LoadImage("./img/2.png");
    ImageResize(&img, CARD_WIDTH, CARD_HEIGHT);
    two.txte = LoadTextureFromImage(img);
    two.value = 2;
    two.indexValue = two.value;

    img = LoadImage("./img/3.png");
    ImageResize(&img, CARD_WIDTH, CARD_HEIGHT);
    three.txte = LoadTextureFromImage(img);
    three.value = 3;
    three.indexValue = three.value;

    img = LoadImage("./img/4.png");
    ImageResize(&img, CARD_WIDTH, CARD_HEIGHT);
    four.txte = LoadTextureFromImage(img);
    four.value = 4;
    four.indexValue = four.value;

    img = LoadImage("./img/5.png");
    ImageResize(&img, CARD_WIDTH, CARD_HEIGHT);
    five.txte = LoadTextureFromImage(img);
    five.value = 5;
    five.indexValue = five.value;

    img = LoadImage("./img/6.png");
    ImageResize(&img, CARD_WIDTH, CARD_HEIGHT);
    six.txte = LoadTextureFromImage(img);
    six.value = 6;
    six.indexValue = six.value;

    img = LoadImage("./img/7.png");
    ImageResize(&img, CARD_WIDTH, CARD_HEIGHT);
    seven.txte = LoadTextureFromImage(img);
    seven.value = 7;
    seven.indexValue = seven.value;

    img = LoadImage("./img/blast.png");
    ImageResize(&img, CARD_WIDTH, CARD_HEIGHT);
    blast.txte = LoadTextureFromImage(img);
    blast.value = 1;
    blast.indexValue = 0;
    blast.effect = BLAST;
    
    img = LoadImage("./img/bolt.png");
    ImageResize(&img, CARD_WIDTH, CARD_HEIGHT);
    bolt.txte = LoadTextureFromImage(img);
    bolt.value = 1;
    bolt.indexValue = 0;
    bolt.effect = BOLT;

    img = LoadImage("./img/force.png");
    ImageResize(&img, CARD_WIDTH, CARD_HEIGHT);
    force.txte = LoadTextureFromImage(img);
    force.value = 1;
    force.indexValue = 0;
    force.effect = FORCE;

    img = LoadImage("./img/mirror.png");
    ImageResize(&img, CARD_WIDTH, CARD_HEIGHT);
    mirror.txte = LoadTextureFromImage(img);
    mirror.value = 1;
    mirror.indexValue = 0;
    mirror.effect = MIRROR;

    UnloadImage(img);
}

#endif // BLADECARDS_H