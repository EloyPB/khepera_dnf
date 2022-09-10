/*
 * testConvolution.cpp
 *
 *  Created on: Jan 8, 2017
 *      Author: eloy
 */
#include <Manager.hpp>
#include "NeuralField.hpp"

using namespace std;
using namespace cv;


int main(int argc, char *argv[]) {

    NeuralField Field2D("field 2D", "2Db.txt");
	Mat input, output;
	Mat kernel = (Mat_<float>(1,3) << 0.25, 0.5, 0.25);

	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	output = Mat::zeros(100, 100, CV_32F);
	hconcat(Mat::ones(100, 50, CV_32F), Mat::ones(100, 50, CV_32F), input);
    //Field2D.sepConvolution(input, output, kernel, false, 0);
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>( t2 - t1 ).count();
	//cout << output << endl;
	cout << duration << endl;

	t1 = high_resolution_clock::now();
	output = Mat::zeros(100, 100, CV_32F);
    hconcat(Mat::ones(100, 50, CV_32F), Mat::zeros(100, 50, CV_32F), input);
	sepFilter2D(input, output, CV_32FC1, kernel, kernel);
	t2 = high_resolution_clock::now();
	duration = duration_cast<microseconds>( t2 - t1 ).count();
	//cout << output << endl;
	cout << duration << endl;

    Mat final = Mat::zeros(100, 100, CV_32F);;
    cout << "now for something completely different..." << endl;
    t1 = high_resolution_clock::now();
    float *in = input.ptr<float>(0);
    float *out = output.ptr<float>(0);
    int outIndex;
    for(int i = 0; i<10000; i++)
    {
        //outIndex = i/10;
        outIndex = in[i];
        out[outIndex] = out[outIndex] > in[i] ? out[outIndex] : in[i];
    }
    final += output*3.0;
    t2 = high_resolution_clock::now();
    duration = duration_cast<microseconds>( t2 - t1 ).count();
    cout << duration << endl;
}



