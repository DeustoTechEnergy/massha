#ifndef SHPIMPORTER_H
#define SHPIMPORTER_H

#include <QObject>
#include "geos/geom/Geometry.h"
#include "shapefil.h"

using namespace geos::geom;

class ShpImporter : public QObject
{
    Q_OBJECT
public:

    explicit ShpImporter(QString path , QString filename);
    ~ShpImporter();

    // GETTERS
    int getShapeCount();
    Geometry* getShape(int index , QJsonObject &properties);

private:

    Geometry* parseShape(SHPObject *obj);

    SHPHandle shp_handle;
    DBFHandle dbf_handle;
    int shape_count;
    int shape_type;

};

#endif // SHPIMPORTER_H
