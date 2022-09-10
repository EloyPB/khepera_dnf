/*
 * CamListener_show.hpp
 *
 *  Created on: Dec 7, 2016
 *      Author: eloy
 */

#ifndef ROBOTLISTENER_HUE_HPP_
#define ROBOTLISTENER_HUE_HPP_

#include "RobotListener.hpp"

class HueHistogram : public RobotListener {
public:
    HueHistogram(RobotComm* pRobotComm);

    Mat mHSV, mHueHist;
    Size mHueHistSize;
    Rect mROI;
    const int mBinSize = 3;

public slots:
    void listenAndAct();
};


#endif /* ROBOTLISTENER_HUE_HPP_ */
