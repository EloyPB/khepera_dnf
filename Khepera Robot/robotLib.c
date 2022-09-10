#include "robotLib.h"

void stopRobot()
{
	kh4_set_speed(0, 0, dsPic); // stop robot
	kh4_SetMode(kh4RegIdle, dsPic);

	kh4_SetRGBLeds(0,0,0,0,0,0,0,0,0,dsPic); // clear RGB leds

	kb_change_term_mode(0); // revert to original terminal if called
}

void ctrlc_handler(int sig)
{
	static int count;
	quitReq = 1;

	if (count < 1)
		printf("\npress once more if I'm still around\n");
	else
	{
		stopRobot();
		exit(0);
	}

	count ++;
}

void initRobot()
{
	quitReq = 0;
	// Set the libkhepera debug level (I left this here from the template but not using it)
	//kb_set_debug_level(2);

	// Init the khepera library
	if (kh4_init(0, NULL) != 0)
		printf("\nERROR: could not initiate the libkhepera!\n\n");

	// open robot socket and store the handle in their respective pointers
	dsPic  = knet_open("Khepera4:dsPic", KNET_BUS_I2C, 0, NULL);
	if (dsPic == NULL)
		printf("\nERROR: could not initiate communication with Kh4 dsPic\n\n");

	// deactivate ultrasonic sensors
	kh4_activate_us(0b00000, dsPic);

	kh4_SetMode(kh4RegIdle, dsPic);

	phiTarg = 0;
	posSize = 50;
}

void createServer(void)
{
	// CREATE SERVER, IP 10.42.0.250, port 5001
	int sockFD0, portno, clilen;
	struct sockaddr_in serv_addr, cli_addr;

	sockFD0 = socket(AF_INET, SOCK_STREAM, 0);
	if (setsockopt(sockFD0, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
	    perror("setsockopt(SO_REUSEADDR) failed");

	if (sockFD0 < 0) {
		perror("ERROR opening socket");
	    exit(1);}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = 5001;

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(sockFD0, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("ERROR on binding");
	    exit(1);}

	listen(sockFD0, 5);
	clilen = sizeof(cli_addr);

	socketFD = accept(sockFD0, (struct sockaddr *)&cli_addr, (unsigned int*)&clilen);

	if (socketFD < 0) {
		perror("ERROR on accept");
		exit(1);}
}

void initMotors()
{
	int maxsp, accinc, accdiv, minspacc, minspdec; // for speed profile
	int kp, ki, kd;
	int pmarg;

	// initialize the motors controllers
	pmarg = 20;
	kh4_SetPositionMargin(pmarg, dsPic);

	kp = 10;
	ki = 5;
	kd = 1;
	kh4_ConfigurePID(kp, ki, kd, dsPic);

	accinc = 3;
	accdiv = 0;
	minspacc = 20;
	minspdec = 1;
	maxsp = 400;
	kh4_SetSpeedProfile(accinc, accdiv, minspacc, minspdec, maxsp, dsPic);

	kh4_SetMode(kh4RegSpeedProfile, dsPic);
	kh4_ResetEncoders(dsPic);
}

void initCamera()
{
	unsigned int width = 192;	// image width (max 752)
	unsigned int height = 144; // image height  (max 480)
	picSize = width*height*3*sizeof(char);

	int r;
	r = kb_camera_init(&width, &height);
	if (r < 0)
		printf("error %d initializing camera", r);

	switch(r) {
		case 1:
			printf("width adjusted to %d\r\n",width);
			break;
		case 2:
			printf("height adjusted to %d\r\n",height);
			break;
		case 3:
			printf("width adjusted to %d and height adjusted to %d !\r\n",width,height);
			break;
		default:
			break;
	}

	int buffSize = picSize + posSize;
	sendBuffer = malloc(buffSize);

	if (sendBuffer == NULL)
	{
		fprintf(stderr, "could not alloc image buffer!\r\n");
		free(sendBuffer);
	}

	if(kb_captureStart()<0)
	{
		printf("error starting capture");
		kb_camera_release();
	}

	usleep(100000);
}

void stopCamera()
{
	kb_captureStop();
	kb_camera_release();
	free(sendBuffer);
}

// Small function that tells you to place the robot at different positions and sends the recorded IR measurements
void getIRCalibrationData()
{
	int distance[12];
	char buffer[24];
	int measurements[10];
	char tmp[10], send[50];
	int i, j;
	char nothing[10];

	for(j=0; j<11; j++)
		distance[j] = 2*(j);
	distance[11] = 30;

	for(i=0; i<8; i++)
	{
		memset(send, 0, sizeof(send));
		printf("Sensor number %u (0 is back-left, incrementing clockwise)\n", i);
		for(j=0; j<12; j++)
		{
			printf("Place sensor at %d cm and press key to continue\n", distance[j]);
			scanf("%s", nothing);
			kh4_proximity_ir(buffer, dsPic);
			measurements[j] = 1023 - (buffer[i*2] | buffer[i*2+1]<<8);
			printf("measurement[j]: %i\n", measurements[j]);
			snprintf(tmp, 10, "%i ", measurements[j]);
			strcat(send, tmp);
		}
		write(socketFD, send, 12*sizeof(int));
	}
}

void navigation()
{
	static char bufferIR[20];
	static short distances[8];
	static int sensorNum;
	static double landa;
	static double landaTarg = 10.0;

	static const int angles[8] = {135, 90, 45, 2, -45, -90, -135, -180}; // angles of the IR sensors
//	static const int b1 = 1;
	static const int b1 = 3;
	static const int b2 = 500;
	// factors for compensating differences among IR sensors
	static const float calibration[8] = {1.022346, 1.039773, 1.040956, 1.103739, 1.191406, 1.118582, 1.067678, 1.000000};

	kh4_proximity_ir(bufferIR, dsPic);

	dPhidt = 0;
	// force-let contributions from obstacles
	for (sensorNum = 0; sensorNum < 8; sensorNum++)
	{
		distances[sensorNum] = (1023 - (bufferIR[sensorNum*2] | bufferIR[sensorNum*2+1]<<8))*calibration[sensorNum];
		landa = b1*exp(-(double)distances[sensorNum]/b2);
		dPhidt += landa*angles[sensorNum]*exp(-pow(angles[sensorNum], 2)/3000);
		speed += ((distances[sensorNum]-120)/10 - speed)*exp(-pow(angles[sensorNum], 2)/800)*exp(-distances[sensorNum]/1000);
		speed = (speed > 100 ? 100 : speed); // saturate speed
	}
	// force-let contribution from target
	dPhidt -= landaTarg * sin(phiTarg);

	kh4_set_speed((int)speed+5*dPhidt, (int)speed-5*dPhidt, dsPic);
}

void odometry(unsigned char* buffer)
{
	static int posL, posR, posLp, posRp;
	static int dL, dR;
	static double r;
	static double dXe, dYe, dPhi, x, y, phi;

	kh4_get_position(&posL, &posR, dsPic); // get encoder pulses
	dL = posL - posLp;
	dR = posR - posRp;
	posLp = posL;
	posRp = posR;
	r = dR==dL ? DBL_MAX : dbw/(double)2.0*(dR+dL)/(double)(dR-dL);
	dPhi = (double)(dR-dL)/dbw;
	dXe = r*sin(dPhi); // incr x_egocentric
	dYe = r*(1-cos(dPhi));
	x += dXe*cos(phi) - dYe*sin(phi);
	y += dXe*sin(phi) + dYe*cos(phi);
	phi += dPhi;
	if(phi > 2*pi)
		phi -= 2*pi;
	else if(phi < 0)
		phi += 2*pi;

	// send (x, y, phi), x and y in mm, phi in radians
	snprintf(&buffer[picSize], posSize, "%f,%f:%f", x*0.006782, y*0.006782, phi);
}

void getPhiTarget()
{
	char phiTargBuffer[20];
	read(socketFD, phiTargBuffer, 10);
	sscanf(phiTargBuffer, "%lf", &phiTarg);
}
