#ifndef NORMALDISTRIBUTIONSKILL
#define NORMALDISTRIBUTIONSKILL

#include <QObject>

#include "skills/Skill.h"

class NormalDistributionSkill : public Skill
{
    Q_OBJECT
public:

    NormalDistributionSkill();
    ~NormalDistributionSkill();

    // METHODS
    double normalDistribution(double mean, double deviation);
};

#endif // NORMALDISTRIBUTIONSKILL

