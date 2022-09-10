#ifndef MANAGER_HPP_
#define MANAGER_HPP_

#include "NeuralFieldBase.hpp"
#include "ElementaryBehavior.hpp"
#include "MatTransform.hpp"
#include <QObject>

class Manager : public QObject
{
Q_OBJECT

public:
    Manager(int sleepTime = 10000);
    ~Manager();
    vector<NeuralFieldBase*> mpFields;
    bool mHasGUI, mHasMatTransforms;
    vector<MatTransform*> mpMatTransforms;
    int mSleepTime; // microseconds
    int mRunSteps;


    void addFields(vector<NeuralFieldBase*> pFields);
    void addMatTransforms(vector<MatTransform*> pMatTransforms);
    void addEBs(vector<ElementaryBehavior*> pEBs);

public slots:
    void run(int runSteps);
    void stop();

signals:
    void update();
};

#endif /* MANAGER_HPP_ */
