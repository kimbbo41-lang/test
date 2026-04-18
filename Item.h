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
    Potion(std::string name, int price, int hpRestore, int mpRestore);

    std::string getDescription() const override;
    int getHpRestore() const;
    int getMpRestore() const;

private:
    int hpRestore;
    int mpRestore;
};

class Equipment : public Item
{
public:
    Equipment(std::string name, ItemType type, int price, int atkBonus, int defBonus);

    std::string getDescription() const override;
    int getAtkBonus() const;
    int getDefBonus() const;

private:
    int atkBonus;
    int defBonus;
};

namespace ItemFactory
{
    std::unique_ptr<Item> createSmallPotion();
    std::unique_ptr<Item> createLargePotion();
    std::unique_ptr<Item> createMpPotion();
    std::unique_ptr<Item> createLargeMpPotion();

    std::unique_ptr<Item> createWeaponForJob(JobType job, int tier);
    std::unique_ptr<Item> createArmorForJob(JobType job, int tier);

    std::unique_ptr<Item> randomTreasure(JobType playerJob, int floor);
}
