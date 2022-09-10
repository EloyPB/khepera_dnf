#ifndef MATTRANSFORM_HPP_
#define MATTRANSFORM_HPP_

#include <cv.h>
using namespace cv;

class MatTransform
{
protected:
    Mat mInputMat;
    Mat mOutputMat;

public:
    inline MatTransform(Mat input, Mat output) : mInputMat(input), mOutputMat(output) {}

    virtual void transform() = 0;


};

#endif /* MATTRANSFORM_HPP_ */

