#ifndef CSVIMPORTER_H
#define CSVIMPORTER_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QVariant>

class CsvImporter : public QObject
{
    Q_OBJECT
public:
    explicit CsvImporter(QString file_path , char separator);

    // GETTERS
    QMap<QString,int> getColumnNames();
    QList< QMap<QString , QString> > getRows();

private:
    QString file_path;
    char separator;
    QMap<QString, int> column_names;

};

#endif // CSVIMPORTER_H
