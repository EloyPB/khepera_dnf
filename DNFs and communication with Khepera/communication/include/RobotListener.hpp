/*
 * CamListener.hpp
 *
 *  Created on: Dec 7, 2016
 *      Author: eloy
 */

#ifndef ROBOTLISTENER_HPP_
#define ROBOTLISTENER_HPP_

#include "RobotComm.hpp"

using namespace std;
using namespace cv;

class RobotListener: public QObject
{
Q_OBJECT
public:
    RobotListener(RobotComm* pRobotComm);

public slots:
    virtual void listenAndAct() = 0;

protected:
    RobotComm* mpRobotComm;
};

#endif /* ROBOTLISTENER_HPP_ */
