#ifndef GOOSE_DATA_HPP
#define GOOSE_DATA_HPP


#include "opencv2/opencv.hpp"
#include "modules/img_proc/CanadaGoose.h"
#include <QObject>
#include <QDebug>
#include <QString>
#include <QDir>
#include <QDirIterator>

using namespace std;
using namespace cv;

class Goose_Data: public QObject{
    Q_OBJECT
public:
    Goose_Data();
    //set functions
    void setData(vector<Rect> contour, vector<Scalar> type, double latitude,double longitude,QString filePath, double altitude, Mat outputImage, QString fromPath);
    void clearData();


    //getters
    QString toString();
    QString getFromPath();
    double getlat();
    double getlong();
    QString getFilePath();
    Mat getImage();
    double getalt();
    vector<Rect> getContour();
    vector<Scalar> getType();

private:
    QString filePath;
    QString fromPath;
    Mat outputImage;
    vector<Rect> contour;
    vector<Scalar> type;
    double latitude;
    double longitude;
    double altitude;
};

#endif
