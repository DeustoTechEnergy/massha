#include "TiffImporter.h"

#include <QDebug>

TiffImporter::TiffImporter(QString file_path) : QObject(){
    QFile file(file_path);
    if( !file.exists() ){
        qWarning() << "[TiffImporter::TiffImporter] File" << file_path << "not found" << endl;
        return;
    }
}

