#ifndef LEASTSQUARESSOLVINGSKILL
#define LEASTSQUARESSOLVINGSKILL

#include <QVector>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/SVD>

#include "skills/Skill.h"

using namespace Eigen;

class LeastSquaresSolvingSkill : public Skill
{
    Q_OBJECT
public:

    LeastSquaresSolvingSkill();
    ~LeastSquaresSolvingSkill();

    // METHODS
    QVector<double> leastSquaresSolving(QVector<QVector<double> > matrix, QVector<double> values);
};

#endif // LEASTSQUARESSOLVINGSKILL

