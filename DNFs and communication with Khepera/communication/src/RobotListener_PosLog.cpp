/*
 * RobotListener_ToPolar.cpp
 *
 *  Created on: Dec 8, 2016
 *      Author: eloy
 */

#include "RobotListener_PosLog.hpp"

PosLog::PosLog(RobotComm* pRobotComm) : RobotListener(pRobotComm)
{
    std::ostringstream date;
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    date << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

    std::ostringstream fileName;
    fileName << "LogFiles/" << date.str() << " Positions.txt";

    mFile = fopen(fileName.str().c_str(), "a");
    if(mFile == nullptr)
        cout << "ERROR: Please create a folder named 'LogFiles'" << endl;
}

PosLog::~PosLog()
{
	fclose(mFile);
}

void PosLog::listenAndAct()
{
    fprintf(mFile, "%f %f %f\n", mpRobotComm->mPos[0], mpRobotComm->mPos[1], mpRobotComm->mPos[2]);
}


