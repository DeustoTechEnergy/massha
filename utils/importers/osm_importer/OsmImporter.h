#ifndef OSMIMPORTER_H
#define OSMIMPORTER_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QXmlStreamReader>

#include "geos/geom/Geometry.h"

using namespace geos::geom;

struct OsmElement {
    QString id;
    QString type;
    QMap<QString, QString> tags;
    Geometry* geometry;
};

class OsmImporter : public QObject
{
    Q_OBJECT
public:
    explicit OsmImporter(QString file_path);

    // GETTERS
    QList<OsmElement> getOsmElements( bool nodes = true , bool ways = true , bool relations = true );

private:
    QString file_path;

    // METHODS
    OsmElement processNode( QXmlStreamReader* xml );
    OsmElement processWay( QXmlStreamReader* xml , QMap<QString , OsmElement> nodes );
    OsmElement processRelation( QXmlStreamReader* xml , QMap<QString , OsmElement> nodes , QMap<QString , OsmElement> ways );
};

#endif // OSMIMPORTER_H
