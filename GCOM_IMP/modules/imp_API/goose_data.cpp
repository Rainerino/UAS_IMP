#include "goose_data.hpp"

Goose_Data::Goose_Data(){

}

void Goose_Data::setData(vector<Rect> contour, vector<Scalar> type, double latitude,double longitude,QString filePath, double altitude, Mat outputImage, QString fromPath){
    this->contour = contour;
    this->type = type;
    this->latitude = latitude;
    this->longitude = longitude;
    this->filePath = filePath;
    this->altitude = altitude;
    this->outputImage = outputImage;
    this->fromPath = fromPath;
}

QString Goose_Data::toString(){
    QString msg = "The number of Goose is: ";
    msg.append(QString::number(contour.size()));
    msg += "\n The filePath is ";
    msg.append(filePath);
    msg.append("\n The fromPath is ");
    msg.append(fromPath);
    return msg;
}
double Goose_Data::getlat(){
    return this->latitude;
}

double Goose_Data::getlong(){
    return this->longitude;
}

QString Goose_Data::getFilePath(){
    return this->filePath;
}

Mat Goose_Data::getImage(){
    return this->outputImage;
}
double Goose_Data::getalt(){
    return this->altitude;
}
void Goose_Data::clearData(){
    this->contour.clear();
    this->type.clear();
    this->latitude = 0;
    this->longitude = 0;
    this->filePath = "";
    this->altitude = 0;
    this->outputImage.release();
}
vector<Rect> Goose_Data::getContour(){
    return this->contour;
}
QString Goose_Data::getFromPath(){
    return this->fromPath;
}
vector<Scalar> Goose_Data::getType(){
    return this->type;
}
