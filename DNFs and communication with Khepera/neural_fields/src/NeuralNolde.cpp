#include "NeuralNode.hpp"

NeuralNode::NeuralNode(QString nodeName, const char* pFieldConfigFile)
{
    mDimensions = 0;
    mFieldName = nodeName;
    mpConfigFile = pFieldConfigFile;
    initFromFile();
}

int NeuralNode::initFromFile()
{

    readParameters();
    mU = mSigmU = mInputSum = 0;
    mSigmoidU = Mat(1, 1, CV_32FC1);

	return 0;
}

int NeuralNode::readParameters()
{
    //read configuration file
    ifstream myfile (mpConfigFile);
    if (!myfile.is_open())
    {
        cout << "there's no configuration file" << endl;
        return -1;
    }
    else
    {
        readParam(myfile, &mSigmoidBeta);
        readParam(myfile, &mTimeConstant);
        readParam(myfile, &mRestingLevel);
        readParam(myfile, &mAutoExc);

        myfile.close();
    }

    emit updateValues();
}

int NeuralNode::writeParameters()
{
    fstream myFile(mpConfigFile);

    writeDoubleParam(myFile, "SigmoidBeta", mSigmoidBeta);
    writeDoubleParam(myFile, "TimeConstant", mTimeConstant);
    writeDoubleParam(myFile, "RestingLevel", mRestingLevel);
    writeDoubleParam(myFile, "AutoExc", mAutoExc);

    myFile.close();
}

void NeuralNode::update() {}

void NeuralNode::setInputField(NeuralField* pInputField, double strength)
{
	mpInputFields.push_back(pInputField);
	mInputFieldStrengths.push_back(strength);
}
void NeuralNode::setInputNode(NeuralNode* pInputNode, double strength)
{
    setInput(&pInputNode->mSigmU, strength);
}

void NeuralNode::setInput(double* input, double strength)
{
	mpInputValues.push_back(input);
	mInputValueStrengths.push_back(strength);

}

void NeuralNode::step()
{
	addInputs();
    mU += (-mU + mRestingLevel + mInputSum + mAutoExc*mSigmU)/mTimeConstant;
	sigmoid();

    mSigmoidU.at<float>(0,0) = mSigmU;

	for (unsigned int outputNum = 0; outputNum<mOutputs.size(); outputNum++)
        mOutputs[outputNum] = mSigmU*mOutputWeights[outputNum];
}

void NeuralNode::sigmoid(void)
{
    mSigmU = 1/(1+exp(-mSigmoidBeta*mU));
}

void NeuralNode::addInputs()
{
	mInputSum = 0;
	for(unsigned int inputNum=0; inputNum<mpInputValues.size(); inputNum++)
	{
		mInputSum += *mpInputValues[inputNum]*mInputValueStrengths[inputNum];
	}
	for(unsigned int inputNum=0; inputNum<mpInputFields.size(); inputNum++)
	{
		double max = 0;
        float* pIn = mpInputFields[inputNum]->mSigmoidU.ptr<float>(0);
		for (int i = 0; i<mpInputFields[inputNum]->mFlatSize; i++)
			max = max > pIn[i] ? max : pIn[i];
		mInputSum += max*mInputFieldStrengths[inputNum];
	}
}

int NeuralNode::setOutput(vector<int> sizes)
{
	//prepare mats
	int matDims = sizes.size();
	int matSize[matDims];
	copy(sizes.begin(), sizes.end(), matSize);
	if (matDims > 1)
	{
		int temp = matSize[0];
		matSize[0] = matSize[1];
		matSize[1] = temp;
	}

	mOutputs.push_back(Mat(matDims, matSize, CV_32FC1));
    mOutputWeights.push_back(Mat(matDims, matSize, CV_32FC1));

	//calculate total size of mat
	int outputSize = 1;
	for(auto dimSize : sizes)
		outputSize *= dimSize;
	mOutputFlatSizes.push_back(outputSize);

	//calculate numbers for modulus operations for extracting dimensions indices out of flat index
	int mod = 1;
	vector<int> outputMods;
	for(auto dimSize : sizes)
	{
		outputMods.push_back(mod);
		mod *= dimSize;
	}
	mOutputMods.push_back(outputMods);

	return mOutputs.size(); // in case you want to associate a number to the output
}

void NeuralNode::addG2OutKernel(int outputNum, vector<int> center, double kernelAmp, double kernelSigma)
{
    float* pOutKernel = mOutputWeights[outputNum].ptr<float>(0);
	double twoSigmaSqr = 2*kernelSigma*kernelSigma;

	for(int flatIndex = 0; flatIndex<mOutputFlatSizes[outputNum]; flatIndex++)
	{
		int dimIndex;
		double gaussExponent = 0;
		int remainder = flatIndex;
		for(int dimNum = mOutputMods[outputNum].size()-1; dimNum >= 0; dimNum--)
		{
			dimIndex = remainder/mOutputMods[outputNum][dimNum];
			gaussExponent += powf((dimIndex-center[dimNum]), 2);
			remainder = remainder%mOutputMods[outputNum][dimNum];
		}
		pOutKernel[flatIndex] += kernelAmp*exp(-gaussExponent/twoSigmaSqr);
	}
}


