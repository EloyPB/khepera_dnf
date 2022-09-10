/*
 * CamListener_ShowCam.cpp
 *
 *  Created on: Dec 7, 2016
 *      Author: eloy
 */

#include "RobotListener_SendTarget.hpp"


SendTarget::SendTarget(RobotComm* pRobotComm, Mat intentionMat) : RobotListener(pRobotComm)
{
    mIntentionMatSize = intentionMat.size[1];
    mIntentionMatPtr = intentionMat.ptr<float>(0);
}

void SendTarget::listenAndAct()
{
    int sum = 0, count = 0;
    for(int i=0; i<mIntentionMatSize; i++)
    {
        if(mIntentionMatPtr[i]>0.7)
        {
            sum += i;
            count ++;
        }
    }
    int center = count > 0 ? sum/count : mIntentionMatSize/2;
    mPhiTarg = 3.0/8.0*Pi - center/(double)mIntentionMatSize*3.0/4.0*Pi;

    char sendBuffer[10];
    std::snprintf(sendBuffer, 10, "%lf", mPhiTarg);
    write(mpRobotComm->mSockFD, sendBuffer, 10);
}


