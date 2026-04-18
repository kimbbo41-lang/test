#pragma once

#include "Common.h"

#include <string>
#include <vector>

class Skill
{
public:
    Skill(std::string name, SkillType type, int mpCost, double multiplier, std::string description);

    const std::string& getName() const;
    const std::string& getDescription() const;
    SkillType getType() const;
    int getMpCost() const;
    double getMultiplier() const;

private:
    std::string name;
    std::string description;
    SkillType type;
    int mpCost;
    double multiplier;
};

namespace SkillLibrary
{
    std::vector<Skill> getSkillsForJob(JobType job);
}
