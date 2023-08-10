#ifndef BLADECARDS_H
#define BLADECARDS_H

#include "raylib.h"

enum effects{BLAST, BOLT, FORCE, MIRROR};

typedef struct Card {
    Texture2D txte;
    // Card startPoint is where the upper left corner of the card is located
    // Card endPoint is where the lower right corner of the card is located
    // The reason I do this is to detect mouse hover and mouse clicks on the card
    Vector2 startPoint;
    Vector2 endPoint;
    int value;
    //char* effect;
    enum effects effect;
} Card;

Card one, two, three, four, five, six, seven;
Card blast, bolt, force, mirror, back;

const int CARD_WIDTH = 180;
const int CARD_HEIGHT = 240;

void CardsInit(void) {
    Image img = LoadImage("./img/1.png");
    one.txte = LoadTextureFromImage(img);
    one.value = 1;

    img = LoadImage("./img/2.png");
    two.txte = LoadTextureFromImage(img);
    two.value = 2;

    img = LoadImage("./img/3.png");
    three.txte = LoadTextureFromImage(img);
    three.value = 3;

    img = LoadImage("./img/4.png");
    four.txte = LoadTextureFromImage(img);
    four.value = 4;

    img = LoadImage("./img/5.png");
    five.txte = LoadTextureFromImage(img);
    five.value = 5;

    img = LoadImage("./img/6.png");
    six.txte = LoadTextureFromImage(img);
    six.value = 6;

    img = LoadImage("./img/7.png");
    seven.txte = LoadTextureFromImage(img);
    seven.value = 7;

    img = LoadImage("./img/blast.png");
    blast.txte = LoadTextureFromImage(img);
    blast.value = 1;
    blast.effect = BLAST;
    
    img = LoadImage("./img/bolt.png");
    bolt.txte = LoadTextureFromImage(img);
    bolt.value = 1;
    bolt.effect = BOLT;

    img = LoadImage("./img/force.png");
    force.txte = LoadTextureFromImage(img);
    force.value = 1;
    force.effect = FORCE;

    img = LoadImage("./img/mirror.png");
    mirror.txte = LoadTextureFromImage(img);
    mirror.value = 1;
    mirror.effect = MIRROR;

    UnloadImage(img);
}

#endif // BLADECARDS_H