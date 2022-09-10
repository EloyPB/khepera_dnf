#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

int main()
{
    Mat image, hsv;
    image = imread("image.png", CV_LOAD_IMAGE_COLOR);
    hsv = Mat(image.size(), CV_8U);

    namedWindow( "Display window", WINDOW_AUTOSIZE );
    imshow( "Display window", image );

    cvtColor(image, hsv, CV_BGR2HSV);

    const int binSize = 3;
    Mat hist = Mat::zeros(180/binSize, image.size[1], CV_32FC1);

    char h;
    Vec3b pix;

    for(int i = 0; i < hsv.rows; i++)
        for(int j = 0; j < hsv.cols; j++)
        {
            pix = hsv.at<Vec3b>(i,j);
            if (pix[1] > 100)
            {
                h = pix[0]/binSize;
                hist.at<float>(h,j) = 1;
            }
        }


    namedWindow( "hist", WINDOW_AUTOSIZE );
    imshow( "hist", hist );

    waitKey(0);

    return 0;
}
