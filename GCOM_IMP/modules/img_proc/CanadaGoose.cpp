/**
 *
 */

#include "CanadaGoose.h"

CanadaGoose::CanadaGoose() {
    boundRect.clear();
    color.clear();
}

void CanadaGoose::cascadeRecognition(QString toName, QString fromName){
    inputImage= imread(fromName.toStdString());
    directDetection(inputImage, geese_cascade_name, toName);
}
void CanadaGoose::setSavePath(QString savePath){
    this->savePath = savePath;
}

void CanadaGoose::directDetection(Mat src, string cascade, QString toName) {


    Mat mask, thresh, canny, outlines;

    inRange(src, Scalar(165, 165, 165), Scalar(255, 255, 255), mask);

    bitwise_and(src, src, thresh, mask = mask);

    cvtColor(thresh, canny, COLOR_BGR2GRAY);

    //bilateralFilter(output, output, 11, 17, 17);

    Canny(canny, outlines, 30, 200);


    src.copyTo(output);
    vector<vector<Point>> contour;
    boundRect.clear();
    findContours( outlines, contour, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    vector<vector<Point>> contours_poly( contour.size() );


    double area = 0;
    for( unsigned int i = 0; i < contour.size(); i++ ) {
        approxPolyDP( Mat(contour[i]), contours_poly[i], 3, true );
        //boundRect[i] = boundingRect(Mat(contours_poly[i]));
        boundRect.push_back(boundingRect(Mat(contours_poly[i])));
    }

    for (unsigned int i = 0; i< boundRect.size(); i ++) {
        area = boundRect[i].area();
       //cout << area << endl;
        if( area < MINI_WIDTH * MINI_HEIGHT|| area > MAX_WIDTH * MAX_HEIGHT) {
            boundRect.erase(boundRect.begin() + i);
            contour.erase(contour.begin() + i);
            //cout << "delete!" <<endl;
            i--;
        }else{

            rectangle(output, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 0, 255), 6, 8, 0);
        }
    }
    QDir dir(savePath);
    filePathName = dir.absolutePath() + "/" + toName;
    //qInfo()<< filePathName;
    writeImage(filePathName, output);

    //return filePathName;
}

vector<Rect> CanadaGoose::getContour(){
    return boundRect;
}

Mat CanadaGoose::getOutputImage(){
    return output;
}

QString CanadaGoose::getSavePath(){
    return filePathName;
}
vector<Scalar> CanadaGoose::getColor(){
    return color;
}
void CanadaGoose::writeImage(QString filePath, Mat image){
    imwrite(filePathName.toStdString(), image);
}
