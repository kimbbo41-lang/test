#include "Skill.h"

Skill::Skill(std::string name, SkillType type, int mpCost, double multiplier, std::string description)
    : name(std::move(name))
    , description(std::move(description))
    , type(type)
    , mpCost(mpCost)
    , multiplier(multiplier)
{
}

const std::string& Skill::getName() const { return name; }
const std::string& Skill::getDescription() const { return description; }
SkillType Skill::getType() const { return type; }
int Skill::getMpCost() const { return mpCost; }
double Skill::getMultiplier() const { return multiplier; }

namespace SkillLibrary
{
    std::vector<Skill> getSkillsForJob(JobType job)
    {
        switch (job)
        {
        case JobType::Warrior:
            return {
                Skill("강타",       SkillType::Physical, 8,  1.8, "강력한 일격 (물리 180%)"),
                Skill("회오리베기", SkillType::Physical, 15, 2.3, "광역 공격 (물리 230%)"),
                Skill("전투의 함성", SkillType::Heal,    12, 0.2, "HP 20% 회복")
            };
        case JobType::Mage:
            return {
                Skill("파이어볼",   SkillType::Magic,  8, 2.0, "화염구 (마법 200%)"),
                Skill("아이스 스톰", SkillType::Magic, 20, 2.8, "얼음 폭풍 (마법 280%)"),
                Skill("치유의 빛", SkillType::Heal,  12, 0.4, "HP 40% 회복")
            };
        case JobType::Archer:
            return {
                Skill("정밀사격",   SkillType::Physical, 8,  2.2, "약점 사격 (물리 220%)"),
                Skill("연속사격",   SkillType::Physical, 20, 2.6, "3연발 (물리 260%)"),
                Skill("응급처치", SkillType::Heal,   10, 0.30, "HP 30% 회복")
            };
        }
        return {};
    }
}
