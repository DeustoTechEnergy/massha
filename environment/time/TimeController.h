#ifndef TIMECONTROLLER_H
#define TIMECONTROLLER_H

#include <QObject>
#include <QDebug>
#include <QDateTime>
#include <QJsonObject>

/**
 * @brief The TimeController class
 * Class to be used in environment to get time / hours.
 */
class TimeController : public QObject
{
    Q_OBJECT
    friend class Environment; // Only environment can call creator and setters
public:
    QJsonObject toJSON();
    void fromJSON(QJsonObject json);

    // GETTERS
    static TimeController* getInstance();
    static double msecondsToEnvironmentMseconds(double seconds);
    static double secondsToEnvironmentMseconds(double seconds);
    static QDateTime getCurrentDateTime();
    static QVariantMap getCurrentDateTimeAsJSON();
    static double getTimeSpeed();

private: // Only environment can create and init
    static void initTime(QDateTime environment_time, double time_speed = 1);
    static void setTimeSpeed(double time_speed = 1);

    TimeController();
    TimeController(TimeController const&);

    // Environment start date/time expecified when init
    static QDateTime environment_time;
    // Time speed
    // (How many seconds will there be in a real world second)
    static double time_speed;
    // The real date/time the class was launched
    // to make calculations about time spent since start
    static QDateTime internal_start_time;
};

#endif // TIMECONTROLLER_H
