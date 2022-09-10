/*
 * CamListener_show.hpp
 *
 *  Created on: Dec 7, 2016
 *      Author: eloy
 */

#ifndef ROBOTLISTENER_QCAM_HPP_
#define ROBOTLISTENER_QCAM_HPP_

#include "RobotListener.hpp"
#include <QLabel>
#include <QWidget>
#include <QImage>
#include <QHBoxLayout>

class QCam : public RobotListener {
public:
    QCam(RobotComm* pRobotComm);

    QImage *mpCamImage;
    QLabel* mpCamLabel;
    RobotComm *mpRobotComm;
    QWidget *mpCamWidget;
    QHBoxLayout *mpBox;

    int mWidth;
    int mHeight;

public slots:
    void listenAndAct();
    void showCam();
};


#endif /* ROBOTLISTENER_QCAM_HPP_ */
