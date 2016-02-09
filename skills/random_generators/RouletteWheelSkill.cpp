#include "RouletteWheelSkill.h"
#include "environment/Environment.h"

#include <QTime>

RouletteWheelSkill::RouletteWheelSkill() : Skill() {
    this->sum_frequencies = 0;
    this->ordered_frequencies.empty();
    this->roulette.empty();
    this->uniform_distribution_skill = new UniformDistributionSkill();
}

RouletteWheelSkill::~RouletteWheelSkill() {
    this->uniform_distribution_skill->deleteLater();
}


/**********************************************************************
 GETTERS
**********************************************************************/

bool RouletteWheelSkill::isEmpty(){
    return this->roulette.isEmpty();
}

/**********************************************************************
 SETTERS
**********************************************************************/

/**
 * Adds element to QMap of <Object, frequencies>
 * @brief RouletteWheelSkill::addElement
 * @param frequency
 * @param obj
 */
void RouletteWheelSkill::addElement( QObject* obj, double frequency ){   
    if ( this->ordered_frequencies.isEmpty() || this->ordered_frequencies.last() <= frequency ){
        this->ordered_frequencies.append( frequency );
        this->roulette.append( obj );

    } else {

        // Insert frequency in ascending order
        for ( int i = 0; i < this->ordered_frequencies.size(); i++ ){

            if( frequency <= this->ordered_frequencies.at(i) ){
                this->ordered_frequencies.insert( i , frequency );
                this->roulette.insert( i , obj );
                break;
            }

        }
    }
    this->sum_frequencies += frequency;
}


/**********************************************************************
 METHODS
**********************************************************************/

/**
 * Generates a random value to get random element
 * based on roulette wheel selection
 * @brief RouletteWheelSkill::getElement
 * @return
 */
QObject* RouletteWheelSkill::getElement(){
    double value = this->uniform_distribution_skill->uniformDistribution(); // Returns from 0 to 1
    return getElement( value );
}

/**
 * Retrieves the element which frequency falls within the value
 * passed as parameter
 * @brief RouletteWheelSkill::getElement
 * @param value
 * @return
 */
QObject* RouletteWheelSkill::getElement( double value ){
    QObject* obj = 0;
    double accumulative = 0;
    value = value * this->sum_frequencies;

    for ( int i = 0; i < this->ordered_frequencies.size(); i++ ){
        accumulative += this->ordered_frequencies.at( i );
        if( accumulative >= value ){
            obj = this->roulette.at( i );
            break;
        }
    }
    return obj;
}
