#include "GUI.hpp"
#include "PlotFieldMat.hpp"

PlotFieldMat::PlotFieldMat(NeuralFieldBase *pNeuralField, QWidget *pParent) :
    PlotField(pNeuralField, pParent)
{
    mpImageLabel = new QLabel(this);
    mpPlot = mpImageLabel;
    mpQImage = new QImage;
}

PlotFieldMat::~PlotFieldMat()
{
}

void PlotFieldMat::update()
{
    mpNeuralField->mOutputMutex.lock();
    *mpQImage = Mat2QImage(mpNeuralField->mSigmoidU);
    mpNeuralField->mOutputMutex.unlock();
    mpImageLabel->setPixmap(QPixmap::fromImage(*mpQImage));
}

QImage PlotFieldMat::Mat2QImage(const cv::Mat_<float> &src)
{
    const float scale = 255.0;
    unsigned int color;

    int rows = src.rows > 1 ? src.rows : mNulDimSize;
    int cols = src.cols > 1 ? src.cols : mNulDimSize;
    QImage dest(cols, rows, QImage::Format_ARGB32);

    if(src.rows > 1) // two dimensional source image
    {
        for (int y = 0; y < src.rows; ++y)
        {
            const float *srcRow = src[y];
            QRgb *destRow = (QRgb*)dest.scanLine(y);
            for (int x = 0; x < src.cols; ++x) {
                color = srcRow[x] * scale;
                destRow[x] = qRgba(color, color, color, 255);
            }
        }
    }
    else if(src.rows == 1 && src.cols == 1) // zero dimensional source image -> (mNulDimSize, mNulDimSize)
    {
        dest.fill(Qt::transparent);
        QPainter painter(&dest);
        color = src.at<float>(0,0) * scale;
        QColor qcolor(color, color, color);
        QBrush brush(qcolor, Qt::SolidPattern);
        painter.setBrush(brush);
        QPen qpen(Qt::blue);
        qpen.setWidth(2);
        painter.setPen(qpen);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.drawEllipse(1.0, 1.0, 18.0, 18.0);
        painter.end();
    }
    else if(src.rows == 1) // one dimensional image -> (mNulDimSize, src.cols)
    {
        const float *srcRow = src[0];
        for (int y = 0; y < mNulDimSize; ++y)
        {
            QRgb *destRow = (QRgb*)dest.scanLine(y);
            for (int x = 0; x < src.cols; ++x) {
                color = srcRow[x] * scale;
                destRow[x] = qRgba(color, color, color, 255);
            }
        }
    }
    return dest;
}


