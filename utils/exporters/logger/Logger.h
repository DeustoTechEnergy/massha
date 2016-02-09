#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QString>

class Logger : public QObject
{
    Q_OBJECT
public:
    static void log(QVariant info, QString filename = "log");

private:
    static QString logs_folder_name;
};

#endif // LOGGER_H

