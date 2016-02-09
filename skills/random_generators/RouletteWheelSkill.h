#ifndef ROULETEWHEELSKILL_H
#define ROULETEWHEELSKILL_H

#include <QList>

#include "skills/Skill.h"
#include "skills/random_generators/UniformDistributionSkill.h"

class RouletteWheelSkill : public Skill
{
    Q_OBJECT

public:

    RouletteWheelSkill();
    ~RouletteWheelSkill();

    // GETTERS
    bool isEmpty();

    // SETTERS
    void addElement( QObject* obj, double frequency );

    // METHODS
    QObject* getElement();
    QObject* getElement( double frequency );

private:
    QList<QObject*> roulette;   // pair of object and frequency
    QList<double> ordered_frequencies; // frequencies ordered in ascending order
    double sum_frequencies;
    UniformDistributionSkill* uniform_distribution_skill;
};

#endif // ROULETEWHEELSKILL_H
