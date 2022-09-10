/*
 * RobotComm.cpp
 *
 *  Created on: Dec 7, 2016
 *      Author: eloy
 */
#include "RobotComm.hpp"

RobotComm::RobotComm(const char* IP, int portNum)
{
	mPortNum = portNum;

	mSockFD = socket(AF_INET, SOCK_STREAM, 0);
	if (mSockFD < 0)
		perror("ERROR opening socket");

	mpServer = gethostbyname(IP);
	if (mpServer == NULL)
		perror("ERROR no such host");

	bzero((char *) &mServerAddr, sizeof(mServerAddr));
	mServerAddr.sin_family = AF_INET;
	bcopy((char *)mpServer->h_addr, (char *)&mServerAddr.sin_addr.s_addr, mpServer->h_length);
	mServerAddr.sin_port = htons(mPortNum);

    if (::connect(mSockFD, (struct sockaddr*)&mServerAddr, sizeof(mServerAddr)) < 0)
		perror("ERROR connecting");

	mPhiTarg = 0;

    mFrame = cv::Mat(mPicHeight, mPicWidth, CV_8UC3);
    for(auto i = 0; i<3 ; i++)
        mWhiteFactors.push_back(1);
}

int RobotComm::communicate()
{
	char header[2] = "0";
    read(mSockFD, header, 2);

    if(header[0] == 'I')
    {
        write(mSockFD, "i", 2);
        getIRcalibrationData();
    }

    else if(header[0] == 'N') // normal mode of operation
    {
        write(mSockFD, "n", 2);
        while(true)
        {
            if(!receiveData())
                break;

            emit(listenAndAct());
        }
        std::cout << "Robot has disconnected." << std::endl;
    }
	return 0;
}


int RobotComm::receiveData()
{
    int n;
	int p = 0;
//    bzero(mReceiveBuffer, mBufferSize);

    // read buffer
    while (p < mBufferSize)
	{
        n = read(mSockFD, &mReceiveBuffer[p], mBufferSize);
	   	p += n;
	  	if (n < 0) {
            printf("ERROR reading from socket\n");
	   	    exit(1);
	   	}
	}

    // check if the buffer is filled with 'q' (robot has finished)
    if(mReceiveBuffer[0]=='q')
    {
        bool quit = true;
        for(int i=0; i<mBufferSize; i++)
            if(mReceiveBuffer[i] != 'q')
                quit = false;
        if(quit)
            return 0;
    }

    // transform to Mat and simple whitebalance
    mFrame = cv::Mat(mPicHeight, mPicWidth, CV_8UC3, mReceiveBuffer);

    for (int i = 0; i < mPicHeight; i++)
        for (int j = 0; j < mPicWidth; j++)
        {
            mFrame.at<cv::Vec3b>(i,j)[0] *= mWhiteFactors[0];
            mFrame.at<cv::Vec3b>(i,j)[1] *= mWhiteFactors[1];
            mFrame.at<cv::Vec3b>(i,j)[2] *= mWhiteFactors[2];
        }

    // extract positions
    std::string posStr = std::string(&mReceiveBuffer[mPicSize]);
    mPos[0] = stof(posStr.substr(0, posStr.find(",")-1));
    mPos[1] = stof(posStr.substr(posStr.find(",")+1, posStr.find(":")-1));
    mPos[2] = stof(posStr.substr(posStr.find(":")+1));

    return 1;
}

void RobotComm::whiteBalance()
{
    long red=0, green=0, blue=0;

    int firstRow, lastRow, firstCol, lastCol;
    firstRow = mPicHeight/2-40 > 0 ? mPicHeight/2-40 : 0;
    lastRow = mPicHeight/2+40 >= mPicHeight ? mPicHeight : mPicHeight/2+40;
    firstCol = mPicWidth/2-40 > 0 ? mPicWidth/2-40 : 0;
    lastCol = mPicWidth/2+40 >= mPicWidth ? mPicWidth : mPicWidth/2+40;

    for (int i = firstRow; i < lastRow; i++)
        for (int j = firstCol; j < lastCol; j++)
        {
            red += mFrame.at<cv::Vec3b>(i,j)[0];
            green += mFrame.at<cv::Vec3b>(i,j)[1];
            blue += mFrame.at<cv::Vec3b>(i,j)[2];
        }

    if(red >= green && red >= blue)
    {
        mWhiteFactors[0] = 1;
        mWhiteFactors[1] = (double)red/(double)green;
        mWhiteFactors[2] = (double)red/(double)blue;
    }
    else if(green >= red && green >= blue)
    {
        mWhiteFactors[0] = (double)green/(double)red;
        mWhiteFactors[1] = 1;
        mWhiteFactors[2] = (double)green/(double)blue;
    }
    else if(blue >= red && blue >= green)
    {
        mWhiteFactors[0] = (double)blue/(double)red;
        mWhiteFactors[1] = (double)blue/(double)green;
        mWhiteFactors[2] = 1;
    }
}

void RobotComm::sendTarget()
{
	char sendBuffer[10];
	snprintf(sendBuffer, 10, "%f", mPhiTarg);
	write(mSockFD, "t", 2);
	write(mSockFD, sendBuffer, 10);
}

void RobotComm::getIRcalibrationData()
{
    FILE *fp;

    std::ostringstream date;
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    date << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

    std::ostringstream fileName;
    fileName << "LogFiles/" << date.str() << " IR Sensors Data.txt";

    fp = fopen(fileName.str().c_str(), "a");
    if(fp == nullptr)
        std::cout << "ERROR: Please create a folder named 'LogFiles'" << std::endl;

	int n, size = 50;
	char buffer[size];

	for(n=0; n<8; n++)
	{
		bzero(buffer, size);
		read(mSockFD, buffer, size);
		printf("%s\n", buffer);
		fprintf(fp, "%s\n", buffer);
	}

    fclose(fp);
}


