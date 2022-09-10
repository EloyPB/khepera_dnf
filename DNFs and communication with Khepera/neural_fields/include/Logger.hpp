#ifndef INCLUDE_LOGGER_HPP_
#define INCLUDE_LOGGER_HPP_

#include "Manager.hpp"
#include <QObject>
#include <QThread>

//#include <iomanip>
//#include <ctime>

class Logger : public QObject
{
Q_OBJECT

public:
    Logger(Manager* manager, int cyclesPerLog = 1);
    ~Logger();

    vector<NeuralFieldBase*> mpFields;
    vector<FileStorage> mLogFiles;
    int mCycle, mCyclesPerLog;
    QThread mLoggerThread;

public slots:
    void log();
};


#endif /* INCLUDE_LOGGER_HPP_ */
