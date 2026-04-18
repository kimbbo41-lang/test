#pragma once

#include <memory>

class Character;
class Dungeon;
class Inventory;

class Game
{
public:
    Game();
    ~Game();

    void run();

private:
    std::unique_ptr<Character> player;
    std::unique_ptr<Dungeon> dungeon;
    std::unique_ptr<Inventory> inventory;

    void showMainMenu();
    bool startNewGame();
    void mainLoop();

    void showStatus();
    void showInventoryMenu();

    void gameOver(bool victory);
};
