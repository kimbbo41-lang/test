#include "Dungeon.h"
#include "Character.h"
#include "Monster.h"
#include "Combat.h"
#include "Inventory.h"
#include "Item.h"
#include "GameConfig.h"
#include "UI.h"

#include <iostream>

Dungeon::Dungeon()
    : currentFloor(1), currentRoom(0), complete(false)
{
    generateFloor();
}

int Dungeon::getCurrentFloor() const { return currentFloor; }
int Dungeon::getCurrentRoom() const { return currentRoom; }
int Dungeon::getTotalFloors() const { return Config::TOTAL_FLOORS; }
bool Dungeon::isComplete() const { return complete; }

void Dungeon::generateFloor()
{
    floorRooms.clear();
    int roomCount = randomInt(Config::MIN_ROOMS_PER_FLOOR, Config::MAX_ROOMS_PER_FLOOR);

    for (int i = 0; i < roomCount; ++i)
    {
        Room r;
        r.event = rollEvent();
        floorRooms.push_back(r);
    }

    Room boss;
    boss.event = EventType::Monster;
    boss.isBossRoom = true;
    floorRooms.push_back(boss);

    currentRoom = 0;
}

EventType Dungeon::rollEvent() const
{
    int total = Config::EVENT_MONSTER_WEIGHT + Config::EVENT_TREASURE_WEIGHT +
        Config::EVENT_TRAP_WEIGHT + Config::EVENT_REST_WEIGHT + Config::EVENT_SHOP_WEIGHT;
    int roll = randomInt(1, total);

    int cutoff = Config::EVENT_MONSTER_WEIGHT;
    if (roll <= cutoff) return EventType::Monster;
    cutoff += Config::EVENT_TREASURE_WEIGHT;
    if (roll <= cutoff) return EventType::Treasure;
    cutoff += Config::EVENT_TRAP_WEIGHT;
    if (roll <= cutoff) return EventType::Trap;
    cutoff += Config::EVENT_REST_WEIGHT;
    if (roll <= cutoff) return EventType::Rest;
    return EventType::Shop;
}

bool Dungeon::enterNextRoom(Character& player, Inventory& inventory)
{
    if (currentRoom >= static_cast<int>(floorRooms.size()))
    {
        if (currentFloor >= Config::TOTAL_FLOORS)
        {
            complete = true;
            return true;
        }
        ++currentFloor;
        generateFloor();
        UI::clear();
        UI::printTitle(std::to_string(currentFloor) + "층에 도착했습니다!");
        UI::pause();
    }

    const Room& room = floorRooms[currentRoom];
    ++currentRoom;

    UI::clear();
    UI::printTitle("던전 " + std::to_string(currentFloor) + "층 - " +
        std::to_string(currentRoom) + "/" + std::to_string(floorRooms.size()) + " 방");

    if (room.isBossRoom)
    {
        UI::printLine(">>> 보스 방! <<<", UI::Color::Red);
        return handleMonster(player, inventory, true);
    }

    switch (room.event)
    {
    case EventType::Monster:
        UI::printLine("몬스터가 나타났다!", UI::Color::Red);
        return handleMonster(player, inventory, false);
    case EventType::Treasure:
        UI::printLine("보물 상자를 발견했다!", UI::Color::Yellow);
        handleTreasure(player, inventory);
        break;
    case EventType::Trap:
        UI::printLine("함정에 걸렸다!", UI::Color::Red);
        handleTrap(player);
        break;
    case EventType::Rest:
        UI::printLine("안전한 휴식처를 발견했다.", UI::Color::Green);
        handleRest(player);
        break;
    case EventType::Shop:
        UI::printLine("떠돌이 상인을 만났다.", UI::Color::Cyan);
        handleShop(player, inventory);
        break;
    }

    UI::pause();
    return player.isAlive();
}

bool Dungeon::handleMonster(Character& player, Inventory& inventory, bool boss)
{
    Monster monster = boss
        ? MonsterFactory::createBoss(currentFloor)
        : MonsterFactory::createForFloor(currentFloor);

    UI::printLine(monster.getName() + "과(와)의 전투가 시작된다!",
        boss ? UI::Color::Red : UI::Color::Yellow);
    UI::pause();

    CombatEngine engine(player, monster, inventory);
    CombatResult result = engine.run();

    switch (result)
    {
    case CombatResult::Victory:
    {
        int expR = monster.getExpReward();
        int goldR = monster.getGoldReward();
        player.gainExp(expR);
        player.gainGold(goldR);
        UI::printLine("\n승리! 경험치 " + std::to_string(expR) +
            ", 골드 " + std::to_string(goldR) + " 획득.", UI::Color::Green);
        UI::pause();
        return true;
    }
    case CombatResult::Defeat:
        UI::printLine("\n패배했습니다...", UI::Color::Red);
        return false;
    case CombatResult::Escape:
        UI::printLine("\n도망쳤습니다.", UI::Color::Yellow);
        UI::pause();
        return true;
    }
    return true;
}

void Dungeon::handleTreasure(Character& player, Inventory& inventory)
{
    if (randomInt(1, 100) <= 30)
    {
        int gold = randomInt(10, 30) * currentFloor;
        player.gainGold(gold);
        UI::printLine("골드 " + std::to_string(gold) + " 획득!", UI::Color::Yellow);
        return;
    }

    auto item = ItemFactory::randomTreasure(player.getJob(), currentFloor);
    UI::printLine(item->getName() + "을(를) 발견했다!", UI::Color::Yellow);

    if (inventory.isFull())
    {
        UI::printLine("인벤토리가 가득 차서 획득할 수 없습니다.", UI::Color::Red);
    }
    else
    {
        inventory.addItem(std::move(item));
        UI::printLine("인벤토리에 추가되었습니다.", UI::Color::Green);
    }
}

void Dungeon::handleTrap(Character& player)
{
    int dmg = randomInt(Config::TRAP_MIN_DAMAGE, Config::TRAP_MAX_DAMAGE) + currentFloor * 2;
    player.takeDamage(dmg);
    UI::printLine(std::to_string(dmg) + " 데미지를 입었다!", UI::Color::Red);

    if (!player.isAlive())
    {
        UI::printLine("함정에 의해 쓰러졌다...", UI::Color::Red);
    }
}

void Dungeon::handleRest(Character& player)
{
    player.rest();
    UI::printLine("HP와 MP가 일부 회복되었습니다.", UI::Color::Green);
}

void Dungeon::handleShop(Character& player, Inventory& inventory)
{
    while (true)
    {
        UI::clear();
        UI::printTitle("상점");
        std::cout << "보유 골드: " << player.getGold() << "G\n\n";

        UI::printLine("[판매 물품]", UI::Color::Cyan);
        std::cout << "  1. 작은 체력 물약 (HP+40) - " << Config::SMALL_POTION_PRICE << "G\n";
        std::cout << "  2. 큰 체력 물약 (HP+100) - " << Config::LARGE_POTION_PRICE << "G\n";
        std::cout << "  3. 마나 물약 (MP+50) - " << Config::MP_POTION_PRICE << "G\n";
        std::cout << "  4. 큰 마나 물약 (MP+120) - " << Config::LARGE_MP_POTION_PRICE << "G\n";

        auto weapon = ItemFactory::createWeaponForJob(player.getJob(), currentFloor >= 4 ? 3 : currentFloor >= 2 ? 2 : 1);
        auto armor  = ItemFactory::createArmorForJob(player.getJob(), currentFloor >= 4 ? 3 : currentFloor >= 2 ? 2 : 1);
        std::cout << "  5. " << weapon->getDescription() << "\n";
        std::cout << "  6. " << armor->getDescription() << "\n";
        std::cout << "  7. 아이템 판매 (구매가의 50%)\n";
        std::cout << "  8. 나가기\n";

        int choice = UI::askChoice("선택", 1, 8);
        if (choice == 8) return;
        if (choice == 7) { handleSell(player, inventory); continue; }

        std::unique_ptr<Item> bought;
        switch (choice)
        {
        case 1: bought = ItemFactory::createSmallPotion();    break;
        case 2: bought = ItemFactory::createLargePotion();    break;
        case 3: bought = ItemFactory::createMpPotion();       break;
        case 4: bought = ItemFactory::createLargeMpPotion();  break;
        case 5: bought = std::move(weapon); break;
        case 6: bought = std::move(armor);  break;
        }

        if (!player.spendGold(bought->getPrice()))
        {
            UI::printLine("골드가 부족합니다.", UI::Color::Red);
            UI::pause();
            continue;
        }

        if (bought->getType() == ItemType::Weapon || bought->getType() == ItemType::Armor)
        {
            Equipment* eqPtr = static_cast<Equipment*>(bought.release());
            std::unique_ptr<Equipment> eq(eqPtr);
            auto old = (eq->getType() == ItemType::Weapon)
                ? player.equipWeapon(std::move(eq))
                : player.equipArmor(std::move(eq));
            UI::printLine("장착 완료!", UI::Color::Green);
            if (old) UI::printLine("기존 " + old->getName() + "은(는) 버려졌습니다.", UI::Color::Gray);
        }
        else
        {
            if (inventory.isFull())
            {
                UI::printLine("인벤토리가 가득 찼습니다. 구매 취소.", UI::Color::Red);
                player.gainGold(bought->getPrice());
            }
            else
            {
                inventory.addItem(std::move(bought));
                UI::printLine("구매 완료!", UI::Color::Green);
            }
        }
        UI::pause();
    }
}

void Dungeon::handleSell(Character& player, Inventory& inventory)
{
    if (inventory.isEmpty())
    {
        UI::printLine("판매할 아이템이 없습니다.", UI::Color::Gray);
        UI::pause();
        return;
    }

    UI::clear();
    UI::printTitle("아이템 판매");
    std::cout << "보유 골드: " << player.getGold() << "G\n\n";
    UI::printLine("[판매 가능 아이템 - 표시 가격의 50%]", UI::Color::Cyan);

    for (size_t i = 0; i < inventory.size(); ++i)
    {
        Item* it = inventory.getItem(i);
        int sellPrice = it->getPrice() / 2;
        std::cout << "  " << (i + 1) << ". " << it->getDescription()
            << " -> 판매가 " << sellPrice << "G\n";
    }
    std::cout << "  " << (inventory.size() + 1) << ". 취소\n";

    int choice = UI::askChoice("선택", 1, static_cast<int>(inventory.size() + 1));
    if (choice == static_cast<int>(inventory.size() + 1)) return;

    Item* target = inventory.getItem(choice - 1);
    int sellPrice = target->getPrice() / 2;
    std::string itemName = target->getName();

    inventory.removeItem(choice - 1);
    player.gainGold(sellPrice);

    UI::printLine(itemName + " 을(를) " + std::to_string(sellPrice) +
        "G 에 판매했습니다.", UI::Color::Yellow);
    UI::pause();
}
