#pragma once

#include "Common.h"

#include <vector>

class Character;
class Inventory;

struct Room
{
    EventType event;
    bool isBossRoom = false;
};

class Dungeon
{
public:
    Dungeon();

    bool enterNextRoom(Character& player, Inventory& inventory);

    int getCurrentFloor() const;
    int getCurrentRoom() const;
    int getTotalFloors() const;
    bool isComplete() const;
    bool isInfiniteMode() const;

private:
    int currentFloor;
    int currentRoom;
    std::vector<Room> floorRooms;
    bool complete;
    bool infiniteMode;

    void generateFloor();
    EventType rollEvent() const;

    bool handleMonster(Character& player, Inventory& inventory, bool boss);
    void handleTreasure(Character& player, Inventory& inventory);
    void handleTrap(Character& player);
    void handleRest(Character& player);
    void handleShop(Character& player, Inventory& inventory);
    void handleSell(Character& player, Inventory& inventory);
};
