/*
 * CamListener_show.hpp
 *
 *  Created on: Dec 7, 2016
 *      Author: eloy
 */

#ifndef ROBOTLISTENER_SENDTARGET_HPP_
#define ROBOTLISTENER_SENDTARGET_HPP_

#include "RobotListener.hpp"
#include "NeuralField.hpp"

#define Pi 3.14159265359

class SendTarget : public RobotListener {
public:
    SendTarget(RobotComm* pRobotComm, Mat intentionMat);

    double mPhiTarg;
    int mIntentionMatSize;
    float *mIntentionMatPtr;

public slots:
    void listenAndAct();
};



#endif /* ROBOTLISTENER_SENDTARGET_HPP_ */
