#include <Manager.hpp>

Manager::Manager(int sleepTime) : mSleepTime(sleepTime)
{
    mHasGUI = false;
}

Manager::~Manager()
{
}

void Manager::addFields(vector<NeuralFieldBase*> pFields)
{
    for(auto pField : pFields)
    {
        mpFields.push_back(pField);
        pField->mFieldNum = mpFields.size();
	}
}

void Manager::addEBs(vector<ElementaryBehavior*> pEBs)
{
    for(auto pEB : pEBs)
	{
        mpFields.push_back(&pEB->mIntentionNode);
        pEB->mIntentionNode.mFieldNum = mpFields.size();

        mpFields.push_back(&pEB->mCoSNode);
        pEB->mCoSNode.mFieldNum = mpFields.size();

        mpFields.push_back(&pEB->mIntentionField);
        pEB->mIntentionField.mFieldNum = mpFields.size();

        mpFields.push_back(&pEB->mCoSField);
        pEB->mCoSField.mFieldNum = mpFields.size();
	}
}

void Manager::addMatTransforms(vector<MatTransform*> pMatTransforms)
{
    for(auto pMatTransform : pMatTransforms)
        mpMatTransforms.push_back(pMatTransform);
}

void Manager::run(int runSteps)
{
    int step = 0;
    mRunSteps = runSteps;
    while(mRunSteps<0 || step<mRunSteps)
    {
        for(auto pField : mpFields)
            pField->step();

        for(auto pMatTransform : mpMatTransforms)
            pMatTransform->transform();

        emit update();

        if(!mHasGUI)
        {
            for(auto pField : mpFields)
                pField->cvShow();
        }

        usleep(mSleepTime);

        step++;
    }
}

void Manager::stop()
{
    mRunSteps = 0;
}


