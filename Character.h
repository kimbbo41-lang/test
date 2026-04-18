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

    void initStatsForJob();
    void levelUpIfReady();
};
