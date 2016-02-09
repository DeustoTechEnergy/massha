#ifndef PARALLELISMCONTROLLER_H
#define PARALLELISMCONTROLLER_H

#include <QObject>
#include <QThreadPool>
#include <QMap>
#include <QMutex>

class ParallelismController : public QObject
{
    Q_OBJECT
public:

    static ParallelismController* getInstance();

    // GETTERS
    QThread* liberateThread( QThread* thread );
    QThread* getNewThread(int agent_id);

private:

    ParallelismController();
    ParallelismController(ParallelismController const&);
    ~ParallelismController();

    QThreadPool* thread_pool;
    QThread* main_thread;
    QList<QThread*>available_threads;

    int number_of_working_agents;
    QMutex mutex;

signals:
    void finished();

public:
    int getNumberOfWorkingAgents();
};

#endif // PARALLELISMCONTROLLER_H
