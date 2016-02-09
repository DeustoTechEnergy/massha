#include "NormalDistributionSkill.h"

#include <random>

NormalDistributionSkill::NormalDistributionSkill() : Skill() {
}

NormalDistributionSkill::~NormalDistributionSkill() {
}

/**********************************************************************
 METHODS
**********************************************************************/

/** Given a mean and a deviation, returns a double deviated
 * from the initial value following a normal function
 * @brief normalDistribution
 * @param mean
 * @param deviation
 * @return
 */
double NormalDistributionSkill::normalDistribution( double mean, double deviation ){
    std::random_device rd;
    std::mt19937 generator(rd());
    std::normal_distribution<double> distribution(mean , deviation);
    return distribution( generator );
}


