#ifndef JSONIMPORTER_H
#define JSONIMPORTER_H

#include <QObject>
#include <QJsonObject>

class JsonImporter : public QObject
{
    Q_OBJECT

public:
    explicit JsonImporter(QString file_path);

    // GETTERS
    QJsonObject getJson();

private:
    QString file_path;
};

#endif // JSONIMPORTER_H
