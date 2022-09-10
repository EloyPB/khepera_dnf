#ifndef MATTRANSFORM_1D_HPP_
#define MATTRANSFORM_1D_HPP_

#include "MatTransform.hpp"

class MatTransform_1D: public MatTransform
{
public:
    MatTransform_1D(Mat input, Mat output, double scale);
    double mScale;

    void transform();

};

#endif /* MATTRANSFORM_HPP_1D_ */
