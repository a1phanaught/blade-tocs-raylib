#include "raylib.h"

void PlayerLost() {
    while (!WindowShouldClose()) {
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            DrawText("YOU LOSE!", 350, 250, 150, BLACK);
        }
        EndDrawing();
    }
}

void CPULost() {
    while (!WindowShouldClose()) {
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            DrawText("YOU WIN!", 350, 250, 150, BLACK);
        }
        EndDrawing();
    }
}