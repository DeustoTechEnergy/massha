#ifndef TIFFIMPORTER_H
#define TIFFIMPORTER_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QMap>

#include "geos/geom/Geometry.h"

using namespace geos::geom;

class TiffImporter : public QObject
{
    Q_OBJECT
public:
    explicit TiffImporter(QString file_path);

private:
    QMap<geos::geom::Envelope* , double> heights; // IN METERS
};

#endif // TIFFIMPORTER_H
