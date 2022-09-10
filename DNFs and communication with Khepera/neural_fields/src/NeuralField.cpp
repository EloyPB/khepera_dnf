/*
 * NeuralField.cpp
 *
 *  Created on: Dec 19, 2016
 *      Author: eloy
 */

#include "NeuralField.hpp"

NeuralField::NeuralField(QString fieldName, const char* pFieldConfigFile)
{
    mFieldName = fieldName;
    mpConfigFile = pFieldConfigFile;
    initFromFile();
}

int NeuralField::readParameters()
{
    ifstream myFile(mpConfigFile);
    if (!myFile.is_open())
    {
        cout << "there's no configuration file" << endl;
        return 1;
    }
    else
    {
        readParam(myFile, &mSigmoidBeta);
        readParam(myFile, &mTimeConstant);
        readParam(myFile, &mRestingLevel);
        readParam(myFile, &mKernelSize);
        readParam(myFile, &mKernelExcAmp);
        readParam(myFile, &mKernelInhAmp);
        readParam(myFile, &mKernelExcSigma);
        readParam(myFile, &mKernelInhSigma);
        readParam(myFile, &mGlobInhibitionAmp);
        readParam(myFile, &mDimensions);
        int size;
        mDimensions == 1 ? mMatDim = 2 : mMatDim = mDimensions;
        mFlatSize = 1;
        for (int i=0; i<mDimensions; i++)
        {
            readParam(myFile, &size);
            mSizes.push_back(size);
            mFlatSize *= size;
        }
        myFile.close();
    }

    emit updateValues();
    return 0;
}

int NeuralField::initFromFile()
{
    readParameters();

    //create variables
    mMatSize = mSizes;
    if(mDimensions == 1)
        mMatSize.push_back(1);

    int temp = mMatSize[0];
    mMatSize[0] = mMatSize[1];
    mMatSize[1] = temp;

    int matSize[mMatSize.size()];
    copy(mMatSize.begin(), mMatSize.end(), matSize);

    mU = Mat(mMatDim, matSize, CV_32FC1);
    fill(mU, 0.0);
    mSigmoidU = Mat(mMatDim, matSize, CV_32FC1);
    sigmoid();
    mInputSum = Mat(mMatDim, matSize, CV_32FC1);
    mInputContrib = Mat(mMatDim, matSize, CV_32FC1);
    mLateralInteraction = Mat(mMatDim, matSize, CV_32FC1);

    createDoGkernel();

    return 0;
}

int NeuralField::writeParameters()
{
    fstream myFile(mpConfigFile);
    writeDoubleParam(myFile, "SigmoidBeta", mSigmoidBeta);
    writeDoubleParam(myFile, "TimeConstant", mTimeConstant);
    writeDoubleParam(myFile, "RestingLevel", mRestingLevel);
    writeIntParam(myFile, "KernelSize", mKernelSize);
    writeDoubleParam(myFile, "KernelExcAmp", mKernelExcAmp);
    writeDoubleParam(myFile, "KernelInhAmp", mKernelInhAmp);
    writeDoubleParam(myFile, "KernelExcSigma", mKernelExcSigma);
    writeDoubleParam(myFile, "KernelInhSigma", mKernelInhSigma);
    writeDoubleParam(myFile, "GlobalInhibitionAmp", mGlobInhibitionAmp);
    writeIntParam(myFile, "Dimensions", mDimensions);
    for (int i=0; i<mDimensions; i++)
    {
        string name = "Size";
        name.append(to_string(i));
        writeIntParam(myFile, name.c_str(), mSizes[i]);
    }
    myFile.close();
}

inline void NeuralField::fill(Mat mat, float filler)
{
    float *p = mat.ptr<float>(0);
    for(int i=0; i<mFlatSize; i++)
        p[i] = filler;
}

void NeuralField::update()
{
    createDoGkernel();
}

void NeuralField::createDoGkernel()
{
    if(mKernelExcSigma <= 0 || mKernelInhSigma <= 0)
        cout << "ERROR: Kernel sigma has to be positive" << endl;

    mKernel = Mat(1, mKernelSize, CV_32FC1);
    int centerX = mKernelSize/2;

    for (int i=0; i<mKernelSize; i++)
            mKernel.at<float>(0,i) = mKernelExcAmp * exp(-powf(i-centerX, 2) / (2*powf(mKernelExcSigma, 2)))
            + mKernelInhAmp * exp(-powf(i-centerX, 2) / (2*powf(mKernelInhSigma, 2)));
}

void NeuralField::step(void)
{
//    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    convolve(mSigmoidU, mLateralInteraction, mKernel);

	addInputs();

    if(mGlobInhibitionAmp == 0)
        mU += (-mU + mRestingLevel + mInputSum + mLateralInteraction)/mTimeConstant;
    else
    {
        globalInhibition();
        mU += (-mU + mRestingLevel - mGlobInhibition + mInputSum + mLateralInteraction)/mTimeConstant;
    }

    mOutputMutex.lock();
	sigmoid();
    mOutputMutex.unlock();

//    high_resolution_clock::time_point t2 = high_resolution_clock::now();
//    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
//    cout << duration << endl;
}

void NeuralField::globalInhibition(void)
{
    float* ptr = mSigmoidU.ptr<float>(0);
    double sum = 0;
    for(int i = 0; i<mFlatSize; i++)
        sum += ptr[i];
    mGlobInhibition = mGlobInhibitionAmp * sum;
}

void NeuralField::sigmoid(void)
{
    Mat exp_of_minus_u = Mat(mU.size(), CV_32FC1);
    exp((-1)*mSigmoidBeta*mU, exp_of_minus_u);
    mSigmoidU = 1/(1+exp_of_minus_u);
}

void NeuralField::addInputs(void)
{
    fill(mInputSum, 0.0);

	for(unsigned int inputNum=0; inputNum<mInputs.size(); inputNum++)
	{
        fill(mInputContrib, 0.0);
        float *pIn;
        Mat inputConvolved = Mat(mInputs[inputNum].size(), CV_32FC1);
		if (mInputHasKernel[inputNum])  // convolve inputs
		{
            convolve(mInputs[inputNum], inputConvolved, mInputKernels[inputNum]);
            pIn = inputConvolved.ptr<float>(0);
		}	
		else 
			pIn = mInputs[inputNum].ptr<float>(0);

        float *pOut = mInputContrib.ptr<float>(0);
        int *pIndices = mInputIndices[inputNum].ptr<int>(0);
        addInput(inputNum, pIn, pOut, pIndices);
        mInputSum += mInputContrib*mInputStrengths[inputNum];
	}
}

void NeuralField::addInput(int inputNum, float* pIn, float* pOut, int* pIndices)
{
    if (mFlatSize >= mInputFlatSize[inputNum]) // input is smaller
        for(int index = 0; index < mFlatSize; index++)
            pOut[index] = pIn[pIndices[index]];
    else
        for(int index = 0; index < mInputFlatSize[inputNum]; index++) // input is larger
        {
            pOut[pIndices[index]] = pOut[pIndices[index]] > pIn[index] ? pOut[pIndices[index]] : pIn[index];
        }
}

void NeuralField::convolve(Mat inputMat, Mat outputMat, Mat kernel) // fix for multidimensional
{
	if (mDimensions == 1)
        filter2D(inputMat, outputMat, CV_32FC1, kernel);
	else if (mDimensions == 2)
        sepFilter2D(inputMat, outputMat, CV_32FC1, kernel, kernel);
    else
        cout << "Sorry, I can't handle 3 Dimensions :(" << endl;
}

int NeuralField::setInputField(NeuralField* pInputField, double strength, vector<vector<int>> dimPairs, const char* pKernelConfigFile)
{
    if(setInput(pInputField->mSigmoidU, strength, dimPairs, pKernelConfigFile))
		return 1;
	return 0;
}

int NeuralField::setInput(Mat input, double strength, vector<vector<int>> dimPairs, const char* pKernelConfigFile)
{
    mInputs.push_back(input);
	mInputStrengths.push_back(strength);
    if (prepareInput(input, mU, dimPairs))
		return 1;
    setKernel(pKernelConfigFile);
	return 0;
}

void NeuralField::swapDims(int &dim)
{
    if (dim == 0) dim = 1;
    else if (dim == 1) dim = 0;
}

int NeuralField::prepareInput(Mat inputMat, Mat outputMat, vector<vector<int>> &dimPairs)
{
    int dimensionality[2] = {0,0};

	for (unsigned int pairNum = 0; pairNum < dimPairs.size(); pairNum++)
	{
		int inputSize, outputSize, size, inputDim, outputDim;

		// dimension 0 and 1 are swapped
        inputDim = dimPairs[pairNum][0];
        swapDims(inputDim);
        outputDim = dimPairs[pairNum][1];
        swapDims(outputDim);

        // find sizes and check that they match, append larger size to dimPair
		if (inputDim >= 0 && outputDim >= 0)
		{
			inputSize = inputMat.size[inputDim];
			outputSize = outputMat.size[outputDim];
			if (inputSize != outputSize)
			{
                cout << "ERROR: sizes don't match! (in " << mFieldName.toUtf8().constData() << ")" << endl;
                cout << "input size: " << inputSize << " field size: " << outputSize << endl;
				return 1;
			}
			else
			{
				size = inputSize;
                dimensionality[0]++; dimensionality[1]++;
			}
		}
		else if (inputDim >= 0)
		{
			size = inputMat.size[inputDim];
            dimensionality[0]++;
		}
		else if (outputDim >= 0)
		{
			size = outputMat.size[outputDim];
            dimensionality[1]++;
		}

		dimPairs[pairNum].push_back(size);
	}

    // create Mat that will contain the flat indices that have to be acessed in the the smaller Mat (input or internal summation)
    // as we loop over the elements in the larger Mat

        // crete the Mat
    bool inputIsSmaller;
    Mat indices;
    if(dimensionality[0] <= dimensionality[1])
    {
        inputIsSmaller = true;
        int sz[outputMat.dims];
        for(int dimNum = 0; dimNum < outputMat.dims; dimNum++)
            sz[dimNum] = outputMat.size[dimNum];
        indices = Mat(outputMat.dims, sz, CV_32S);
    }
    else
    {
        inputIsSmaller = false;
        int sz[inputMat.dims];
        for(int dimNum = 0; dimNum < inputMat.dims; dimNum++)
            sz[dimNum] = inputMat.size[dimNum];
        indices = Mat(inputMat.dims, sz, CV_32S);
    }

        // calculate the pointer index increments that have to be multiplied by the dimension index while looping
    vector<vector<int>> ptrIncrs;
    vector<int> ptrIncrIn, ptrIncrOut;
    ptrIncrIn.push_back(1);
    for(int dimNum = 1; dimNum < dimensionality[0]; dimNum++)
    {
        int dim = dimNum-1;
        swapDims(dim);
        ptrIncrIn.push_back(ptrIncrIn.back()*inputMat.size[dim]);
    }
    ptrIncrs.push_back(ptrIncrIn);

    ptrIncrOut.push_back(1);
    for(int dimNum = 1; dimNum < dimensionality[1]; dimNum++)
    {
        int dim = dimNum-1;
        swapDims(dim);
        ptrIncrOut.push_back(ptrIncrOut.back()*outputMat.size[dim]);
    }
    ptrIncrs.push_back(ptrIncrOut);

        // calculate the indices with 'prepareIndices4Addition(...)'
    int *pIndices = indices.ptr<int>(0);
    prepareIndices4Addition(dimPairs.size()-1, {0,0}, ptrIncrs, dimPairs, pIndices, inputIsSmaller);

    int inputFlatSize = 1;
    for(int dimNum = 0; dimNum < inputMat.dims; dimNum++)
        inputFlatSize *= inputMat.size[dimNum];

    mInputFlatSize.push_back(inputFlatSize);
    mInputIndices.push_back(indices);

	return 0;
}

void NeuralField::prepareIndices4Addition(int pendingDims, vector<int> accumulatedIndices, vector<vector<int>> &ptrIncrs,
    vector<vector<int>> &dimPairs, int* pIndices, bool inputIsSmaller)
{
    if (pendingDims >= 0)
    {
        for (int i = 0; i < dimPairs[pendingDims][2]; i++)
        {
            int dimIn = dimPairs[pendingDims][0];
            int dimOut = dimPairs[pendingDims][1];

            vector<int> index = {0,0};
            index[0] = accumulatedIndices[0];
            index[1] = accumulatedIndices[1];

            if(dimIn >= 0)
                index[0] += i*ptrIncrs[0][dimIn];
            if(dimOut >= 0)
                index[1] += i*ptrIncrs[1][dimOut];

            prepareIndices4Addition(pendingDims-1, index, ptrIncrs, dimPairs, pIndices, inputIsSmaller);

            if (pendingDims == 0)
            {
                if(inputIsSmaller)
                    pIndices[index[1]] = index[0];
                else
                    pIndices[index[0]] = index[1];
            }
        }
    }
}

void NeuralField::setKernel(const char* pKernelConfigFile)
{
	if (pKernelConfigFile != nullptr)
	{
		int kernelSize;
		double kernelAmp;
		double kernelSigma;

        ifstream myFile (pKernelConfigFile);
        if (!myFile.is_open())
		{
			cout << "there's no configuration file" << endl;
		}
        readParam(myFile, &kernelSize);
        readParam(myFile, &kernelAmp);
        readParam(myFile, &kernelSigma);

        myFile.close();

        Mat kernel = Mat(1, kernelSize, CV_32FC1);
		createGkernel(kernel, kernelSize, kernelAmp, kernelSigma);
		mInputKernels.push_back(kernel);
		mInputHasKernel.push_back(true);
	}
	else
	{
		Mat dummy;
		mInputKernels.push_back(dummy);
		mInputHasKernel.push_back(false);
	}
}




