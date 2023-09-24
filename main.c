/*******************************************************************************************
*
*   Make sure that you have Raylib installed on your machine. You can get Raylib at
*   https://github.com/raysan5/raylib/wiki/Working-on-Windows if you're on Windows.
*
*   To compile the game, just press F5.
*   Note that compiled executable is placed in the same folder as .c file
*
********************************************************************************************/

#include "raylib.h"
#include "GameScreen.h"

#include <stdlib.h>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------

const int SCREEN_WIDTH = 1600, SCREEN_HEIGHT = 900;
// bool GAME_START = false; // bring in the main game screen if the game has been started

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    Image icon = LoadImage("./img/blade.ico");
    //--------------------------------------------------------------------------------------

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Blade");
    SetWindowIcon(icon);
    PlayGame(SCREEN_WIDTH, SCREEN_HEIGHT);
    
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();                  // Close window and OpenGL context
    UnloadImage(icon);
    //--------------------------------------------------------------------------------------

    return 0;
}