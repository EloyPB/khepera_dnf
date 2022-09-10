/*
 * RobotListener_ToPolar.cpp
 *
 *  Created on: Dec 8, 2016
 *      Author: eloy
 */

#include "RobotListener.hpp"

RobotListener::RobotListener(RobotComm* pRobotComm) : mpRobotComm(pRobotComm)
{
    connect(pRobotComm, SIGNAL(listenAndAct()), this, SLOT(listenAndAct()), Qt::DirectConnection);
}



