#ifndef SPINBOXDOUBLE_HPP
#define SPINBOXDOUBLE_HPP

#include<QDoubleSpinBox>
#include<iostream>
using namespace std;

class SpinBoxDouble : public QObject
{
    Q_OBJECT

public:
    SpinBoxDouble(double* pValue, vector<double> bounds);

    QDoubleSpinBox *mpSpinBox;
    double* mpValue;

public slots:
    void readValue(double value);
    void writeValue();
};

#endif //SPINBOXDOUBLE_HPP
