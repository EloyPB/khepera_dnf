#include "GUI.hpp"
#include "Logger.hpp"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    Mat input1D, input1D2;

    NeuralField Field1D("Field 1D", "Config/1D.txt");
    NeuralField Field1D2("Field 1D2", "Config/1D2.txt");
    NeuralField Field2D("Field 2D", "Config/2D.txt");
    NeuralField Field1DOut("Field 1DOut", "Config/1D.txt");
    NeuralField Field1DOut2("Field 1DOut2", "Config/1D2.txt");
    NeuralNode Node("Node", "Config/0D.txt");

    hconcat(Mat::zeros(1, 20, CV_32F), Mat::ones(1, 20, CV_32F), input1D);
    hconcat(input1D, Mat::zeros(1, 60, CV_32F), input1D);
    Field1D.setInput(input1D, 2, {{0, 0}}, "Config/GaussKernelParams.txt");

    hconcat(Mat::zeros(1, 20, CV_32F), Mat::ones(1, 20, CV_32F), input1D2);
    hconcat(input1D2, Mat::zeros(1, 20, CV_32F), input1D2);
    Field1D2.setInput(input1D2, 1, {{0, 0}}, "Config/GaussKernelParams.txt");

    Field2D.setInputField(&Field1D, 1, {{-1, 1}, {0, 0}}, "Config/GaussKernelParams.txt");
    Field2D.setInputField(&Field1D2, 1, {{-1, 0}, {0, 1}}, "Config/GaussKernelParams.txt");

    Field1DOut.setInputField(&Field2D, 1, {{0, 0}, {1, -1}});

    Field1DOut2.setInputField(&Field2D, 1, {{0, -1}, {1, 0}});

    Node.setInputField(&Field2D, 1);

    Manager fieldsManager;
    fieldsManager.addFields({&Field1D, &Field1D2, &Field2D, &Field1DOut, &Field1DOut2, &Node});

//    Logger logger(&fieldsManager, 1);

    DNFGUI myGui(&fieldsManager);

    return app.exec();
}

