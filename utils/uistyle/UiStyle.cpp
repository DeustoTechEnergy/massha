#include "UiStyle.h"
#include <QStringList>
#include "utils/exporters/agent_to_ui/AgentToUI.h"

UiStyle::UiStyle(QObject *parent) : QObject(parent){
    this->border_weight = 4;
    this->color = QColor("Gray");
    this->border_color = QColor("Black");
}

UiStyle::~UiStyle(){
}

QJsonObject UiStyle::toJSON(){
    QJsonObject style;
    style.insert("color" , AgentToUI::formatProperty( this->color ) );
    style.insert("fill_opacity" , AgentToUI::formatProperty( this->color.alpha() / 255 ) ); // To be between 0 and 1
    style.insert("border_color" , AgentToUI::formatProperty( this->border_color ) );
    style.insert("border_opacity" , AgentToUI::formatProperty( this->border_color.alpha() / 255 ) ); // To be between 0 and 1
    style.insert("weight" , AgentToUI::formatProperty( this->border_weight ) );
    if( !this->dash_array.isEmpty() ) {
        style.insert("dash_array" , AgentToUI::formatProperty( this->dash_array ) );
    }
    if( !this->icon_url.isEmpty() ) {
        style.insert("icon_url" , AgentToUI::formatProperty( this->icon_url ) );
    }

    return style;
}

void UiStyle::fromJSON( QJsonObject json ){
    if( json.contains("color") ) { this->color = QColor( json["color"].toObject()["value"].toString() ); }
    if( json.contains("fill_opacity") ) { this->color.setAlpha( json["fill_opacity"].toObject()["value"].toString().toDouble() * 255 ); } // To be between 0 and 255
    if( json.contains("border_color") ) { this->border_color = QColor( json["border_color"].toObject()["value"].toString() ); }
    if( json.contains("border_opacity") ) { this->border_color.setAlpha( json["border_opacity"].toObject()["value"].toString().toDouble() * 255 ); } // To be between 0 and 255
    if( json.contains("weight") ) { this->border_weight = json["weight"].toObject()["value"].toString().toInt(); }
    if( json.contains("icon_url") ) { this->icon_url = json["icon_url"].toObject()["value"].toString(); }
    if( json.contains("dash_array") ) { this->dash_array = json["dash_array"].toObject()["value"].toString(); }
}

/**********************************************************************
 METHODS
**********************************************************************/


QColor UiStyle::randomColor(){
    int rand = qrand() % QColor::colorNames().size();
    return QColor( QColor::colorNames().at( rand ) );
}
