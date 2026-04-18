#pragma once

#include "Common.h"

#include <string>

class Monster
{
public:
    Monster(std::string name, Stats stats, int expReward, int goldReward, bool isBoss = false);

    const std::string& getName() const;
    bool isAlive() const;
    bool isBoss() const;

    const Stats& getStats() const;
    int getExpReward() const;
    int getGoldReward() const;

    void takeDamage(int damage);

private:
    std::string name;
    Stats stats;
    int expReward;
    int goldReward;
    bool boss;
};

namespace MonsterFactory
{
    Monster createForFloor(int floor);
    Monster createBoss(int floor);
}
