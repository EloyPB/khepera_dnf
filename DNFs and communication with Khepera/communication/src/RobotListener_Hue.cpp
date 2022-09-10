/*
 * CamListener_ShowCam.cpp
 *
 *  Created on: Dec 7, 2016
 *      Author: eloy
 */

#include "RobotListener_Hue.hpp"

HueHistogram::HueHistogram(RobotComm *pRobotComm) : RobotListener(pRobotComm)
{
    mHueHistSize = Size(pRobotComm->mPicWidth, 180/mBinSize);
    mHueHist = Mat::zeros(mHueHistSize, CV_32FC1);

    mROI = Rect(0, pRobotComm->mPicHeight/2+5, pRobotComm->mPicWidth, pRobotComm->mPicHeight/2-5); // lower half of image;
}

void HueHistogram::listenAndAct()
{
    cvtColor(mpRobotComm->mFrame(mROI), mHSV, CV_RGB2HSV);

    char hue;
    Vec3b pixel;

    mHueHist = Mat::zeros(mHueHistSize, CV_32FC1);

    for(int i = 0; i < mHSV.rows; i++)
        for(int j = 0; j < mHSV.cols; j++)
        {
            pixel = mHSV.at<Vec3b>(i,j);
            if (pixel[1] > 100)
            {
                hue = pixel[0]/mBinSize;
                mHueHist.at<float>(hue, j) = 1;
            }
        }

    namedWindow("hist", WINDOW_AUTOSIZE);
    imshow("hist", mHueHist);
}


