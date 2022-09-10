/*
 * robotLib.h
 *
 *  Created on: Dec 16, 2016
 *      Author: eloy
 */

#ifndef ROBOTLIB_H_
#define ROBOTLIB_H_

#include <khepera/khepera.h>
#include <signal.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <sys/time.h>

#define dbw 15541 // distance between wheels in encoder pulses
#define pi 3.14159265359

knet_dev_t * dsPic; // robot pic microcontroller access

int quitReq; // quit variable for loops

int socketFD;
unsigned char* sendBuffer; // buffer that gets sent

int picSize;
int posSize;

double speed;
double dPhidt;
double phiTarg;

void stopRobot();
void ctrlc_handler(int sig);
void initRobot();
void createServer(void);
void initMotors();
void initCamera();
void stopCamera();
void getIRCalibrationData();
void navigation();
void odometry(unsigned char* buffer);
void getPhiTarget();

#endif /* ROBOTLIB_H_ */
