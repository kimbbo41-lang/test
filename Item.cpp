#include "Item.h"
#include "GameConfig.h"

#include <algorithm>

Item::Item(std::string name, ItemType type, int price)
    : name(std::move(name)), type(type), price(price)
{
}

const std::string& Item::getName() const { return name; }
ItemType Item::getType() const { return type; }
int Item::getPrice() const { return price; }

Potion::Potion(std::string name, int price, int hpRestore, int mpRestore)
    : Item(std::move(name), ItemType::Potion, price)
    , hpRestore(hpRestore)
    , mpRestore(mpRestore)
{
}

std::string Potion::getDescription() const
{
    std::string desc = name + " (";
    bool first = true;
    if (hpRestore > 0) { desc += "HP +" + std::to_string(hpRestore); first = false; }
    if (mpRestore > 0) {
        if (!first) desc += ", ";
        desc += "MP +" + std::to_string(mpRestore);
    }
    desc += ", " + std::to_string(price) + "G)";
    return desc;
}

int Potion::getHpRestore() const { return hpRestore; }
int Potion::getMpRestore() const { return mpRestore; }

Equipment::Equipment(std::string name, ItemType type, int price, int atkBonus, int defBonus)
    : Item(std::move(name), type, price)
    , atkBonus(atkBonus)
    , defBonus(defBonus)
{
}

std::string Equipment::getDescription() const
{
    std::string desc = name + " (";
    if (atkBonus > 0) desc += "공격 +" + std::to_string(atkBonus) + " ";
    if (defBonus > 0) desc += "방어 +" + std::to_string(defBonus) + " ";
    desc += std::to_string(price) + "G)";
    return desc;
}

int Equipment::getAtkBonus() const { return atkBonus; }
int Equipment::getDefBonus() const { return defBonus; }

namespace ItemFactory
{
    std::unique_ptr<Item> createSmallPotion()
    {
        return std::make_unique<Potion>("작은 체력 물약", Config::SMALL_POTION_PRICE, 40, 0);
    }

    std::unique_ptr<Item> createLargePotion()
    {
        return std::make_unique<Potion>("큰 체력 물약", Config::LARGE_POTION_PRICE, 100, 0);
    }

    std::unique_ptr<Item> createMpPotion()
    {
        return std::make_unique<Potion>("마나 물약", Config::MP_POTION_PRICE, 0, 50);
    }

    std::unique_ptr<Item> createLargeMpPotion()
    {
        return std::make_unique<Potion>("큰 마나 물약", Config::LARGE_MP_POTION_PRICE, 0, 120);
    }

    std::unique_ptr<Item> createWeaponForJob(JobType job, int tier)
    {
        tier = std::clamp(tier, 1, 3);
        int atkBonus = 3 + tier * 3;
        int price = 50 + tier * 40;
        std::string suffix = (tier == 1 ? "(조악한)" : tier == 2 ? "(정교한)" : "(전설의)");

        std::string base;
        switch (job)
        {
        case JobType::Warrior: base = "검"; break;
        case JobType::Mage:    base = "지팡이"; break;
        case JobType::Archer:  base = "활"; break;
        }
        return std::make_unique<Equipment>(suffix + " " + base, ItemType::Weapon, price, atkBonus, 0);
    }

    std::unique_ptr<Item> createArmorForJob(JobType job, int tier)
    {
        tier = std::clamp(tier, 1, 3);
        int defBonus = 2 + tier * 2;
        int price = 40 + tier * 35;
        std::string suffix = (tier == 1 ? "(가죽)" : tier == 2 ? "(강철)" : "(미스릴)");

        std::string base;
        switch (job)
        {
        case JobType::Warrior: base = "갑옷"; break;
        case JobType::Mage:    base = "로브"; break;
        case JobType::Archer:  base = "경갑"; break;
        }
        return std::make_unique<Equipment>(suffix + " " + base, ItemType::Armor, price, 0, defBonus);
    }

    std::unique_ptr<Item> randomTreasure(JobType playerJob, int floor)
    {
        int roll = randomInt(1, 100);
        int tier = std::clamp((floor + 1) / 2, 1, 3);

        if (roll <= 40) return createSmallPotion();
        if (roll <= 65) return createLargePotion();
        if (roll <= 85) return createMpPotion();
        if (roll <= 92) return createWeaponForJob(playerJob, tier);
        return createArmorForJob(playerJob, tier);
    }
}
