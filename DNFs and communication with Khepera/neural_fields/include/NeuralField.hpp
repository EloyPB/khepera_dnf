/*
 * NeuralField.hpp
 *
 *  Created on: Dec 19, 2016
 *      Author: eloy
 */

#ifndef INCLUDE_NEURALFIELD_HPP_
#define INCLUDE_NEURALFIELD_HPP_

#include "NeuralFieldBase.hpp"

class NeuralField : public NeuralFieldBase {
public:
    NeuralField(QString fieldName, const char* pFieldConfigFile);

    int initFromFile() override;
	void step(void) override;

    int setInput(Mat input, double strength, vector<vector<int>> dimPairs, const char* pKernelConfigFile = nullptr);
    int setInputField(NeuralField* pInputField, double strength, vector<vector<int>> dimPairs, const char* pKernelConfigFile = nullptr);

public slots:
    int readParameters() override;
    int writeParameters() override;
    void update() override;

public:
    Mat mU, mInputSum, mInputContrib;
	Mat mLateralInteraction;
	Mat mKernel;

    double mGlobInhibition;
    double mGlobInhibitionAmp;

	int mKernelSize;
	double mKernelExcAmp;
	double mKernelInhAmp;
	double mKernelExcSigma;
	double mKernelInhSigma;

	int mMatDim;
    vector<int> mSizes, mMatSize;
	int mFlatSize;

	vector<Mat> mInputs;
    vector<bool> mInputHasKernel;
    vector<Mat> mInputKernels;
	vector<double> mInputStrengths;
    vector<Mat> mInputIndices;
    vector<int> mInputFlatSize;

protected:
	void sigmoid(void);
    void saturate(double lowerBound, double upperBound);
    void globalInhibition(void);
    void convolve(Mat inputMat, Mat outputMat, Mat kernel);
	void addInputs(void);
    void addInput(int inputNum, float *pIn, float *pOut, int* pIndices);
    int prepareInput(Mat inputMat, Mat outputMat, vector<vector<int>> &dimPairs);
    void prepareIndices4Addition(int pendingDims, vector<int> accumulatedIndices, vector<vector<int> > &ptrIncrs,
                                 vector<vector<int>> &dimPairs, int* pIndices, bool inputIsSmaller);
    void setKernel(const char* pKernelConfigFile);
    void createDoGkernel();

private:
    void swapDims(int &dim);
    void fill(Mat mat, float filler);
};

#endif /* INCLUDE_NEURALFIELD_HPP_ */
