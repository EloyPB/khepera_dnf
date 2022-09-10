#ifndef PLOTFIELD1D_HPP
#define PLOTFIELD1D_HPP

#include "PlotField.hpp"

#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

using namespace QtCharts;

class PlotField1D : public PlotField
{
    Q_OBJECT

public:
    PlotField1D(NeuralFieldBase* pNeuralField, QWidget *parent = 0);
    ~PlotField1D();

    QChart *mpChart;
    QChartView *mpChartView;
    QLineSeries *mpSeries;

public slots:
    void update();

};

#endif //PLOTFIELD1D_HPP
