/*
 *
 */


#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/face.hpp"

#include <iostream>
#include <dirent.h>
#include <QString>
#include <QDebug>
#include <QObject>
#include <QDirIterator>

#ifndef UAS_CANADAGOOSE_H
#define UAS_CANADAGOOSE_H

using namespace std;
using namespace cv;
class CanadaGoose{
private:
    /** Global variables */
    const string geese_cascade_name = "/home/yiyi/Documents/CLionProjects/UAS/cascade/new_cascade_for_1_v1.xml";
    CascadeClassifier geese_cascade;
    Mat inputImage;
    QString savePath;
    QString filePathName;
    vector<Rect> boundRect;
    Mat output;
    vector<Scalar> color;
public:
    const int MINI_WIDTH = 50;
    const int MINI_HEIGHT = 50;
    const int MAX_WIDTH = 300;
    const int MAX_HEIGHT = 100;
    CanadaGoose();
    void cascadeRecognition(QString toName, QString fromName);
    void directDetection(cv::Mat src, string cascade, QString fileName);
    void setSavePath(QString path);

    vector<Rect> getContour();
    Mat getOutputImage();
    QString getSavePath();
    vector<Scalar> getColor();
    void writeImage(QString filePath, Mat image);

};
#endif //UAS_CANADAGOOSE_H
