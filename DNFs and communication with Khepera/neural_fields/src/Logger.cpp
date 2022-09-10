#include "Logger.hpp"

Logger::Logger(Manager *manager, int cyclesPerLog)
{
    mCycle = 1;
    mCyclesPerLog = cyclesPerLog;
    mpFields = manager->mpFields;

    ostringstream date;
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    date << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

    for(auto pField : mpFields)
    {
        ostringstream fileName;
        fileName << "LogFiles/" << date.str() << " " << pField->mFieldName.toLocal8Bit().constData() <<".xml.gz";

        FileStorage file(fileName.str().c_str(), FileStorage::WRITE);

        if(!file.isOpened())
            cout << "ERROR: Please create a folder named 'LogFiles'" << endl;

        mLogFiles.push_back(file);
    }

    this->moveToThread(&mLoggerThread);
    connect(manager, SIGNAL(update()), this, SLOT(log()), Qt::QueuedConnection);
    mLoggerThread.start();
}

Logger::~Logger()
{
    for(auto logFile : mLogFiles)
        logFile.release();

    mLoggerThread.quit();
    mLoggerThread.wait();
}

// log time stamp

void Logger::log()
{
    if(mCycle == mCyclesPerLog)
    {
        mCycle = 1;
        for(unsigned int fieldNum=0; fieldNum < mpFields.size(); fieldNum++)
            mLogFiles[fieldNum] << "mat" << mpFields[fieldNum]->mSigmoidU;
    }
    else
        mCycle++;
}
