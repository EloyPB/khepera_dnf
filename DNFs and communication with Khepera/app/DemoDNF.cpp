#include <Manager.hpp>
#include "NeuralField.hpp"
#include "NeuralNode.hpp"

using namespace std;
using namespace cv;

// creates two 1D fields with input in the middle and projects each along one dimension of a 2D field
int main(int argc, char *argv[]) {

    Mat input1D, input1D2;

    NeuralField Field1D("field 1D", "Config/1D.txt");
    NeuralField Field1D2("field 1D2", "Config/1D2.txt");
    NeuralField Field2D("field 2D", "Config/2D.txt");
    NeuralField Field1DOut("field 1DOut", "Config/1D.txt");
    NeuralField Field1DOut2("field 1DOut2", "Config/1D2.txt");

    NeuralNode CoS("CoS", "Config/0D.txt");

	Manager fieldsManager;
	fieldsManager.addFields({&Field1D, &Field1D2, &Field2D, &Field1DOut, &Field1DOut2, &CoS});

    hconcat(Mat::zeros(1, 40, CV_32F), Mat::ones(1, 20, CV_32F), input1D);
    hconcat(input1D, Mat::zeros(1, 40, CV_32F), input1D);
    Field1D.setInput(input1D, 2, {{0, 0}}, "Config/GaussKernelParams.txt");

    hconcat(Mat::zeros(1, 20, CV_32F), Mat::ones(1, 20, CV_32F), input1D2);
    hconcat(input1D2, Mat::zeros(1, 20, CV_32F), input1D2);
    Field1D2.setInput(input1D2, 1, {{0, 0}}, "Config/GaussKernelParams.txt");

    Field2D.setInputField(&Field1D, 1, {{-1, 1}, {0, 0}}, "Config/GaussKernelParams.txt");
    Field2D.setInputField(&Field1D2, 1, {{-1, 0}, {0, 1}}, "Config/GaussKernelParams.txt");

    Field1DOut.setInputField(&Field2D, 1, {{0, 0}, {1, -1}});

    Field1DOut2.setInputField(&Field2D, 1, {{0, -1}, {1, 0}});

//	double input = 1;
//	CoS.setInput(&input, 1);
    CoS.setInputField(&Field2D, 1);
//	CoS.setOutput({4,8});
//	CoS.addG2OutKernel(0, {1,2}, 10, 2);
//	cout << CoS.mOutputKernels[0] << endl;


    fieldsManager.run(200);

	return 0;
}
