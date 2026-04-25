#include "Character.h"
#include "GameConfig.h"
#include "UI.h"

#include <algorithm>
#include <iostream>

Character::Character(std::string name, JobType job)
    : name(std::move(name))
    , job(job)
    , level(1)
    , exp(0)
    , gold(Config::STARTING_GOLD)
{
    initStatsForJob();
    skills = SkillLibrary::getSkillsForJob(job);
}

void Character::initStatsForJob()
{
    switch (job)
    {
    case JobType::Warrior:
        stats = { Config::WARRIOR_HP, Config::WARRIOR_HP,
                  Config::WARRIOR_MP, Config::WARRIOR_MP,
                  Config::WARRIOR_ATK, Config::WARRIOR_DEF, Config::WARRIOR_AGI };
        break;
    case JobType::Mage:
        stats = { Config::MAGE_HP, Config::MAGE_HP,
                  Config::MAGE_MP, Config::MAGE_MP,
                  Config::MAGE_ATK, Config::MAGE_DEF, Config::MAGE_AGI };
        break;
    case JobType::Archer:
        stats = { Config::ARCHER_HP, Config::ARCHER_HP,
                  Config::ARCHER_MP, Config::ARCHER_MP,
                  Config::ARCHER_ATK, Config::ARCHER_DEF, Config::ARCHER_AGI };
        break;
    }
}

const std::string& Character::getName() const { return name; }
JobType Character::getJob() const { return job; }

std::string Character::getJobName() const
{
    switch (job)
    {
    case JobType::Warrior: return "전사";
    case JobType::Mage:    return "마법사";
    case JobType::Archer:  return "궁수";
    }
    return "?";
}

int Character::getLevel() const { return level; }
int Character::getExp() const { return exp; }
int Character::getExpToNextLevel() const { return level * Config::EXP_PER_LEVEL; }
int Character::getGold() const { return gold; }
const Stats& Character::getStats() const { return stats; }

int Character::getEffectiveAttack() const
{
    int bonus = weapon ? weapon->getAtkBonus() : 0;
    return stats.attack + bonus;
}

int Character::getEffectiveDefense() const
{
    int bonus = armor ? armor->getDefBonus() : 0;
    return stats.defense + bonus;
}

const std::vector<Skill>& Character::getSkills() const { return skills; }

bool Character::isAlive() const { return stats.hp > 0; }

void Character::takeDamage(int damage)
{
    if (damage < 1) damage = 1;
    stats.hp = std::max(0, stats.hp - damage);
}

void Character::takeTrueDamage(int damage)
{
    if (damage < 0) damage = 0;
    stats.hp = std::max(0, stats.hp - damage);
}

void Character::heal(int amount)
{
    stats.hp = std::min(stats.maxHp, stats.hp + amount);
}

void Character::restoreMp(int amount)
{
    stats.mp = std::min(stats.maxMp, stats.mp + amount);
}

bool Character::consumeMp(int amount)
{
    if (stats.mp < amount) return false;
    stats.mp -= amount;
    return true;
}

void Character::gainExp(int amount)
{
    exp += amount;
    levelUpIfReady();
}

void Character::gainGold(int amount) { gold += amount; }

bool Character::spendGold(int amount)
{
    if (gold < amount) return false;
    gold -= amount;
    return true;
}

void Character::levelUpIfReady()
{
    while (level < Config::MAX_LEVEL && exp >= getExpToNextLevel())
    {
        exp -= getExpToNextLevel();
        ++level;

        stats.maxHp += Config::STAT_GROWTH_PER_LEVEL * 5;
        stats.maxMp += Config::STAT_GROWTH_PER_LEVEL * 3;
        stats.attack += Config::STAT_GROWTH_PER_LEVEL;
        stats.defense += Config::STAT_GROWTH_PER_LEVEL;
        stats.agility += 1;

        stats.hp = stats.maxHp;
        stats.mp = stats.maxMp;

        UI::printLine("*** 레벨 업! 현재 레벨: " + std::to_string(level) + " ***", UI::Color::Yellow);
    }
    if (level >= Config::MAX_LEVEL) exp = 0;
}

void Character::refreshCurseDeltas()
{
    // 1. 이전에 차감했던 값을 먼저 복원
    stats.maxHp  += cursedHpDelta;
    stats.maxMp  += cursedMpDelta;
    stats.agility += cursedAgiDelta;
    cursedHpDelta = 0;
    cursedMpDelta = 0;
    cursedAgiDelta = 0;

    // 2. 현재 장착 중인 저주 장비들에서 페널티 재계산 (복원된 최대치 기준)
    auto accumulate = [&](const Equipment* eq)
    {
        if (!eq || !eq->isCursed()) return;
        int v = eq->getCursePenaltyValue();
        switch (eq->getCursePenalty())
        {
        case CursePenalty::HpPercent:
            cursedHpDelta += stats.maxHp * v / 100;
            break;
        case CursePenalty::MpPercent:
            cursedMpDelta += stats.maxMp * v / 100;
            break;
        case CursePenalty::Agility:
            cursedAgiDelta += v;
            break;
        }
    };
    accumulate(weapon.get());
    accumulate(armor.get());

    stats.maxHp  -= cursedHpDelta;
    stats.maxMp  -= cursedMpDelta;
    stats.agility -= cursedAgiDelta;

    if (stats.maxHp < 1) stats.maxHp = 1;
    if (stats.maxMp < 0) stats.maxMp = 0;
    if (stats.agility < 1) stats.agility = 1;
    if (stats.hp > stats.maxHp) stats.hp = stats.maxHp;
    if (stats.mp > stats.maxMp) stats.mp = stats.maxMp;
}

std::unique_ptr<Equipment> Character::equipWeapon(std::unique_ptr<Equipment> newWeapon)
{
    auto old = std::move(weapon);
    weapon = std::move(newWeapon);
    refreshCurseDeltas();
    return old;
}

std::unique_ptr<Equipment> Character::equipArmor(std::unique_ptr<Equipment> newArmor)
{
    auto old = std::move(armor);
    armor = std::move(newArmor);
    refreshCurseDeltas();
    return old;
}

const Equipment* Character::getWeapon() const { return weapon.get(); }
const Equipment* Character::getArmor() const { return armor.get(); }

void Character::applyStatus(StatusEffect effect, int duration, int magnitude)
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

bool Character::hasStatus(StatusEffect effect) const
{
    for (const auto& s : statuses)
        if (s.type == effect && s.remainingTurns > 0) return true;
    return false;
}

bool Character::isStunned() const
{
    return hasStatus(StatusEffect::Stun);
}

std::string Character::tickStatus(int currentFloor)
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
            log += name + "은(는) 독으로 " + std::to_string(dmg) + " 데미지!";
        }
        else if (s.type == StatusEffect::Burn)
        {
            int dmg = std::max(1, s.magnitude);
            takeTrueDamage(dmg);
            if (!log.empty()) log += " ";
            log += name + "은(는) 화상으로 " + std::to_string(dmg) + " 데미지!";
        }

        --s.remainingTurns;
    }
    (void)currentFloor;
    statuses.erase(
        std::remove_if(statuses.begin(), statuses.end(),
            [](const StatusInstance& s) { return s.remainingTurns <= 0; }),
        statuses.end());
    return log;
}

void Character::clearStatuses()
{
    statuses.clear();
}

const std::vector<StatusInstance>& Character::getStatuses() const
{
    return statuses;
}

void Character::rest()
{
    int hpRecover = static_cast<int>(stats.maxHp * Config::REST_HP_RECOVERY);
    int mpRecover = static_cast<int>(stats.maxMp * Config::REST_MP_RECOVERY);
    heal(hpRecover);
    restoreMp(mpRecover);
}

void Character::printStatus() const
{
    UI::printTitle(name + " [" + getJobName() + "] Lv." + std::to_string(level));
    UI::printHpMpBar(stats.hp, stats.maxHp, "HP", UI::Color::Green);
    std::cout << "\n";
    UI::printHpMpBar(stats.mp, stats.maxMp, "MP", UI::Color::Cyan);
    std::cout << "\n";
    std::cout << "공격력: " << getEffectiveAttack();
    if (weapon) std::cout << " (" << stats.attack << " + " << weapon->getAtkBonus() << ")";
    std::cout << "\n";
    std::cout << "방어력: " << getEffectiveDefense();
    if (armor) std::cout << " (" << stats.defense << " + " << armor->getDefBonus() << ")";
    std::cout << "\n";
    std::cout << "민첩: " << stats.agility << "\n";
    std::cout << "경험치: " << exp << "/" << getExpToNextLevel() << "\n";
    std::cout << "골드: " << gold << "G\n";
    if (weapon)
    {
        std::cout << "무기: " << weapon->getName();
        if (weapon->isCursed()) std::cout << " [저주: " << weapon->getCurseDescription() << "]";
        std::cout << "\n";
    }
    if (armor)
    {
        std::cout << "방어구: " << armor->getName();
        if (armor->isCursed()) std::cout << " [저주: " << armor->getCurseDescription() << "]";
        std::cout << "\n";
    }
    if (!statuses.empty())
    {
        std::cout << "상태: ";
        bool first = true;
        for (const auto& s : statuses)
        {
            if (s.remainingTurns <= 0) continue;
            if (!first) std::cout << ", ";
            first = false;
            switch (s.type)
            {
            case StatusEffect::Poison: std::cout << "독"; break;
            case StatusEffect::Burn:   std::cout << "화상"; break;
            case StatusEffect::Stun:   std::cout << "기절"; break;
            default: break;
            }
            std::cout << "(" << s.remainingTurns << "턴)";
        }
        std::cout << "\n";
    }
    UI::printDivider();
}
