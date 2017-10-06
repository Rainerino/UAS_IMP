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
    const String geese_cascade_name_bk = "/home/yiyi/Documents/CLionProjects/UAS/cascade/new_cascade_for_2_v2.xml";
    const String PATHNAME = "//home/yiyi/Documents/CLionProjects/UAS/Sample/";
    const string NUMBER = "0123456789";
    CascadeClassifier geese_cascade;
    const int HEIGHT = 250;
    const int WIDTH = 250;
    Size smallSize;
    Mat inputImage;
    Mat outputImage;
    vector<Mat> inputImageArray;
    vector<Mat> outputIMageArray;
    String pathname;
    String savePath;
public:
    CanadaGoose();
    void cascadeRecognition();
    int recognition(Mat& imag, String cascade);
    vector<Mat> edgeDetection(Mat src);
    vector<Mat> imageCutting(Mat bigImage);
    void displayOutput();
    void directDetection(Mat src, String cascade);
    void getPath(String path);
    void saveFile(String path);
    void setSavePath(String path);
};
#endif //UAS_CANADAGOOSE_H
