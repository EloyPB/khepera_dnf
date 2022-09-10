#include "RobotListener_CVCam.hpp"
#include "RobotListener_SendTarget.hpp"
#include "RobotListener_PosLog.hpp"

int main(int argc, char *argv[]) {

	// establish the communication
	RobotComm robot("10.42.0.250", 5001);

	// declare and add listeners
    CVCam showCam(&robot);

    Mat phiTargMat;
    hconcat(Mat::zeros(1, 45, CV_32F), Mat::ones(1, 10, CV_32F), phiTargMat);
    hconcat(phiTargMat, Mat::zeros(1, 45, CV_32F), phiTargMat);
    SendTarget sendTarget(&robot, phiTargMat);

	// start
	robot.communicate();
}


