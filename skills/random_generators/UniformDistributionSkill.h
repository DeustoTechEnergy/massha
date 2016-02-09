#ifndef UNIFORMDISTRIBUTIONSKILL_H
#define UNIFORMDISTRIBUTIONSKILL_H

#include "skills/Skill.h"

class UniformDistributionSkill : public Skill
{
    Q_OBJECT

public:
    UniformDistributionSkill();
    ~UniformDistributionSkill();

    // METHODS
    double uniformDistribution();

private:
    std::uniform_real_distribution<double> uniform_distribution;
};

#endif // UNIFORMDISTRIBUTIONSKILL_H
