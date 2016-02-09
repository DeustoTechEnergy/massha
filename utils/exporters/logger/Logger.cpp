#include <QDebug>
#include <QFile>
#include <QMutexLocker>
#include <QDir>

#include "Logger.h"

void Logger::log(QVariant info, QString filename){

    QString logs_folder_name = "logs";

    // Check if common_path folder exists
    if( !QDir( QDir::currentPath() + "/" + logs_folder_name ).exists() ){
        qDebug() << "[Logger::log] Creating logs folder in " << QDir::currentPath() + "/" + logs_folder_name;
        QDir().mkdir( QDir::currentPath() + "/" + logs_folder_name );
    }

   QFile logfile( QDir::currentPath() + "/" + logs_folder_name + "/" + filename );

   // Check if file exists, otherwise create it
   if( !logfile.exists() ){
       qDebug() << "[Logger::log] Creating log file " << logfile.fileName();
       logfile.open( QIODevice::WriteOnly );
       logfile.close();
   }

   if( logfile.open( QIODevice::Append ) ){
       QTextStream out(&logfile);
       out << info.toString() << "\n";
   }

   logfile.close();
}
