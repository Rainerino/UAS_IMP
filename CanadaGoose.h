/**
 *
 */


#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/face.hpp"
#include <iostream>
#include <dirent.h>

#ifndef UAS_CANADAGOOSE_H
#define UAS_CANADAGOOSE_H

using namespace std;
using namespace cv;
class CanadaGoose{
private:
    /** Global variables */
    const String geese_cascade_name = "/home/yiyi/Documents/CLionProjects/UAS/cascade/new_cascade_for_1_v1.xml";
    const String PATHNAME = "//home/yiyi/Documents/CLionProjects/UAS/Sample/";
    const string NUMBER = "0123456789";
    CascadeClassifier geese_cascade;
    const int HEIGHT = 378;
    const int WIDTH = 504;
    Size smallSize;
    Mat inputImage;
    vector<Mat> inputImageArray;
    vector<Mat> outputIMageArray;
public:
    CanadaGoose();
    void cascadeRecognition();
    int recognition(Mat& imag);
    vector<Mat> edgeDetection(Mat src);
    vector<Mat> imageCutting(Mat bigImage);
    void displayOutput();
};
#endif //UAS_CANADAGOOSE_H
