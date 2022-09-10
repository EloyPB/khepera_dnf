#include "Parameters.hpp"

void Parameters::createWindow(NeuralFieldBase *pNeuralField)
{
    this->setWindowTitle(pNeuralField->mFieldName.append(" parameters:"));

    mpVBox = new QVBoxLayout;

    for(int i = 0; i<mpIntegers.size(); i++)
    {
        mpVBox->addWidget(mpIntNames[i]);
        SpinBox *spinBox = new SpinBox(mpIntegers[i], mIntBounds[i]);
        connect(pNeuralField, SIGNAL(updateValues()), spinBox, SLOT(writeValue()));
        mpVBox->addWidget(spinBox->mpSpinBox);
        if(mFieldHasToKnow[i])
            connect(spinBox->mpSpinBox, SIGNAL(valueChanged(int)), pNeuralField, SLOT(update()));
    }

    for(int i = 0; i<mpDoubles.size(); i++)
    {
        mpVBox->addWidget(mpDoubleNames[i]);
        SpinBoxDouble *spinBox = new SpinBoxDouble(mpDoubles[i], mDoubleBounds[i]);
        connect(pNeuralField, SIGNAL(updateValues()), spinBox, SLOT(writeValue()));
        mpVBox->addWidget(spinBox->mpSpinBox);
        if(mFieldHasToKnow[i])
            connect(spinBox->mpSpinBox, SIGNAL(valueChanged(double)), pNeuralField, SLOT(update()));
    }

    mpRestoreButton = new QPushButton("Restore Values", this);
    connect(mpRestoreButton, SIGNAL(released()), pNeuralField, SLOT(readParameters()));
    mpSaveButton = new QPushButton("Save", this);
    connect(mpSaveButton, SIGNAL(released()), pNeuralField, SLOT(writeParameters()));

    mpHBox = new QHBoxLayout;
    mpHBox->addWidget(mpRestoreButton);
    mpHBox->addWidget(mpSaveButton);

    mpVBox->addSpacing(20);
    mpVBox->addLayout(mpHBox);

    this->setLayout(mpVBox);
}

void Parameters::showWindow()
{
    this->show();
}

void Parameters::addInteger(QString name, int *value, vector<int> bounds, bool fieldHasToKnow)
{
    mpIntegers.push_back(value);
    mIntBounds.push_back(bounds);
    mFieldHasToKnow.push_back(fieldHasToKnow);
    QLabel *label = new QLabel(this);
    label->setText(name.append(":"));
    mpIntNames.push_back(label);
}

void Parameters::addDouble(QString name, double *value, vector<double> bounds, bool fieldHasToKnow)
{
    mpDoubles.push_back(value);
    mDoubleBounds.push_back(bounds);
    mFieldHasToKnow.push_back(fieldHasToKnow);
    QLabel *label = new QLabel(this);
    label->setText(name.append(":"));
    mpDoubleNames.push_back(label);
}
