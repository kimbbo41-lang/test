#pragma once

#include "Common.h"

class Character;
class Monster;
class Inventory;

class CombatEngine
{
public:
    CombatEngine(Character& player, Monster& monster, Inventory& inventory);

    CombatResult run();

private:
    Character& player;
    Monster& monster;
    Inventory& inventory;

    bool playerTurn();
    void monsterTurn();

    void showCombatStatus() const;

    void doAttack();
    bool doSkill();
    bool doItem();
    bool doEscape();

    int rollDamage(int attack, int defense, int attackerAgi, bool& isCrit);
};
