/*
 * CommRobot.hpp
 *
 *  Created on: Dec 7, 2016
 *      Author: eloy
 */

#ifndef ROBOTCOMM_HPP_
#define ROBOTCOMM_HPP_

#include <iostream>
#include <string>
#include <cmath>
#include <chrono>

#include <cv.h>
#include <highgui.h>

#include <QObject>

#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

class RobotComm : public QObject
{
Q_OBJECT
public:
	RobotComm(const char* IP, int portNum);

	double mPos[3];
    cv::Mat mFrame;
	double mPhiTarg;
    int mSockFD;

public slots:
	int communicate();
    void whiteBalance();

signals:
    void listenAndAct();

protected:
	void getIRcalibrationData();
    int receiveData();
	void sendTarget();

	int mPortNum;
	struct sockaddr_in mServerAddr;
	struct hostent* mpServer;

public:
    static const int mPicWidth = 192;
    static const int mPicHeight = 144;
    static const int mPicSize = 3*mPicWidth*mPicHeight;
    static const int mPosSize = 50;
    static const int mBufferSize = mPicSize+mPosSize;
    char mReceiveBuffer[mBufferSize];
    std::vector<double> mWhiteFactors;
};


#endif /* ROBOTCOMM_HPP_ */
