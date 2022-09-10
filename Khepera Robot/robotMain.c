#include "robotLib.h"

int main(int argc, char *argv[]) {

	// INITIALIZATION
	printf("Welcome! Press control-c to leave\n");
	printf("The robot starts in navigation mode by default, run './robot i' to send IR calibration data instead\n");
	signal(SIGINT , ctrlc_handler); // set signal for catching ctrl-c
	initRobot();

	createServer();

	char ack[2];

	char mode;
	if(argc > 1)
		mode = argv[1][0];
	else
		mode = 'n';

	switch(mode)
	{
		case 'n':
			write(socketFD, "N", 2); // "Normal" mode of operation
			read(socketFD, ack, 2);

			initCamera();
			initMotors();

			while(!quitReq)
			{
//				struct timeval t0, t1, t2;
//				gettimeofday(&t0, NULL);
				kb_frameRead(sendBuffer); // takes picture
//				gettimeofday(&t1, NULL);

				navigation();
				odometry(sendBuffer); // calculates odometry step and appends position data to buffer

				write(socketFD, sendBuffer, picSize+posSize);

				getPhiTarget();

//				gettimeofday(&t2, NULL);
//				printf("cycle took %ld\n", ((t2.tv_sec * 1000000 + t2.tv_usec)
//				- (t0.tv_sec * 1000000 + t0.tv_usec)));
//				printf("frameRead took %ld\n", ((t1.tv_sec * 1000000 + t1.tv_usec)
//				- (t0.tv_sec * 1000000 + t0.tv_usec)));
			}
			stopCamera();
			int i;
			for(i=0; i<picSize+50; i++)
				sendBuffer[i]='q';
			write(socketFD, sendBuffer, picSize+50);
			break;

		case 'i':
			write(socketFD, "I", 2); // "IR" sensors data
			read(socketFD, ack, 2);

			getIRCalibrationData();
			break;
	}
	stopRobot();
	return 0;
}

//SIMPLE OBSTACLE AVOIDANCE
/*#include "robotLib.h"

int main(int argc, char *argv[]) {

	// INITIALIZATION
	printf("Welcome! Press control-c to leave\n");
	signal(SIGINT , ctrlc_handler); // set signal for catching ctrl-c
	initRobot();
	initMotors();

	while(!quitReq)
	{
		navigation();
	}

	stopRobot();
	return 0;
}*/
