#ifndef INEMICRODATAIMPORTER_H
#define INEMICRODATAIMPORTER_H

#include <QObject>
#include <QList>

struct Record {
    // Person info
    QString province;
    QString municipality;
    QString born_month;
    QString born_year;
    QString age;
    QString gender;

    // Living
    QString living_country;
    QString living_province;
    QString living_municipality;

    // Work
    QString working_place;
    QString working_country;
    QString working_province;
    QString working_municipality;
    QString working_postcode;
    QString working_travels_amount;
    QString working_transport_type_1;
    QString working_transport_type_2;
    QString working_travel_time;

};

class IneMicroDataImporter : public QObject
{
    Q_OBJECT
public:
    explicit IneMicroDataImporter(QString file_path);

    // GETTERS
    QList<Record> getRows();

private:
    Record processLine(QString line);
    QString file_path;

};

#endif // INEMICRODATAIMPORTER_H
