#include "Monster.h"
#include "GameConfig.h"

#include <algorithm>
#include <array>

Monster::Monster(std::string name, Stats stats, int expReward, int goldReward, bool isBoss, bool isElite)
    : name(std::move(name))
    , stats(stats)
    , expReward(expReward)
    , goldReward(goldReward)
    , boss(isBoss)
    , elite(isElite)
{
}

const std::string& Monster::getName() const { return name; }
bool Monster::isAlive() const { return stats.hp > 0; }
bool Monster::isBoss() const { return boss; }
bool Monster::isElite() const { return elite; }
const Stats& Monster::getStats() const { return stats; }
int Monster::getExpReward() const { return expReward; }
int Monster::getGoldReward() const { return goldReward; }

void Monster::takeDamage(int damage)
{
    if (damage < 1) damage = 1;
    stats.hp = std::max(0, stats.hp - damage);
}

void Monster::takeTrueDamage(int damage)
{
    if (damage < 0) damage = 0;
    stats.hp = std::max(0, stats.hp - damage);
}

void Monster::applyStatus(StatusEffect effect, int duration, int magnitude)
{
    if (effect == StatusEffect::None || duration <= 0) return;
    for (auto& s : statuses)
    {
        if (s.type == effect)
        {
            s.remainingTurns = std::max(s.remainingTurns, duration);
            s.magnitude = std::max(s.magnitude, magnitude);
            return;
        }
    }
    statuses.push_back({ effect, duration, magnitude });
}

bool Monster::hasStatus(StatusEffect effect) const
{
    for (const auto& s : statuses)
        if (s.type == effect && s.remainingTurns > 0) return true;
    return false;
}

bool Monster::isStunned() const
{
    return hasStatus(StatusEffect::Stun);
}

std::string Monster::tickStatus(int currentFloor)
{
    std::string log;
    for (auto& s : statuses)
    {
        if (s.remainingTurns <= 0) continue;

        if (s.type == StatusEffect::Poison)
        {
            int dmg = std::max(1, stats.maxHp * s.magnitude / 100);
            takeTrueDamage(dmg);
            if (!log.empty()) log += " ";
            log += name + "이(가) 독으로 " + std::to_string(dmg) + " 데미지!";
        }
        else if (s.type == StatusEffect::Burn)
        {
            int dmg = std::max(1, s.magnitude);
            takeTrueDamage(dmg);
            if (!log.empty()) log += " ";
            log += name + "이(가) 화상으로 " + std::to_string(dmg) + " 데미지!";
        }
        // Stun: 데미지 없음, 행동 차단은 isStunned()로 처리

        --s.remainingTurns;
    }
    (void)currentFloor; // 현재 로직에서는 미사용 (확장용)
    statuses.erase(
        std::remove_if(statuses.begin(), statuses.end(),
            [](const StatusInstance& s) { return s.remainingTurns <= 0; }),
        statuses.end());
    return log;
}

void Monster::clearStatuses()
{
    statuses.clear();
}

namespace MonsterFactory
{
    static const std::array<std::string, 10> MONSTER_POOL = {
        "슬라임", "고블린", "스켈레톤", "오크", "다크엘프",
        "미노타우로스", "와이번", "데몬", "아크데몬", "타락한 천사"
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
        return Monster(name, stats, expR, goldR, false, false);
    }

    Monster createEliteForFloor(int floor)
    {
        Monster base = createForFloor(floor);
        const Stats& bs = base.getStats();

        Stats elite{};
        elite.hp      = static_cast<int>(bs.hp * Config::ELITE_HP_MULT);
        elite.maxHp   = elite.hp;
        elite.mp      = 0;
        elite.maxMp   = 0;
        elite.attack  = static_cast<int>(bs.attack * Config::ELITE_ATK_MULT);
        elite.defense = bs.defense + 2;
        elite.agility = static_cast<int>(bs.agility * Config::ELITE_AGI_MULT);

        int expR  = base.getExpReward()  * Config::ELITE_REWARD_MULT;
        int goldR = base.getGoldReward() * Config::ELITE_REWARD_MULT;

        return Monster("엘리트 " + base.getName(), elite, expR, goldR, false, true);
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
        case 1:  name = "슬라임 왕";       break;
        case 2:  name = "고블린 족장";     break;
        case 3:  name = "리치";            break;
        case 4:  name = "오크 로드";       break;
        case 5:  name = "다크엘프 여왕";   break;
        case 6:  name = "미노타우로스 왕"; break;
        case 7:  name = "와이번 군주";     break;
        case 8:  name = "악마 공작";       break;
        case 9:  name = "타락한 드래곤";   break;
        case 10: name = "던전의 지배자";   break;
        default: name = "심연의 왕";       break;
        }

        Stats stats{ hp, hp, 0, 0, atk, def, agi };
        return Monster(name, stats, expR, goldR, true, false);
    }
}
