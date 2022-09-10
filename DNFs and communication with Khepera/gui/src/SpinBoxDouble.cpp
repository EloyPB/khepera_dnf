#include "SpinBoxDouble.hpp"

SpinBoxDouble::SpinBoxDouble(double *pValue, vector<double> bounds) : mpValue(pValue)
{
    mpSpinBox = new QDoubleSpinBox();
    mpSpinBox->setRange(bounds[0], bounds[1]);
    mpSpinBox->setDecimals(3);
    mpSpinBox->setValue(*pValue);
    mpSpinBox->setSingleStep(0.01);

    connect(mpSpinBox, SIGNAL(valueChanged(double)), this, SLOT(readValue(double)));
}

void SpinBoxDouble::readValue(double value)
{
    *mpValue = value;
}

void SpinBoxDouble::writeValue()
{
    mpSpinBox->setValue(*mpValue);
}
