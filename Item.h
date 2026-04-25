#pragma once

#include "Common.h"

#include <memory>
#include <string>

class Item
{
public:
    Item(std::string name, ItemType type, int price);
    virtual ~Item() = default;

    virtual std::string getDescription() const = 0;

    const std::string& getName() const;
    ItemType getType() const;
    int getPrice() const;

protected:
    std::string name;
    ItemType type;
    int price;
};

class Potion : public Item
{
public:
    Potion(std::string name, int price, int hpRestore, int mpRestore, bool clearsStatus = false);

    std::string getDescription() const override;
    int getHpRestore() const;
    int getMpRestore() const;
    bool getClearsStatus() const;

private:
    int hpRestore;
    int mpRestore;
    bool clearsStatus;
};

class Equipment : public Item
{
public:
    Equipment(std::string name, ItemType type, int price, int atkBonus, int defBonus);

    std::string getDescription() const override;
    int getAtkBonus() const;
    int getDefBonus() const;

    // 저주 관련
    bool isCursed() const;
    CursePenalty getCursePenalty() const;
    int getCursePenaltyValue() const;           // HpPercent/MpPercent면 %, Agility면 절대값
    std::string getCurseDescription() const;    // 한국어 설명 ("최대 HP -15%" 등)
    void setCurse(CursePenalty penalty, int value);

private:
    int atkBonus;
    int defBonus;
    bool cursed = false;
    CursePenalty cursePenalty = CursePenalty::HpPercent;
    int cursePenaltyValue = 0;
};

namespace ItemFactory
{
    std::unique_ptr<Item> createSmallPotion();
    std::unique_ptr<Item> createLargePotion();
    std::unique_ptr<Item> createMpPotion();
    std::unique_ptr<Item> createLargeMpPotion();

    std::unique_ptr<Item> createWeaponForJob(JobType job, int tier);
    std::unique_ptr<Item> createArmorForJob(JobType job, int tier);

    std::unique_ptr<Item> createCursedWeaponForJob(JobType job, int tier);
    std::unique_ptr<Item> createCursedArmorForJob(JobType job, int tier);

    std::unique_ptr<Item> randomTreasure(JobType playerJob, int floor);
}
