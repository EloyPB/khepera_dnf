#include "PlotField1D.hpp"


PlotField1D::PlotField1D(NeuralFieldBase *pNeuralField, QWidget *pParent) :
    PlotField(pNeuralField, pParent)
{
    mpChart = new QChart;

    mpChartView = new QChartView(mpChart);
    mpPlot = mpChartView;

    mpSeries = new QLineSeries(this);
    mpChart->addSeries(mpSeries);

    QValueAxis *axisX = new QValueAxis(this);
    axisX->setRange(0, mpNeuralField->mSigmoidU.size[1]);
    axisX->setLabelFormat("%g");

    QValueAxis *axisY = new QValueAxis(this);
    axisY->setRange(0, 1);

    mpChart->setAxisX(axisX, mpSeries);
    mpChart->setAxisY(axisY, mpSeries);
    mpChart->legend()->hide();
}

PlotField1D::~PlotField1D()
{
}

void PlotField1D::update()
{
    QVector<QPointF> points;
    float* ptr = mpNeuralField->mSigmoidU.ptr<float>(0);
    mpNeuralField->mOutputMutex.lock();
    for(int i = 0; i< mpNeuralField->mSigmoidU.size[1]; i++)
        points.append(QPointF(i, ptr[i]));
    mpNeuralField->mOutputMutex.unlock();

    mpSeries->replace(points);
}
