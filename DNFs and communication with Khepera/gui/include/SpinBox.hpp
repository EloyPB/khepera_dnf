#ifndef SPINBOX_HPP
#define SPINBOX_HPP

#include<QSpinBox>
#include<iostream>
using namespace std;

class SpinBox : public QObject
{
    Q_OBJECT

public:
    SpinBox(int* pValue, vector<int> bounds);

    QSpinBox *mpSpinBox;
    int* mpValue;

public slots:
    void readValue(int value);
    void writeValue();
};

#endif //SPINBOX_HPP
