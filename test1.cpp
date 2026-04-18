#include "Game.h"

#include <windows.h>

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    Game game;
    game.run();
    return 0;
}
