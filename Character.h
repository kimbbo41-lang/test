#pragma once

#include "Common.h"
#include "Item.h"
#include "Skill.h"

#include <memory>
#include <string>
#include <vector>

class Character
{
public:
    Character(std::string name, JobType job);

    const std::string& getName() const;
    JobType getJob() const;
    std::string getJobName() const;

    int getLevel() const;
    int getExp() const;
    int getExpToNextLevel() const;
    int getGold() const;

    const Stats& getStats() const;
    int getEffectiveAttack() const;
    int getEffectiveDefense() const;
    const std::vector<Skill>& getSkills() const;

    bool isAlive() const;
    void takeDamage(int damage);
    void takeTrueDamage(int damage);   // 방어 무시 (화상 등)
    void heal(int amount);
    void restoreMp(int amount);
    bool consumeMp(int amount);

    void gainExp(int amount);
    void gainGold(int amount);
    bool spendGold(int amount);

    std::unique_ptr<Equipment> equipWeapon(std::unique_ptr<Equipment> newWeapon);
    std::unique_ptr<Equipment> equipArmor(std::unique_ptr<Equipment> newArmor);
    const Equipment* getWeapon() const;
    const Equipment* getArmor() const;

    // 상태이상
    void applyStatus(StatusEffect effect, int duration, int magnitude);
    bool hasStatus(StatusEffect effect) const;
    bool isStunned() const;
    std::string tickStatus(int currentFloor);   // 턴 종료 시 호출. 로그 반환
    void clearStatuses();
    const std::vector<StatusInstance>& getStatuses() const;

    void rest();
    void printStatus() const;

private:
    std::string name;
    JobType job;

    int level;
    int exp;
    int gold;

    Stats stats;
    std::vector<Skill> skills;

    std::unique_ptr<Equipment> weapon;
    std::unique_ptr<Equipment> armor;

    // 현재 장착 장비의 저주로 인해 스탯에서 차감된 양 (교체 시 복원용)
    int cursedHpDelta = 0;
    int cursedMpDelta = 0;
    int cursedAgiDelta = 0;

    std::vector<StatusInstance> statuses;

    void initStatsForJob();
    void levelUpIfReady();
    void refreshCurseDeltas();
};
