#include "UniformDistributionSkill.h"

UniformDistributionSkill::UniformDistributionSkill(){
    // Initialize uniform distribution generator
    this->uniform_distribution = std::uniform_real_distribution<double> (0.0,1.0);
}

UniformDistributionSkill::~UniformDistributionSkill(){
}

/** Generate value from uniform distribution
 * @brief uniformDistribution
 * @return
 */
double UniformDistributionSkill::uniformDistribution(){
    std::random_device rd;
    std::mt19937 generator(rd());
    return this->uniform_distribution( generator );
}
