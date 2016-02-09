#ifndef GPXIMPORTER_H
#define GPXIMPORTER_H

#include <QObject>

#include <QXmlStreamReader>
#include <QDateTime>

struct GpxPoint {
    double x;
    double y;
    double z;
    QDateTime datetime;
};

class GpxImporter : public QObject
{
    Q_OBJECT
public:
    explicit GpxImporter(QString file_path);

    // GETTERS
    QList<GpxPoint> getGpxPoints();

private:

    QList<GpxPoint> processPoints();
    GpxPoint processPoint();

    QXmlStreamReader xml;
    QString file_path;
};

#endif // GPXIMPORTER_H
