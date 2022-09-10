#include "NeuralFieldBase.hpp"


void NeuralFieldBase::readParam(ifstream &myfile, void* pvariable)
{
    string line;
    myfile.ignore(100, '=');
    myfile.ignore(1);
    getline(myfile, line);
    if (line.find('.') == string::npos)
        *(int*)pvariable = atoi(line.c_str());
    else
        *(double*)pvariable = atof(line.c_str());
}

void NeuralFieldBase::writeIntParam(fstream &myFile, const char *pParamName, int const &variable)
{
    myFile << pParamName << " = " << variable << endl;
}

void NeuralFieldBase::writeDoubleParam(fstream &myFile, const char *pParamName, double const &variable)
{
    int precision;
    if(variable == 0)
        precision = 4;
    else
        precision = floor(log10(abs(variable))) + 4;
    myFile << setprecision(precision) << showpoint << pParamName << " = " << variable << endl;
}

void NeuralFieldBase::createGkernel(Mat kernel, int kernelSize, double kernelAmp, double kernelSigma)
{
	int centerX = kernelSize/2;

    float* ptr = kernel.ptr<float>(0);
	for (int i=0; i<kernelSize; i++)
        ptr[i] = kernelAmp * exp(-powf(i-centerX, 2) / (2*powf(kernelSigma, 2)));
}


void NeuralFieldBase::cvShow(void)
{
    string windowName = to_string(mFieldNum);
    namedWindow(windowName, WINDOW_AUTOSIZE);

    if(mSigmoidU.rows > 1)
        imshow(windowName, mSigmoidU);
    else if(mSigmoidU.rows == 1 && mSigmoidU.cols == 1)
    {
        Mat fieldDisplay = Mat(20, 20, CV_32FC1);
        float* p = fieldDisplay.ptr<float>(0);
        for (int i = 0; i<400; i++)
            p[i] = mSigmoidU.at<float>(0,0);
        imshow(windowName, fieldDisplay);
    }
    else if(mSigmoidU.rows == 1)
    {
        Mat fieldDisplay = Mat(20, mSigmoidU.cols, CV_32FC1);
        for (int i=0; i<20; i++)
            mSigmoidU.row(0).copyTo(fieldDisplay.row(i));
        imshow(windowName, fieldDisplay);
    }

    waitKey(1);
}
