#pragma once

#include "Common.h"

class Character;
class Monster;
class Inventory;

class CombatEngine
{
public:
    CombatEngine(Character& player, Monster& monster, Inventory& inventory, int currentFloor);

    CombatResult run();

private:
    Character& player;
    Monster& monster;
    Inventory& inventory;
    int currentFloor;

    bool playerTurn();
    void monsterTurn();

    void showCombatStatus() const;

    void doAttack();
    bool doSkill();
    bool doItem();
    bool doEscape();

    int rollDamage(int attack, int defense, int attackerAgi, bool& isCrit);

    // 엘리트/보스가 공격 후 플레이어에게 무작위 상태이상 시도
    void tryInflictStatusOnPlayer();

    // 10층 최종 보스 특수 행동 (페이즈별 분기)
    void bossFinalTurn();
};
