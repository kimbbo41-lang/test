#include "Monster.h"

#include <algorithm>
#include <array>

Monster::Monster(std::string name, Stats stats, int expReward, int goldReward, bool isBoss)
    : name(std::move(name))
    , stats(stats)
    , expReward(expReward)
    , goldReward(goldReward)
    , boss(isBoss)
{
}

const std::string& Monster::getName() const { return name; }
bool Monster::isAlive() const { return stats.hp > 0; }
bool Monster::isBoss() const { return boss; }
const Stats& Monster::getStats() const { return stats; }
int Monster::getExpReward() const { return expReward; }
int Monster::getGoldReward() const { return goldReward; }

void Monster::takeDamage(int damage)
{
    if (damage < 1) damage = 1;
    stats.hp = std::max(0, stats.hp - damage);
}

namespace MonsterFactory
{
    static const std::array<std::string, 5> MONSTER_POOL = {
        "슬라임", "고블린", "스켈레톤", "오크", "다크엘프"
    };

    Monster createForFloor(int floor)
    {
        int hp      = 30 + floor * 20 + randomInt(0, 10);
        int atk     = 8  + floor * 3  + randomInt(0, 2);
        int def     = 3  + floor * 2;
        int agi     = 5  + floor * 2  + randomInt(0, 3);
        int expR    = 10 + floor * 8;
        int goldR   = 5  + floor * 4;

        int idx = std::clamp(floor - 1, 0, static_cast<int>(MONSTER_POOL.size()) - 1);
        std::string name = MONSTER_POOL[idx];

        Stats stats{ hp, hp, 0, 0, atk, def, agi };
        return Monster(name, stats, expR, goldR, false);
    }

    Monster createBoss(int floor)
    {
        int hp    = 110 + floor * 35;
        int atk   = 13  + floor * 4;
        int def   = 8   + floor * 3;
        int agi   = 10  + floor * 2;
        int expR  = 50  + floor * 20;
        int goldR = 40  + floor * 15;

        std::string name;
        switch (floor)
        {
        case 1: name = "슬라임 왕";      break;
        case 2: name = "고블린 족장";    break;
        case 3: name = "리치";           break;
        case 4: name = "오크 로드";      break;
        default: name = "던전의 지배자"; break;
        }

        Stats stats{ hp, hp, 0, 0, atk, def, agi };
        return Monster(name, stats, expR, goldR, true);
    }
}
