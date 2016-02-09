#ifndef SKILL_H
#define SKILL_H

#include <QObject>
#include <QDebug>
#include "skills/SkillConstants.h"

/**
 * @brief The Skill class
 * Empty skill which is parent of all skills
 */
class Skill : public QObject
{
    Q_OBJECT
public:
    QList<SkillConstants::capabilities> getSkillActions();

protected:
    Skill(QObject* parent = 0);
    ~Skill();

private:
    // Actions the skill will enable an agent to do.
    QList<SkillConstants::capabilities> actions;

};

#endif // SKILL_H
