#include "SpinBox.hpp"

SpinBox::SpinBox(int *pValue, vector<int> bounds) : mpValue(pValue)
{
    mpSpinBox = new QSpinBox();
    mpSpinBox->setRange(bounds[0], bounds[1]);
    mpSpinBox->setValue(*pValue);

    connect(mpSpinBox, SIGNAL(valueChanged(int)), this, SLOT(readValue(int)));
}

void SpinBox::readValue(int value)
{
    *mpValue = value;
}

void SpinBox::writeValue()
{
    mpSpinBox->setValue(*mpValue);
}
