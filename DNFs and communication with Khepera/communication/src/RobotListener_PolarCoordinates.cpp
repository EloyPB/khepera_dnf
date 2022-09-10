/*
 * RobotListener_ToPolar.cpp
 *
 *  Created on: Dec 8, 2016
 *      Author: eloy
 */

#include "RobotListener_PolarCoordinates.hpp"

PolarCoordinates::PolarCoordinates(RobotComm *pRobotComm) : RobotListener(pRobotComm)
{
    mXr0 = mYr0 = mThetar0 = 0;
    mXt = mYt = 0;
    mIncrXr = mIncrYr = 0;
}

void PolarCoordinates::initDistanceMat(int size, int dMax)
{
    mDistanceMatSize = size;
    mDMax = dMax;
    mDistanceMat = Mat::zeros(1, mDistanceMatSize, CV_32FC1);
}

void PolarCoordinates::initAngleMat(int size)
{
    mAngleMatSize = size;
    mAngleMat = Mat::zeros(1, mAngleMatSize, CV_32FC1);
}

void PolarCoordinates::initTargetAngleMat(int size)
{
    mTargetAngleMatSize = size;
    mTargetAngleMat = Mat::zeros(1, mTargetAngleMatSize, CV_32FC1);
}

void PolarCoordinates::setTarget(double distance, double angle)
{
    if (angle >= -Pi && angle <= Pi) // calculate the position of target in the absolute reference frame
    {
        mXt = mXr0 + distance*cos(mThetar0+angle);
        mYt = mYr0 + distance*sin(mThetar0+angle);
    }
    else
    {
        cout << "Target angle has to be in [-pi, pi]" << endl;
        return;
    }
}

void PolarCoordinates::resetOrigin()
{
    mXr0 = mpRobotComm->mPos[0];
    mYr0 = mpRobotComm->mPos[1];
    mThetar0 = mpRobotComm->mPos[2];
}

void PolarCoordinates::listenAndAct()
{
    mXr = mpRobotComm->mPos[0];
    mYr = mpRobotComm->mPos[1];
    mThetar = mpRobotComm->mPos[2];

    mIncrXr = mXr - mXr0;
    mIncrYr = mYr - mYr0;

    // calculate position of robot in polar coordinates and put the result in Mats
    mD = sqrt(mIncrXr*mIncrXr + mIncrYr*mIncrYr);
    if(mIncrXr != 0)
        mAlpha = atan(mIncrYr/mIncrXr);
    else
        mAlpha = 0;

    mAngleMat = Mat::zeros(1, mAngleMatSize, CV_32FC1);
    int pos = mAngleMatSize/2*(1+mAlpha/Pi);
    mAngleMat.at<float>(0, pos) = 1.0;

    mDistanceMat = Mat::zeros(1, mDistanceMatSize, CV_32FC1);
    pos = mD/mDMax*mDistanceMatSize;
    pos = pos > mDistanceMatSize ? mDistanceMatSize : pos;
    mDistanceMat.at<float>(0, pos) = 1.0;


    // calculate relative angle to target (mPhiTarget)
    double target  = atan2((mYt-mYr), (mXt-mXr));
    target = target < 0 ? target + 2*Pi : target;
    mPhiTarget = mThetar - target;
    if(mPhiTarget > Pi)
        mPhiTarget -= 2*Pi;
    else if(mPhiTarget < -Pi)
        mPhiTarget += 2*Pi;
//    cout << "theta: " << mThetar << " target: " << target << " phitarg: " << mPhiTarget << endl;

    mTargetAngleMat = Mat::zeros(1, mTargetAngleMatSize, CV_32FC1);
    pos = mTargetAngleMatSize/2*(1+mPhiTarget/Pi);
    mTargetAngleMat.at<float>(0, pos) = 1.0;
}


