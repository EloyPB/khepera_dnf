#ifndef PLOTFIELDMAT_HPP
#define PLOTFIELDMAT_HPP

#include "PlotField.hpp"
#include <QLabel>
#include <QColor>
#include <cv.h>

class PlotFieldMat : public PlotField
{
    Q_OBJECT

public:
    PlotFieldMat(NeuralFieldBase* pNeuralField, QWidget *pParent = 0);
    ~PlotFieldMat();

public slots:
    void update();

private:
    QImage Mat2QImage(const cv::Mat_<float> &src);

    QImage* mpQImage;
    const int mNulDimSize = 20;

public:
    QLabel* mpImageLabel;


};

#endif //PLOTFIELDMAT_HPP
