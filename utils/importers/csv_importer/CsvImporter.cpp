#include "CsvImporter.h"
#include <QDebug>
#include <QDir>

#include <QFile>
#include <QTextStream>

CsvImporter::CsvImporter(QString file_path , char separator) : QObject(){
    QFile file(file_path);
    if( !file.exists() ){
        qWarning() << "[CsvImporter::CsvImporter] File" << file_path << "not found" << endl;
        return;
    }
    this->separator = separator;
    this->file_path = file_path;
}

/**********************************************************************
 GETTERS
**********************************************************************/

QMap<QString, int> CsvImporter::getColumnNames(){
    if( !this->column_names.isEmpty() ){
        return this->column_names;
    }

    QFile file(this->file_path);
    QTextStream in(&file);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QList<QString> header = in.readLine().split(this->separator);
        for ( int i=0 ; i < header.size() ; i++ ) {
            this->column_names.insert(header.at(i).toUpper(), i);
        }
        file.close();
    }
    return this->column_names;
}


QList< QMap<QString , QString> > CsvImporter::getRows(){
    QFile file(this->file_path);
    QTextStream in(&file);

    this->getColumnNames();
    QList< QMap<QString , QString> > results;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        in.readLine(); // Jump first line
        while (!in.atEnd()) {
            QMap<QString , QString> m;
            QList<QString> rows = in.readLine().split(this->separator);
            foreach( QString column_name , this->column_names.keys() ){
                m.insert( column_name , rows.at( this->column_names.value( column_name ) ) );
            }
            results.append( m );
        }
        file.close();
    }
    return results;
}
