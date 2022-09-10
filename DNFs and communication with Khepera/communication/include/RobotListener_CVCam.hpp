/*
 * CamListener_show.hpp
 *
 *  Created on: Dec 7, 2016
 *      Author: eloy
 */

#ifndef ROBOTLISTENER_CVCAM_HPP_
#define ROBOTLISTENER_CVCAM_HPP_

#include "RobotListener.hpp"

class CVCam : public RobotListener {
public:
    CVCam(RobotComm* pRobotComm);
    Mat mFrame;

public slots:
    void listenAndAct();
};


#endif /* ROBOTLISTENER_CVCAM_HPP_ */
