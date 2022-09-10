#include "RobotListener_Hue.hpp"
#include "RobotListener_SendTarget.hpp"
#include "RobotListener_PosLog.hpp"
#include "RobotListener_PolarCoordinates.hpp"
#include "MatTransform_1D.hpp"
#include "NeuralField.hpp"
#include "GUI.hpp"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    // establish the communication
    RobotComm robot("10.42.0.250", 5001);

    // declare listeners to the robot
    HueHistogram hueHistogram(&robot);
    PolarCoordinates polar(&robot);
    polar.initAngleMat(100);
    polar.initDistanceMat(100, 2000);
    polar.initTargetAngleMat(100);
    polar.setTarget(1000, 0);

    // define DNF architecture
    NeuralField HueIntentionField("hue intention", "Params/1DIntention.txt");
    NeuralField PerceptualField("perceptual field", "Params/2DPerceptual.txt");
    NeuralField VisualTargetField("visual target", "Params/1DVisTarget.txt");
    NeuralField DistanceField("distance", "Params/1DIn.txt");
    NeuralField AngleField("angle", "Params/1DIn.txt");
    NeuralField MemoryTargetField("memory target", "Params/1DIn.txt");
    NeuralField CombinedTargetField("combined target", "Params/1DCombinedTarget.txt");

    Mat intention;
    hconcat(Mat::ones(1, 5, CV_32F), Mat::zeros(1, 55, CV_32F), intention);
    //hconcat(intention, Mat::zeros(1, 40, CV_32F), intention);
    HueIntentionField.setInput(intention, 1, {{0, 0}}, "Params/GaussKernelParams.txt");

    DistanceField.setInput(polar.mDistanceMat, 15, {{0, 0}}, "Params/GaussKernelParams.txt");

    AngleField.setInput(polar.mAngleMat, 15, {{0, 0}}, "Params/GaussKernelParams.txt");

    PerceptualField.setInputField(&HueIntentionField, 1, {{0, 1}, {-1, 0}}, "Params/GaussKernelParams.txt");
    PerceptualField.setInput(hueHistogram.mHueHist, 2, {{0, 0}, {1, 1}}, "Params/GaussKernelParams.txt");

    VisualTargetField.setInputField(&PerceptualField, 1, {{0, 0}, {1, -1}}, "Params/GaussKernelParams.txt");

    MemoryTargetField.setInput(polar.mTargetAngleMat, 15, {{0, 0}}, "Params/GaussKernelParams.txt");

    Mat visualTarget = Mat::zeros(1, 100, CV_32FC1);
    MatTransform_1D resizeVisualTarget(VisualTargetField.mSigmoidU, visualTarget, 0.198);
    //CombinedTargetField.setInput(visualTarget, 5, {{0,0}}, "Params/GaussKernelParams.txt");
    CombinedTargetField.setInputField(&MemoryTargetField, 3, {{0,0}}, "Params/GaussKernelParams.txt");

    SendTarget sendTarget(&robot, CombinedTargetField.mSigmoidU);

    // define fields manager
    Manager fieldsManager;
    fieldsManager.addMatTransforms({&resizeVisualTarget});
    fieldsManager.addFields({&HueIntentionField, &PerceptualField, &VisualTargetField, &DistanceField,
                             &AngleField, &MemoryTargetField, &CombinedTargetField});

    // create GUI
    DNFGUI myGui(&fieldsManager, &robot, true);

    return app.exec();
}


