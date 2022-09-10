#include "RobotListener_QCam.hpp"

QCam::QCam(RobotComm* pRobotComm) : RobotListener(pRobotComm)
{
    mWidth = pRobotComm->mPicWidth;
    mHeight = pRobotComm->mPicHeight;
    mpRobotComm = pRobotComm;

    mpCamWidget = new QWidget();
    mpCamWidget->setWindowTitle("Camera");
    mpBox = new QHBoxLayout(mpCamWidget);
    mpBox->setMargin(0);
    mpCamLabel = new QLabel();
    mpBox->addWidget(mpCamLabel);
    mpCamImage = new QImage(mWidth, mHeight, QImage::Format_RGB888);
}

void QCam::listenAndAct()
{
    *mpCamImage = QImage((uchar*)mpRobotComm->mReceiveBuffer, mWidth, mHeight, QImage::Format_RGB888);
    mpCamLabel->setPixmap(QPixmap::fromImage(*mpCamImage));
}

void QCam::showCam()
{
    mpCamWidget->show();
}
