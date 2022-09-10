#include "MatTransform_1D.hpp"

MatTransform_1D::MatTransform_1D(Mat input, Mat output, double scale) : MatTransform(input, output), mScale(scale) {}

void MatTransform_1D::transform()
{
    Mat resized;
    resize(mInputMat, resized, Size(), mScale, 1.0);

    float *ptrIn = resized.ptr<float>(0);
    float *ptrOut = mOutputMat.ptr<float>(0);
    int indexIn, indexOut;

    if(resized.size[1] < mOutputMat.size[1])
    {
        indexIn = (resized.size[1] - mOutputMat.size[1])/2;
        for(indexOut = 0; indexOut<mOutputMat.size[1]; indexOut++)
        {
            ptrOut[indexOut] = indexIn < 0 || indexIn > resized.size[1] ? 0 : ptrIn[indexIn];
            indexIn ++;
        }
    }
    else
    {
        indexIn = (mOutputMat.size[1] - resized.size[1])/2;
        for(indexOut = 0; indexOut<mOutputMat.size[1]; indexOut++)
        {
            ptrOut[indexOut] = ptrIn[indexIn];
            indexIn ++;
        }
    }
}
