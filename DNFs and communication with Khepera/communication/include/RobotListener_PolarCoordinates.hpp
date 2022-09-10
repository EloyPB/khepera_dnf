/*
 * RobotListener_ToPolar.hpp
 *
 *  Created on: Dec 8, 2016
 *      Author: eloy
 */

#ifndef INCLUDE_ROBOTLISTENER_POLARCOORDINATES_HPP_
#define INCLUDE_ROBOTLISTENER_POLARCOORDINATES_HPP_

#define Pi 3.14159265359

#include "RobotListener.hpp"


class PolarCoordinates : public RobotListener {
public:
    PolarCoordinates(RobotComm* pRobotComm);

    void initDistanceMat(int size, int dMax);
    void initAngleMat(int size);
    void initTargetAngleMat(int size);

    double mXr, mYr, mThetar; // position of robot
    double mXr0, mYr0, mThetar0; // position of origin of polar coordinates
    double mD, mAlpha; // position of robot in polar coordinates
    double mXt, mYt; // position of target
    double mIncrXr, mIncrYr; // increments in the position of robot
    double mPhiTarget; // egocentric angle to target

    int mDMax; // max distance for the field
    Mat mDistanceMat, mAngleMat, mTargetAngleMat;
    int mDistanceMatSize, mAngleMatSize, mTargetAngleMatSize;

public slots:
    void listenAndAct();
    void resetOrigin();
    void setTarget(double distance, double angle);
};


#endif /* INCLUDE_ROBOTLISTENER_POLARCOORDINATES_HPP_ */
