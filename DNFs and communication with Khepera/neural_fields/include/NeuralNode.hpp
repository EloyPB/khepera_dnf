/*
 * NeuralNode.hpp
 *
 *  Created on: Dec 20, 2016
 *      Author: eloy
 */

#ifndef INCLUDE_NEURALNODE_HPP_
#define INCLUDE_NEURALNODE_HPP_

#include "NeuralFieldBase.hpp"
#include "NeuralField.hpp"

class NeuralNode : public NeuralFieldBase {
public:
    NeuralNode(QString nodeName, const char* pFieldConfigFile);

    int initFromFile() override;
	void step(void) override;

	void setInputField(NeuralField* pInputField, double strength);
	void setInputNode(NeuralNode* pInputNode, double strength);
	void setInput(double* input, double strenght);

	int setOutput(vector<int> sizes);
	void addG2OutKernel(int outputNum, vector<int> center, double kernelAmp, double kernelSigma);

public slots:
    int readParameters() override;
    int writeParameters() override;
    void update() override;

public:
	double mAutoExc;
    double mU, mSigmU, mInputSum;

	vector<NeuralField*> mpInputFields;
	vector<double> mInputFieldStrengths;

	vector<double*> mpInputValues;
	vector<double> mInputValueStrengths;

	int mDisplayMatSize;

	vector<Mat> mOutputs;
	vector<vector<int>> mOutputMods;
	vector<int> mOutputFlatSizes;
    vector<Mat> mOutputWeights;

protected:
	void sigmoid();
	void addInputs();
	void addG(int outputNum, int dim, float* pOutKernel, double accumExp, vector<int> center, double kernelAmp, double kernel2SigSqr);

};



#endif /* INCLUDE_NEURALNODE_HPP_ */
