#include "Combat.h"
#include "Character.h"
#include "Monster.h"
#include "Inventory.h"
#include "GameConfig.h"
#include "UI.h"

#include <algorithm>
#include <iostream>

CombatEngine::CombatEngine(Character& player, Monster& monster, Inventory& inventory)
    : player(player), monster(monster), inventory(inventory)
{
}

CombatResult CombatEngine::run()
{
    UI::clear();
    UI::printTitle("전투 시작! VS " + monster.getName());

    while (player.isAlive() && monster.isAlive())
    {
        showCombatStatus();

        bool turnEnded = false;
        while (!turnEnded)
        {
            UI::printLine("\n[행동 선택] 1.공격  2.스킬  3.아이템  4.도망", UI::Color::Yellow);
            int choice = UI::askChoice("선택", 1, 4);
            switch (choice)
            {
            case 1: doAttack(); turnEnded = true; break;
            case 2: turnEnded = doSkill(); break;
            case 3: turnEnded = doItem(); break;
            case 4:
                if (doEscape()) return CombatResult::Escape;
                turnEnded = true;
                break;
            }
        }

        if (!monster.isAlive()) break;

        monsterTurn();
    }

    if (!player.isAlive()) return CombatResult::Defeat;
    return CombatResult::Victory;
}

void CombatEngine::showCombatStatus() const
{
    UI::printDivider();
    UI::printColored("[" + player.getName() + "] ", UI::Color::Green);
    UI::printHpMpBar(player.getStats().hp, player.getStats().maxHp, "HP", UI::Color::Green);
    std::cout << "  ";
    UI::printHpMpBar(player.getStats().mp, player.getStats().maxMp, "MP", UI::Color::Cyan);
    std::cout << "\n";
    UI::printColored("[" + monster.getName() + "] ", UI::Color::Red);
    UI::printHpMpBar(monster.getStats().hp, monster.getStats().maxHp, "HP", UI::Color::Red);
    std::cout << "\n";
    UI::printDivider();
}

int CombatEngine::rollDamage(int attack, int defense, int attackerAgi, bool& isCrit)
{
    int raw = std::max(1, attack - defense / 2);
    double rollMult = Config::DAMAGE_MIN_MULTIPLIER +
        randomDouble() * (Config::DAMAGE_MAX_MULTIPLIER - Config::DAMAGE_MIN_MULTIPLIER);

    double critChance = std::min(Config::MAX_CRIT_RATE,
        attackerAgi * Config::CRIT_RATE_PER_AGI);
    isCrit = (randomDouble() < critChance);

    double dmg = raw * rollMult;
    if (isCrit) dmg *= Config::CRIT_DAMAGE_MULTIPLIER;
    return std::max(1, static_cast<int>(dmg));
}

void CombatEngine::doAttack()
{
    bool crit = false;
    int dmg = rollDamage(player.getEffectiveAttack(),
        monster.getStats().defense,
        player.getStats().agility, crit);
    monster.takeDamage(dmg);

    std::string msg = player.getName() + "의 공격! " + monster.getName() +
        "에게 " + std::to_string(dmg) + " 데미지.";
    if (crit) msg = "크리티컬! " + msg;
    UI::printLine(msg, crit ? UI::Color::Yellow : UI::Color::White);
}

bool CombatEngine::doSkill()
{
    const auto& skills = player.getSkills();
    if (skills.empty())
    {
        UI::printLine("사용할 스킬이 없습니다.", UI::Color::Gray);
        return false;
    }

    UI::printLine("\n[스킬 목록]", UI::Color::Cyan);
    for (size_t i = 0; i < skills.size(); ++i)
    {
        std::cout << "  " << (i + 1) << ". " << skills[i].getName()
            << " (MP " << skills[i].getMpCost() << ") - "
            << skills[i].getDescription() << "\n";
    }
    std::cout << "  " << (skills.size() + 1) << ". 취소\n";

    int choice = UI::askChoice("선택", 1, static_cast<int>(skills.size() + 1));
    if (choice == static_cast<int>(skills.size() + 1)) return false;

    const Skill& skill = skills[choice - 1];
    if (!player.consumeMp(skill.getMpCost()))
    {
        UI::printLine("MP가 부족합니다!", UI::Color::Red);
        return false;
    }

    if (skill.getType() == SkillType::Heal)
    {
        int healAmount = static_cast<int>(player.getStats().maxHp * skill.getMultiplier());
        player.heal(healAmount);
        UI::printLine(player.getName() + "은(는) " + skill.getName() + "으로 HP " +
            std::to_string(healAmount) + " 회복!", UI::Color::Green);
    }
    else
    {
        bool crit = false;
        int baseAtk = static_cast<int>(player.getEffectiveAttack() * skill.getMultiplier());
        int dmg = rollDamage(baseAtk, monster.getStats().defense,
            player.getStats().agility, crit);
        monster.takeDamage(dmg);

        std::string msg = player.getName() + "의 " + skill.getName() + "! " +
            monster.getName() + "에게 " + std::to_string(dmg) + " 데미지.";
        if (crit) msg = "크리티컬! " + msg;
        UI::printLine(msg, crit ? UI::Color::Yellow : UI::Color::Magenta);
    }
    return true;
}

bool CombatEngine::doItem()
{
    if (inventory.isEmpty())
    {
        UI::printLine("인벤토리가 비어있습니다.", UI::Color::Gray);
        return false;
    }

    UI::printLine("\n[인벤토리]", UI::Color::Cyan);
    inventory.print();
    std::cout << "  " << (inventory.size() + 1) << ". 취소\n";

    int choice = UI::askChoice("선택", 1, static_cast<int>(inventory.size() + 1));
    if (choice == static_cast<int>(inventory.size() + 1)) return false;

    Item* item = inventory.getItem(choice - 1);
    if (item->getType() != ItemType::Potion)
    {
        UI::printLine("전투 중에는 물약만 사용할 수 있습니다.", UI::Color::Red);
        return false;
    }

    Potion* potion = static_cast<Potion*>(item);
    if (potion->getHpRestore() > 0)
    {
        player.heal(potion->getHpRestore());
        UI::printLine("HP " + std::to_string(potion->getHpRestore()) + " 회복!",
            UI::Color::Green);
    }
    if (potion->getMpRestore() > 0)
    {
        player.restoreMp(potion->getMpRestore());
        UI::printLine("MP " + std::to_string(potion->getMpRestore()) + " 회복!",
            UI::Color::Cyan);
    }
    inventory.removeItem(choice - 1);
    return true;
}

bool CombatEngine::doEscape()
{
    if (monster.isBoss())
    {
        UI::printLine("보스에게서는 도망칠 수 없습니다!", UI::Color::Red);
        return false;
    }

    double chance = Config::ESCAPE_BASE_RATE +
        player.getStats().agility * Config::ESCAPE_AGI_BONUS;
    if (randomDouble() < chance)
    {
        UI::printLine("도망에 성공했습니다!", UI::Color::Yellow);
        return true;
    }

    UI::printLine("도망 실패!", UI::Color::Red);
    return false;
}

void CombatEngine::monsterTurn()
{
    if (!monster.isAlive()) return;

    bool crit = false;
    int dmg = rollDamage(monster.getStats().attack,
        player.getEffectiveDefense(),
        monster.getStats().agility, crit);
    player.takeDamage(dmg);

    std::string msg = monster.getName() + "의 공격! " + player.getName() +
        "은(는) " + std::to_string(dmg) + " 데미지를 입었다.";
    if (crit) msg = "크리티컬! " + msg;
    UI::printLine(msg, crit ? UI::Color::Yellow : UI::Color::Red);
}
