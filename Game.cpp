#include "Game.h"
#include "Character.h"
#include "Dungeon.h"
#include "Inventory.h"
#include "UI.h"
#include "Item.h"

#include <iostream>

Game::Game() = default;
Game::~Game() = default;

void Game::run()
{
    while (true)
    {
        showMainMenu();
        int choice = UI::askChoice("선택", 1, 2);
        if (choice == 2) break;

        if (!startNewGame()) continue;
        mainLoop();
    }

    UI::printLine("게임을 종료합니다. 감사합니다!", UI::Color::Cyan);
}

void Game::showMainMenu()
{
    UI::clear();
    UI::printTitle("Dungeon Adventure");
    UI::printLine("  1. 새 게임 시작", UI::Color::White);
    UI::printLine("  2. 종료", UI::Color::White);
    std::cout << "\n";
}

bool Game::startNewGame()
{
    UI::clear();
    UI::printTitle("캐릭터 생성");

    std::string name = UI::askString("이름을 입력하세요");

    UI::printLine("\n[직업 선택]", UI::Color::Cyan);
    UI::printLine("  1. 전사 (HP 120, 공격 15, 방어 12)", UI::Color::White);
    UI::printLine("  2. 마법사 (HP 80, MP 110, 공격 10, 마법 강력)", UI::Color::White);
    UI::printLine("  3. 궁수 (HP 90, 민첩 15, 크리티컬 유리)", UI::Color::White);

    int jobChoice = UI::askChoice("선택", 1, 3);
    JobType job = static_cast<JobType>(jobChoice - 1);

    player = std::make_unique<Character>(name, job);
    dungeon = std::make_unique<Dungeon>();
    inventory = std::make_unique<Inventory>();

    inventory->addItem(ItemFactory::createSmallPotion());
    inventory->addItem(ItemFactory::createSmallPotion());
    inventory->addItem(ItemFactory::createMpPotion());

    UI::clear();
    UI::printTitle("모험의 시작");
    UI::printLine(player->getName() + " [" + player->getJobName() +
        "] 의 모험이 시작됩니다!", UI::Color::Yellow);
    UI::printLine("작은 체력 물약 2개, 마나 물약 1개를 지급받았습니다.", UI::Color::Green);
    UI::pause();
    return true;
}

void Game::mainLoop()
{
    while (player->isAlive() && !dungeon->isComplete())
    {
        UI::clear();
        UI::printTitle("던전 " + std::to_string(dungeon->getCurrentFloor()) + "층");
        UI::printHpMpBar(player->getStats().hp, player->getStats().maxHp, "HP", UI::Color::Green);
        std::cout << "  ";
        UI::printHpMpBar(player->getStats().mp, player->getStats().maxMp, "MP", UI::Color::Cyan);
        std::cout << "\n골드: " << player->getGold() << "G  Lv." << player->getLevel() << "\n\n";

        UI::printLine("1. 다음 방으로 진행", UI::Color::White);
        UI::printLine("2. 상태 확인", UI::Color::White);
        UI::printLine("3. 인벤토리", UI::Color::White);
        UI::printLine("4. 모험 포기", UI::Color::White);

        int choice = UI::askChoice("선택", 1, 4);
        switch (choice)
        {
        case 1:
            dungeon->enterNextRoom(*player, *inventory);
            break;
        case 2:
            showStatus();
            break;
        case 3:
            showInventoryMenu();
            break;
        case 4:
            if (UI::askYesNo("정말 포기하시겠습니까?"))
            {
                UI::printLine("던전을 탈출했습니다.", UI::Color::Yellow);
                UI::pause();
                return;
            }
            break;
        }
    }

    if (dungeon->isComplete() && player->isAlive())
    {
        gameOver(true);
    }
    else if (!player->isAlive())
    {
        gameOver(false);
    }
}

void Game::showStatus()
{
    UI::clear();
    player->printStatus();

    UI::printLine("\n[보유 스킬]", UI::Color::Cyan);
    for (const auto& skill : player->getSkills())
    {
        std::cout << "  - " << skill.getName()
            << " (MP " << skill.getMpCost() << ") - "
            << skill.getDescription() << "\n";
    }
    UI::pause();
}

void Game::showInventoryMenu()
{
    while (true)
    {
        UI::clear();
        UI::printTitle("인벤토리 (" + std::to_string(inventory->size()) + "/10)");
        inventory->print();

        if (inventory->isEmpty())
        {
            UI::pause();
            return;
        }

        std::cout << "\n  " << (inventory->size() + 1) << ". 돌아가기\n";
        int choice = UI::askChoice("사용할 아이템 선택",
            1, static_cast<int>(inventory->size() + 1));

        if (choice == static_cast<int>(inventory->size() + 1)) return;

        Item* item = inventory->getItem(choice - 1);
        if (item->getType() != ItemType::Potion)
        {
            UI::printLine("던전 밖에서 장비는 자동 장착되지 않습니다. 상점에서 장비를 구매하면 즉시 장착됩니다.", UI::Color::Gray);
            UI::pause();
            continue;
        }

        Potion* potion = static_cast<Potion*>(item);
        if (potion->getHpRestore() > 0)
        {
            player->heal(potion->getHpRestore());
            UI::printLine("HP " + std::to_string(potion->getHpRestore()) + " 회복!",
                UI::Color::Green);
        }
        if (potion->getMpRestore() > 0)
        {
            player->restoreMp(potion->getMpRestore());
            UI::printLine("MP " + std::to_string(potion->getMpRestore()) + " 회복!",
                UI::Color::Cyan);
        }
        if (potion->getClearsStatus())
        {
            player->clearStatuses();
            UI::printLine("모든 상태이상이 해제되었습니다!", UI::Color::Green);
        }
        inventory->removeItem(choice - 1);
        UI::pause();
    }
}

void Game::gameOver(bool victory)
{
    UI::clear();
    if (victory)
    {
        UI::printTitle("*** 던전 정복! ***");
        UI::printLine("축하합니다, " + player->getName() +
            "! 모든 층을 제패했습니다!", UI::Color::Yellow);
        UI::printLine("최종 레벨: " + std::to_string(player->getLevel()), UI::Color::Green);
        UI::printLine("획득 골드: " + std::to_string(player->getGold()) + "G", UI::Color::Green);
    }
    else
    {
        UI::printTitle("GAME OVER");
        UI::printLine(player->getName() + "의 모험은 여기서 끝났습니다...", UI::Color::Red);
        UI::printLine("도달 층: " + std::to_string(dungeon->getCurrentFloor()), UI::Color::Gray);
    }
    UI::pause();
}
