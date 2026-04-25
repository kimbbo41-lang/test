#pragma once

#include "Common.h"

#include <string>
#include <vector>

class Monster
{
public:
    Monster(std::string name, Stats stats, int expReward, int goldReward,
            bool isBoss = false, bool isElite = false);

    const std::string& getName() const;
    bool isAlive() const;
    bool isBoss() const;
    bool isElite() const;

    const Stats& getStats() const;
    int getExpReward() const;
    int getGoldReward() const;

    void takeDamage(int damage);
    void takeTrueDamage(int damage);   // 방어 무시 (화상 등)

    // 상태이상
    void applyStatus(StatusEffect effect, int duration, int magnitude);
    bool hasStatus(StatusEffect effect) const;
    bool isStunned() const;
    std::string tickStatus(int currentFloor);   // 턴 종료 시 호출. 로그 문자열 반환 (빈 문자열이면 무변화)
    void clearStatuses();

private:
    std::string name;
    Stats stats;
    int expReward;
    int goldReward;
    bool boss;
    bool elite;
    std::vector<StatusInstance> statuses;
};

namespace MonsterFactory
{
    Monster createForFloor(int floor);
    Monster createEliteForFloor(int floor);
    Monster createBoss(int floor);
}
