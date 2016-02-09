#include "Place.h"

Place::Place(QString class_name) : Agent(class_name){
    this->getStyle()->color = QColor("Green");
    this->getStyle()->color.setAlphaF( 0.5 );
    this->getStyle()->icon_url = "http://cdn.flaticon.com/png/128/33956.png";
}

/**********************************************************************
 GETTERS
**********************************************************************/

double Place::getArea(){
    return this->area;
}

/**********************************************************************
 SETTERS
**********************************************************************/

void Place::setArea(double area){
    this->area = area;
}

