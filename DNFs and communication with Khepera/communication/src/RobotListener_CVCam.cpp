/*
 * CamListener_ShowCam.cpp
 *
 *  Created on: Dec 7, 2016
 *      Author: eloy
 */

#include "RobotListener_CVCam.hpp"

CVCam::CVCam(RobotComm* pRobotComm) : RobotListener(pRobotComm) {}

void CVCam::listenAndAct()
{
    cvtColor(mpRobotComm->mFrame, mFrame, CV_RGB2BGR);
    namedWindow("Cam", CV_WINDOW_AUTOSIZE);
    imshow("Cam", mFrame);
    //waitKey(1); // need this if used without gui
    //cout << communication->mFrame.at<Vec3b>(72,96) << endl; // for checking values for white balance
}


