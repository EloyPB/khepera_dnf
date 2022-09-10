#ifndef PLOTFIELD_HPP
#define PLOTFIELD_HPP

#include "NeuralFieldBase.hpp"

#include <QWidget>

#include <iostream>
#include <chrono>

#include <QObject>
#include <QMutex>

class PlotField : public QWidget
{
    Q_OBJECT

public:
    QWidget *mpPlot;
    NeuralFieldBase *mpNeuralField;

    inline PlotField(NeuralFieldBase *pNeuralField, QWidget *parent = 0) : QWidget(parent), mpNeuralField(pNeuralField) {}

public slots:
    virtual void update() = 0;
};

#endif //PLOTFIELD_HPP
