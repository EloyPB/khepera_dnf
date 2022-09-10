#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>

#include <iostream>

#include "SpinBox.hpp"
#include "SpinBoxDouble.hpp"
#include "NeuralFieldBase.hpp"
#include "NeuralField.hpp"
#include "NeuralNode.hpp"

using namespace std;

class Parameters : public QWidget
{
    Q_OBJECT
public:
    void createWindow(NeuralFieldBase *pNeuralField);

public slots:
    void showWindow();


protected:
    vector<QLabel*> mpIntNames;
    vector<int*> mpIntegers;
    vector<QLabel*> mpDoubleNames;
    vector<double*> mpDoubles;
    vector<bool> mFieldHasToKnow;
    vector<vector<int>> mIntBounds;
    vector<vector<double>> mDoubleBounds;

    QVBoxLayout *mpVBox;
    QHBoxLayout *mpHBox;
    QPushButton *mpRestoreButton, *mpSaveButton;

    void addInteger(QString name, int *value, vector<int> bounds, bool fieldHasToKnow);
    void addDouble(QString name, double *value, vector<double> bounds, bool fieldHasToKnow);

};

#endif //PARAMETERS_HPP
