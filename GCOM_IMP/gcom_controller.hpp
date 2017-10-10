#ifndef GCOMCONTROLLER_HPP
#define GCOMCONTROLLER_HPP

//===================================================================
// Includes
//===================================================================
// System Includes
#include <QMainWindow>
#include <QString>
#include <QMovie>
#include <QTimer>
#include <QDir>
// GCOM Includes
#include "modules/mavlink_relay/mavlink_relay_tcp.hpp"

// IMP Includes
#include "modules/img_proc/CanadaGoose.h"
#include "modules/imp_API/imp_api.hpp"
#include <QPushButton>
#include <QList>
#include <QThread>
#include <QGraphicsScene>
#include <QPointF>
#include "modules/imp_API/graphicsscene.h"


//===================================================================
// Namespace Declarations
//===================================================================
namespace Ui
{
    class GcomController;
}

//===================================================================
// Class Declarations
//===================================================================
class GcomController : public QMainWindow
{
    Q_OBJECT

public:
    explicit GcomController(QWidget *parent = 0);
    ~GcomController();
    void displayImage(QString path);
    void mousePressEvent(QMouseEvent *event);
    void setUpList();
private slots:
    // UI Slots
    void on_mavlinkConnectionButton_clicked();
    // MAVLinkRelay Slots
    void mavlinkRelayConnected();
    void mavlinkRelayDisconnected();
    void mavlinkTimerTimeout();

    //IMP
    void IMPStartButtonClicked();
    void IMPUpdateButtonClicked();
    void IMPResultButtonClicked();
    void IMPLastButtonClicked();
    void IMPNextButtonClicked();
    void IMPSaveButtonClicked();
    void IMPDeleteButtonClicked();
    void IMPGetButtonClicked();
    void IMPSingleClick(QPointF point);
    void IMPDoubleClick(QPointF point);
    void IMPSaveCSVFile();
private:
    // private member variables
    Ui::GcomController *ui;
    // MAVLinkRelay Variables
    MAVLinkRelay *mavlinkRelay;
    QTimer *mavlinkConnectionTimer;
    void restMavlinkGUI();
    unsigned long mavlinkConnectionTime;
    QMovie *mavlinkConnectingMovie;
    // private member methods
    QString formatDuration(unsigned long seconds);

    //IMP
    CanadaGoose *goose;
    IMP_API *api;
    QList<QString> fileNamePath;
    int counter;
    MyGraphicsScene *viewScene;
    vector<Goose_Data*> gooseSwap;
    Goose_Data* one;
    QPoint addPoint;
    QString txtPath;

};

#endif // GCOMCONTROLLER_HPP
