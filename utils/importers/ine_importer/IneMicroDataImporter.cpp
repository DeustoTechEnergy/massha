#include "IneMicroDataImporter.h"

#include <QFile>
#include <QStringRef>
#include <QDebug>
#include "utils/exporters/logger/Logger.h"

IneMicroDataImporter::IneMicroDataImporter(QString file_path) : QObject(){
    QFile file(file_path);
    if( !file.exists() ){
        qWarning() << "[MicroDataImporter::MicroDataImporter] File" << file_path << "not found" << endl;
        return;
    }
    this->file_path = file_path;
}


/**********************************************************************
 METHODS
**********************************************************************/
QList<Record> IneMicroDataImporter::getRows(){
    QList<Record> records;

    QFile file(this->file_path);
    QTextStream in(&file);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        while (!in.atEnd()) {
            records.append( this->processLine( in.readLine() ) );
        }
        file.close();
    }
    return records;
}


Record IneMicroDataImporter::processLine(QString line){

    //ftp://www.ine.es/temas/censopv/cen11/Personas%20detallado_WEB.xls

    Record r;

    // Person info
    r.province =                    line.mid(0 , 2);
    r.municipality =                line.mid(2 , 3);
    r.born_month =                  line.mid(33 , 2);
    r.born_year =                   line.mid(35 , 4);
    r.age =                         line.mid(39 , 3);
    r.gender =                      line.mid(42 , 1);

    // Living
    r.living_country =              line.mid(78 , 3);
    r.living_province =             line.mid(81 , 2);
    r.living_municipality =         line.mid(83 , 3);

    // Work
    r.working_place =               line.mid(132 , 1);
    r.working_country =             line.mid(133 , 3);
    r.working_province =            line.mid(136 , 2);
    r.working_municipality =        line.mid(138 , 3);
    r.working_postcode =            line.mid(141 , 5);
    r.working_travels_amount =      line.mid(146 , 1);
    r.working_transport_type_1 =    line.mid(147 , 1);
    r.working_transport_type_2 =    line.mid(148 , 1);
    r.working_travel_time =         line.mid(149 , 1);

    return r;
}
