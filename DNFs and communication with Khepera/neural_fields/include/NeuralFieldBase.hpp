// c++ 11 code

#ifndef NEURALFIELD_HPP_
#define NEURALFIELD_HPP_

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

#include <cv.h>
#include <highgui.h>

#include <chrono>
#include <unistd.h>

#include <QMutex>
#include <QString>
#include <QObject>

using namespace std;
using namespace cv;
using namespace chrono;

class NeuralFieldBase : public QObject
{
    Q_OBJECT
public:
//	virtual ~NeuralFieldBase(){}
    virtual int initFromFile() = 0;
	virtual void step(void) = 0;
    void cvShow(void);

public slots:
    virtual int readParameters() = 0;
    virtual int writeParameters() = 0;
    virtual void update() = 0;

signals:
    void updateValues();

public:
	int mFieldNum;
    QString mFieldName;
    QMutex mOutputMutex;

    Mat mSigmoidU; // output of field
    int mDimensions;

	double mSigmoidBeta;
	double mTimeConstant;
	double mRestingLevel;

    const char* mpConfigFile;

protected:
    void readParam(ifstream &myFile, void* pVariable);
    void writeIntParam(fstream &myFile, const char *pParamName, int const &variable);
    void writeDoubleParam(fstream &myFile, const char *pParamName, double const &variable);
	void createGkernel(Mat kernel, int kernelSize, double kernelAmp, double kernelSigma);
};




#endif
