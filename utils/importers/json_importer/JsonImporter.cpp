#include "JsonImporter.h"

#include <QFile>
#include <QDebug>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>

JsonImporter::JsonImporter(QString file_path) : QObject() {
    QFile file(file_path);
    if( !file.exists() ){
        qWarning() << "[JsonImporter:] File" << file_path << "not found" << endl;
        return;
    }
    this->file_path = file_path;
}

/**********************************************************************
 GETTERS
**********************************************************************/

QJsonObject JsonImporter::getJson(){
    QFile file(this->file_path);
    QJsonObject json;
    QJsonParseError jerror;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QJsonDocument json_document = QJsonDocument::fromJson(file.readAll(), &jerror);
        if(jerror.error != QJsonParseError::NoError){
            qDebug() << "[JsonImporter:] Error when parsing JSON file:" << jerror.errorString()  << endl;
        }
        else {
            json = json_document.object();
        }
        file.close();
    } else {
        qDebug() << "[JsonImporter::parse] Can't open JSON file:" << this->file_path << endl;
    }

    return json;
}

