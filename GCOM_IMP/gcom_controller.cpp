//===================================================================
// Includes
//===================================================================
// System Includes
#include <QMovie>
#include <QString>
#include <QIntValidator>
#include <QRegExpValidator>
#include <QRegExp>
#include <QTimer>
#include <QDirIterator>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
// GCOM Includes
#include "gcom_controller.hpp"
#include "ui_gcomcontroller.h"
#include "modules/mavlink_relay/mavlink_relay_tcp.hpp"
//IMP Includes
#include "modules/img_proc/CanadaGoose.h"
#include "modules/imp_API/imp_api.hpp"
#include "modules/imp_API/graphicsscene.h"
#include "modules/imp_API/goose_data.hpp"

//===================================================================
// Constants
//===================================================================
const QString DISCONNECT_LABEL("<font color='red'> DISCONNECTED </font>"
                               "<img src=':/connection/disconnected.png'>");
const QString CONNECTING_LABEL("<font color='#EED202'> CONNECTING </font>"
                               "<img src=':/connection/connecting.png'>");
const QString CONNECTED_LABEL("<font color='green'> CONNECTED </font>"
                               "<img src=':/connection/connected.png'>");

const QRegExp IP_REGEX("^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$");

const QString CONNECT_BUTTON_TEXT("Connect");
const QString CONNECTING_BUTTON_TEXT("Cancel Connecting");
const QString DISCONNECT_BUTTON_TEXT("Disconnect");

//IMP
const QString START_BUTTON_TEXT("Start");
const QString START_BUTTON_STARTED_TEXT("Processing...");
const QString START_BUTTON_COMP_TEXT("Completed");
const QString STOP_BUTTON_TEXT("Stop");
const QString RESULT_BUTTON_TEXT("Get All Result");
const QString RESULT_BUTTON_SUCC_TEXT("Result Displayed");
const QString RESULT_BUTTON_ERROR_TEXT("Path Error");
const QString RESULT_BUTTON_NO_FILE("No File!");

//===================================================================
// Class Declarations
//===================================================================
GcomController::GcomController(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GcomController)
{
    // General UI Setup
    ui->setupUi(this);
    ui->mavlinkPortField->setValidator(new QIntValidator(0,1000000));
    ui->mavlinkIPField->setValidator(new QRegExpValidator(IP_REGEX));
    restMavlinkGUI();

    // Mavlink Setup
    mavlinkRelay = new MAVLinkRelay();
    mavlinkConnectionTimer = new QTimer();
    connect(mavlinkConnectionTimer, SIGNAL(timeout()),this, SLOT(mavlinkTimerTimeout()));
    connect(mavlinkRelay, SIGNAL(mavlinkRelayConnected()),this, SLOT(mavlinkRelayConnected()));
    connect(mavlinkRelay, SIGNAL(mavlinkRelayDisconnected()),this, SLOT(mavlinkRelayDisconnected()));
    mavlinkConnectingMovie = new QMovie (":/connection/mavlink_connecting.gif");

    // IMP Setup
    api = new IMP_API();
    viewScene = new MyGraphicsScene();
    one = new Goose_Data();
    connect(ui->start_button, SIGNAL(released()), this, SLOT(IMPStartButtonClicked()));
    connect(ui->update_button, SIGNAL(released()), this, SLOT(IMPUpdateButtonClicked()));
    connect(ui->result_button, SIGNAL(released()), this, SLOT(IMPResultButtonClicked()));
    connect(ui->last_button, SIGNAL(released()), this, SLOT(IMPLastButtonClicked()));
    connect(ui->next_button, SIGNAL(released()), this, SLOT(IMPNextButtonClicked()));
    connect(ui->save_button, SIGNAL(released()), this, SLOT(IMPSaveButtonClicked()));
    connect(ui->delete_button, SIGNAL(released()), this, SLOT(IMPDeleteButtonClicked()));
    connect(ui->get_button, SIGNAL(released()), this, SLOT(IMPGetButtonClicked()));
    connect(ui->saveCSV, SIGNAL(released()), this, SLOT(IMPSaveCSVFile()));

    connect(viewScene, SIGNAL(returnSingleClick(QPointF)), this, SLOT(IMPSingleClick(QPointF)));
    connect(viewScene, SIGNAL(returnDoubleClick(QPointF)), this, SLOT(IMPDoubleClick(QPointF)));

    ui->start_button->setText(START_BUTTON_TEXT);
    ui->update_button->setText(STOP_BUTTON_TEXT);
    ui->result_button->setText(RESULT_BUTTON_TEXT);

    ui->imageOutput->setScene(viewScene);

}

GcomController::~GcomController()
{
    delete ui;
    delete mavlinkRelay;
    delete mavlinkConnectionTimer;
    delete mavlinkConnectingMovie;
}
//===================================================================
// IMP Methods
//===================================================================
void GcomController::setUpList(){
    if(!gooseSwap.empty()) {
        for(int i = 0 ;i < gooseSwap.size(); i ++){
            ui->listWidget->addItem(QString(gooseSwap[i]->getFilePath()));
        }
    }
}

void GcomController::IMPSaveCSVFile(){
    txtPath = "/home/uas/Desktop/GCOM/assets/GPSExifTags.txt";
    vector<vector<Rect>> cvRect;
    cvRect.resize(gooseSwap.size());
    for(int i = 0; i < gooseSwap.size(); i ++ ){
        cvRect[i] = gooseSwap[i]->getContour();
    }
    if(cvRect.empty()) {
        qDebug() << "ASDASd";
    }
    vector<vector<QRect>> qrect;
    qrect.resize(gooseSwap.size());
    for(int i = 0 ; i < cvRect.size(); i ++){
        for(int j = 0 ; j < cvRect[i].size(); j ++){
          qrect[i].push_back(QRect(cvRect[i][j].x,cvRect[i][j].y, cvRect[i][j].width, cvRect[i][j].height ));
        }
    }
    api->setData(txtPath, qrect, api->getImageDir());
    api->calculatePoints();
    api->saveData2Text();
    qInfo() << "SAVED!";
}

void GcomController::IMPStartButtonClicked()
{
    ui->start_button->setText(START_BUTTON_STARTED_TEXT);
    api->setup(ui->inputPath->text(), ui->outputPath->text());
    api->start();
    gooseSwap = api->getGooseSwapData();
    setUpList();
    ui->start_button->setText(START_BUTTON_COMP_TEXT);
}
void GcomController::IMPUpdateButtonClicked(){
    api->saveData2Text();
}
void GcomController::IMPGetButtonClicked(){
    QString file =  ui->listWidget->currentItem()->text();
    for(int i = 0 ;i < gooseSwap.size(); i ++){
        if(gooseSwap[i]->getFilePath() == file){
            counter = i;
            one = gooseSwap[counter];
            displayImage(one->getFilePath());
            break;
        }
    }
}

void GcomController::IMPLastButtonClicked(){
    if(gooseSwap.size() != 0){
        counter --;
        if( counter >= 0){
            one = gooseSwap[counter];
            displayImage(one->getFilePath());
        }else{
            counter = 0;
            one = gooseSwap[counter];
            displayImage(one->getFilePath());
        }
    }


}

void GcomController::IMPNextButtonClicked(){
    if(gooseSwap.size() != 0){
        counter ++;
        if(counter < gooseSwap.size()){
            one = gooseSwap[counter];
            displayImage(one->getFilePath());
        }else{
            counter = gooseSwap.size() - 1;
            one = gooseSwap[counter];
            displayImage(one->getFilePath());
        }
    }


}

void GcomController::IMPSaveButtonClicked(){

}
void GcomController::IMPDeleteButtonClicked(){

}
void GcomController::displayImage(QString path){
    QImage tempImage(path);
    QImage image = tempImage.convertToFormat(QImage::Format_RGB32);
    QPixmap pixmap = QPixmap::fromImage(image);

    int width = ui->imageOutput->geometry().width();
    int height = ui->imageOutput->geometry().height();
    viewScene->setSceneRect(QRectF(0, 0, width, height));
    ui->imageOutput->setMouseTracking(true);
    QGraphicsPixmapItem *item = viewScene->addPixmap(pixmap.scaled(QSize((int)viewScene->width(), (int)viewScene->height())));
    ui->imageOutput->fitInView(QRectF(0, 0, width, height),Qt::KeepAspectRatio);
    //ui->imageOutput->setScene(viewScene);
    ui->imageOutput->show();

}

void GcomController::IMPResultButtonClicked(){
/**
    if(!api->getProcessedImageDir().isEmpty()){
        if(QDir(api->getProcessedImageDir()).entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries).count() != 0){
            ui->result_button->setText(RESULT_BUTTON_SUCC_TEXT);
            QDirIterator it(api->getProcessedImageDir(), QDir::Files, QDirIterator::Subdirectories);
            //stop function to stop this
                while(it.hasNext()){
                    it.next();
                    QDir dir(it.filePath());
                    fileNamePath.push_back(dir.absolutePath());
                }
                qSort(fileNamePath.begin(), fileNamePath.end());
                counter = 0;
                displayImage(fileNamePath[counter]);
            }else{
                ui->result_button->setText(RESULT_BUTTON_ERROR_TEXT);
            }
        }else{
            ui->result_button->setText(RESULT_BUTTON_NO_FILE);
        }
        */
    if(gooseSwap.size()!=0){
            counter = 0;
            one = gooseSwap[counter];
            displayImage(one->getFilePath());
    }else{
        ui->result_button->setText(RESULT_BUTTON_NO_FILE);
    }
}

void GcomController::mousePressEvent(QMouseEvent* event){
   /* QPoint point;
    point.setX(event->x()-415);
    point.setY(event->y()-157);
    qInfo() << QString(one->toString());
    int width = ui->imageOutput->geometry().width();
    int height = ui->imageOutput->geometry().height();
    point.setX(point.x()*1.0/width*one->getImage().cols);
    point.setY(point.y()*1.0/height*one->getImage().rows);
    qInfo() << point.x();
    qInfo() << point.y();

    one = gooseSwap[counter];
    vector<vector<Point>> contour = one->getContour();
    Mat output = imread(one->getFromPath().toStdString());
    vector<Rect> boundRect(contour.size());
    vector<vector<Point>> contours_poly( contour.size() );

    for( unsigned int i = 0; i < contour.size(); i++ ) {
        approxPolyDP( Mat(contour[i]), contours_poly[i], 3, true );
        boundRect[i] = boundingRect(Mat(contours_poly[i]));
            if( boundRect[i].contains(Point(point.x(), point.y()))) {
                boundRect.erase(boundRect.begin() + i);
                contour.erase(contour.begin() + i);
                i--;
            }else{
                rectangle(output, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 0, 255), 2, 8, 0);
            }
    }
    Goose_Data* data = new Goose_Data();
    QFile::remove(one->getFilePath());
    qInfo() << one->getFilePath();
    imwrite(one->getFilePath().toStdString(), output);
    data->setData(contour, one->getType(), 0, 0,one->getFilePath(), 0, output, one->getFromPath());


    gooseSwap[counter] = data;
    displayImage(data->getFilePath());*/
}

void GcomController::IMPSingleClick(QPointF point)
{

    double width = ui->imageOutput->geometry().width();
    double height = ui->imageOutput->geometry().height();

    addPoint.setX(point.x()*1.0/width * 1.0 * one->getImage().cols);
    addPoint.setY(point.y()*1.0/height * 1.0 * one->getImage().rows);

    qInfo() << "S";
    qInfo() << addPoint.x();
    qInfo() << addPoint.y();

    one = gooseSwap[counter];
    Mat output = imread(one->getFromPath().toStdString());
    vector<Rect> boundRect = one->getContour();
    for( unsigned int i = 0; i < boundRect.size(); i++ ) {
            if( boundRect[i].contains(Point(addPoint.x(), addPoint.y()))) {
                boundRect.erase(boundRect.begin() + i);
                i--;
                qInfo() << "GOOOOOOT it";
            }else{
                rectangle(output, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 0, 255), 4, 8, 0);
            }
    }
    Goose_Data* data = new Goose_Data();
    QFile::remove(one->getFilePath());
    //qInfo() << one->getFilePath();
    imwrite(one->getFilePath().toStdString(), output);
    data->setData(boundRect, one->getType(), 0, 0,one->getFilePath(), 0, output, one->getFromPath());
    gooseSwap[counter] = data;
    displayImage(data->getFilePath());

}
void GcomController::IMPDoubleClick(QPointF point){

    double width = ui->imageOutput->geometry().width();
    double height = ui->imageOutput->geometry().height();

    point.setX(point.x()*1.0/width * 1.0 * one->getImage().cols);
    point.setY(point.y()*1.0/height * 1.0 * one->getImage().rows);

    qInfo() << "D";
    qInfo() << point.x();
    qInfo() << point.y();

    one = gooseSwap[counter];
    Mat output = imread(one->getFromPath().toStdString());


    int w = 100;
    int h = 100;
    qInfo() << "RECT";
    qInfo() << w;
    qInfo() << h;

    int x, y;
    (point.x() - w/2 <0 )? x = 0 : x = point.x() - w/2;
    (point.y() - h/2 <0 )? y = 0 : y = point.y() - h/2;
    qInfo() << "RECT";
    qInfo() << w;
    qInfo() << h;
    qInfo() << x;
    qInfo() << y;
    vector<Rect> contour = one->getContour();
    contour.push_back(Rect(x, y, w, h));

    for (int i = 0 ; i < contour.size() - 1; i ++) {
        rectangle(output, contour[i].tl(), contour[i].br(), Scalar(0, 0, 255), 4, 8, 0);
    }
    rectangle(output, contour[contour.size()-1].tl(), contour[contour.size()-1].br(), Scalar(255, 0, 0), 4, 8, 0);
    Goose_Data* data = new Goose_Data();
    QFile::remove(one->getFilePath());
    qInfo() << "ADDED!";
    imwrite(one->getFilePath().toStdString(), output);
    data->setData(contour, one->getType(), 0, 0,one->getFilePath(), 0, output, one->getFromPath());
    gooseSwap[counter] = data;
    displayImage(data->getFilePath());

}

//===================================================================
// MAVLink Methods
//===================================================================

void GcomController::restMavlinkGUI()
{
    mavlinkConnectionTime = 0;
    ui->mavlinkStatusField->setText(DISCONNECT_LABEL);
    ui->mavlinkConnectionStatusField->setText(DISCONNECT_LABEL);
    ui->mavlinkConnectionTime->display(formatDuration(mavlinkConnectionTime));
    ui->mavlinkConnectionButton->setText(CONNECT_BUTTON_TEXT);
    // Enable all input fields
    ui->mavlinkIPField->setDisabled(false);
    ui->mavlinkPortField->setDisabled(false);


}

void GcomController::mavlinkTimerTimeout()
{
    ui->mavlinkConnectionTime->display(formatDuration(++mavlinkConnectionTime));
}

void GcomController::on_mavlinkConnectionButton_clicked()
{
    // If the MAVLink relay was disconnected the state machine progresses to
    // the connection stage
    if (mavlinkRelay->status() == MAVLinkRelay::MAVLinkRelayStatus::DISCCONNECTED)
    {
        // Disable all input fields
        ui->mavlinkIPField->setDisabled(true);
        ui->mavlinkPortField->setDisabled(true);
        // Change the GUI elements to reflect that the relay is in the
        // connecting stage
        ui->mavlinkConnectionButton->setText(CONNECTING_BUTTON_TEXT);
        ui->mavlinkStatusField->setText(CONNECTING_LABEL);
        ui->mavlinkConnectionStatusField->setText(CONNECTING_LABEL);
        ui->mavlinkStatusMovie->setMovie(mavlinkConnectingMovie);
        mavlinkConnectingMovie->start();
        // Start the MAVLinkRelay
        mavlinkRelay->setup(ui->mavlinkIPField->text(), ui->mavlinkPortField->text().toInt());
        mavlinkRelay->start();
    }
    // If the relay is in any other state (Connecting or Connected) then it is
    // stopped
    else
    {
        mavlinkRelay->stop();
    }
}

void GcomController::mavlinkRelayConnected()
{
    // Update the labels the on screen text to indicate that we have connected
    ui->mavlinkStatusField->setText(CONNECTED_LABEL);
    ui->mavlinkConnectionStatusField->setText(CONNECTED_LABEL);
    ui->mavlinkConnectionButton->setText(DISCONNECT_BUTTON_TEXT);
    // Stop the connection movie
    ui->mavlinkStatusMovie->setText(" ");
    mavlinkConnectingMovie->stop();
    // Start the timer
    mavlinkConnectionTimer->start(1000);
}

void GcomController::mavlinkRelayDisconnected()
{
    if (ui->mavlinkAutoReconnect->isChecked())
    {
        on_mavlinkConnectionButton_clicked();
        return;
    }
    // When a disconnection is detected then the GUI is reset to the unconnected
    // state
    restMavlinkGUI();
    // Stop the connection movie
    ui->mavlinkStatusMovie->setText(" ");
    mavlinkConnectingMovie->stop();
    // Stop the timer
    mavlinkConnectionTimer->stop();
}




//===================================================================
// Utility Methods
//===================================================================
QString GcomController::formatDuration(unsigned long seconds)
{
    // Convert seconds into
    int minutes = seconds / 60;
    seconds = seconds % 60;
    int hours = minutes / 60;
    minutes = minutes % 60;

    return QString("%1:%2:%3").arg(hours).arg(minutes).arg(seconds);
}


