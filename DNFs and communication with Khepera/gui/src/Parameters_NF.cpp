#include "Parameters_NF.hpp"

Parameters_NF::Parameters_NF(NeuralField *pField)
{
    addInteger("kernel size", &pField->mKernelSize, {1, 1000}, true);
    addDouble("kernel excitatory amplitude", &pField->mKernelExcAmp, {-1000.0, 1000.0}, true);
    addDouble("kernel excitatory sigma", &pField->mKernelExcSigma, {0.001, 1000.0}, true);
    addDouble("kernel inhibitory amplitude", &pField->mKernelInhAmp, {-1000.0, 1000.0}, true);
    addDouble("kernel inhibitory sigma", &pField->mKernelInhSigma, {0.001, 1000.0}, true);
    addDouble("global inhibition", &pField->mGlobInhibitionAmp, {-1000.0, 1000.0}, false);
    addDouble("sigmoid beta", &pField->mSigmoidBeta, {0.001, 1000.0}, false);
    addDouble("resting level", &pField->mRestingLevel, {-1000.0, 1000.0}, false);
    addDouble("time constant", &pField->mTimeConstant, {0.001, 1000.0}, false);

    createWindow(pField);
}
