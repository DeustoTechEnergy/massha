#include "ParallelismController.h"

#include <QDebug>

ParallelismController* ParallelismController::getInstance(){
    static ParallelismController instance;
    return &instance;
}

ParallelismController::ParallelismController(){
    this->number_of_working_agents = 0;
    this->thread_pool = QThreadPool::globalInstance();
    this->main_thread = this->thread();
    // EDIT : /etc/security/limits.conf
    int thread_limit = qMax( this->thread_pool->maxThreadCount() , QThread::idealThreadCount() );
    for(int i = 0; i < thread_limit; i++){
        QThread* thread = new QThread();
        this->available_threads.append( thread );
        thread->start();
    }
    qDebug() << "[ParallelismController::ParallelismController] ParallelismController created with" << this->available_threads.size() << "threads." << endl;
}

ParallelismController::~ParallelismController(){
    qDeleteAll( this->available_threads );
}

/**********************************************************************
 GETTERS
**********************************************************************/

QThread* ParallelismController::getNewThread(int agent_id){
    // TODO have a look if not well balanced
    this->mutex.lock();
    number_of_working_agents += 1;
    this->mutex.unlock();

    return this->available_threads.at( agent_id % this->available_threads.size() );
}

QThread* ParallelismController::liberateThread(QThread *thread){
    Q_UNUSED(thread)
    if (this->number_of_working_agents > 0){
        this->mutex.lock();
        this->number_of_working_agents -= 1;
        this->mutex.unlock();
    }

    //qDebug() << "Number of working agents:" << this->number_of_working_agents << endl;
    if (this->number_of_working_agents == 0){
        emit finished();
    }

    return this->main_thread;
}

int ParallelismController::getNumberOfWorkingAgents(){
    return this->number_of_working_agents;
}
