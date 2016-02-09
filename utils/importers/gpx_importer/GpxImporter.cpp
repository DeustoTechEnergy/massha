#include "GpxImporter.h"

#include <QFile>
#include "environment/Environment.h"

GpxImporter::GpxImporter(QString file_path) : QObject(){
    QFile file(file_path);
    if( !file.exists() ){
        qWarning() << "[GpxImporter::GpxImporter] File" << file_path << "not found" << endl;
        return;
    }

    this->file_path = file_path;
}

/**********************************************************************
 GETTERS
**********************************************************************/

QList<GpxPoint> GpxImporter::getGpxPoints(){
    QList<GpxPoint> gpx_points;
    QFile file( this->file_path );
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        this->xml.setDevice(&file);

        while( !this->xml.atEnd() ){
            this->xml.readNextStartElement();
            if ( this->xml.name() == "trkseg" ){
                gpx_points.append( this->processPoints() );
            }
        }
        if ( this->xml.hasError() ) {
            qWarning() << "[GpxImporter::GpxImporter] Error parsing GPX XML" << this->xml.errorString();
        }
        file.close();
    }

    return gpx_points;
}

/**********************************************************************
 METHODS
**********************************************************************/

QList<GpxPoint> GpxImporter::processPoints(){
    QList<GpxPoint> ps;

    if ( !this->xml.isStartElement() || this->xml.name() != "trkseg" ){
        return ps;
    }

    while ( !this->xml.atEnd() ) {
        if ( this->xml.name() == "trkpt"){
            ps.append( this->processPoint() );
        } else {
            this->xml.readNextStartElement();
        }
    }

    return ps;
}


GpxPoint GpxImporter::processPoint(){
    GpxPoint point;
    if ( !this->xml.isStartElement() || this->xml.name() != "trkpt"){
        return point;
    }

    point.x = this->xml.attributes().value("lon").toDouble();
    point.y = this->xml.attributes().value("lat").toDouble();
    point.z = 0;

    while ( this->xml.readNextStartElement() && this->xml.name() != "trkpt") {
        if ( this->xml.name() == "ele"){
            point.z = this->xml.readElementText().toDouble();
        }
        if ( this->xml.name() == "time") {
            point.datetime = QDateTime::fromString( this->xml.readElementText() );
        }
        this->xml.skipCurrentElement();
    }

    return point;
}
