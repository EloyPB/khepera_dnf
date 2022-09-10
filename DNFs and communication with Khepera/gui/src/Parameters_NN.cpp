#include "Parameters_NN.hpp"

Parameters_NN::Parameters_NN(NeuralNode *pNode)
{
    addDouble("sigmoid beta", &pNode->mSigmoidBeta, {0.001, 1000.0}, false);
    addDouble("resting level", &pNode->mSigmoidBeta, {-1000.0, 1000.0}, false);
    addDouble("time constant", &pNode->mTimeConstant, {0.001, 1000.0}, false);
    addDouble("auto excitation", &pNode->mAutoExc, {-1000.0, 1000.0}, false);

    createWindow(pNode);
}
