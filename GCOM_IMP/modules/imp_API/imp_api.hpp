#ifndef IMP_API_HPP
#define IMP_API_HPP

#include "opencv2/opencv.hpp"
#include "modules/img_proc/CanadaGoose.h"
#include <QObject>
#include <QDebug>
#include <QString>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QSize>
#include <QTextStream>
#include <QImageReader>
#include <QImage>
#include <QtCore/qmath.h>
#include <math.h>
#include "goose_data.hpp"
#include <iomanip>
const QString TEXT_FILE_PATH("./assets/impData.txt");

using namespace cv;
using namespace std;

class IMP_API : public QObject{
    Q_OBJECT
 public:
    struct Goose{
        string filePath;
        int species;
        double xcoord;
        double ycoord;
    };
    struct QPointD{
        double x;
        double y;
    };

    IMP_API();
    void setup(QString inputPath, QString outputPath);
    bool start();
    void stop();
    void computeCoordinates(QString textfilePath);
    QString getProcessedImageDir();
    Goose setGooseData(int species, double xcoord, double ycoord, string filePath);
    vector<Goose_Data*> getGooseSwapData();
    void setGooseSwapData(vector<Goose_Data*> gooseSwap);
    void saveData2Text();
    void setData(QString txtPath, vector<vector<QRect>> contour, QString imagePath);
    void calculatePoints();
    vector<vector<QPointF>> getGPS();
    void deletePairs();
    QString getImageDir();
signals:
    void IMPStartButtonReleased();


private:
    vector<Goose_Data*> gooseSwap;
    vector<Goose> gooseReturn;
    QString imageDir;
    vector<QString> imagePath;
    String coordinateCollectPath;
    CanadaGoose* goose;
    QString savePath;

    QString txtPath;
    //QString imageDir;
    vector<vector<double>> rawData;
    vector<vector<QPointF>> points;
    vector<QString> filePaths;
    vector<vector<QRect>> contour;

    const double EARTH_RADIUS = 6356.7523;
    const double DEGREE_FOV_H = 80.5;
    const double DEGREE_FOV_V = 60.3;


};

#endif 
