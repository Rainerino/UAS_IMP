#include <QDebug>
#include "modules/imp_API/graphicsscene.h"

MyGraphicsScene::MyGraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{
}


void MyGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    point.rx() = mouseEvent->scenePos().x();
    point.ry() = mouseEvent->scenePos().y();
    emit returnSingleClick(point);
    //QGraphicsScene::mousePressEvent(mouseEvent);
}
void MyGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent){
    point.rx() = mouseEvent->scenePos().x();
    point.ry() = mouseEvent->scenePos().y();
    emit returnDoubleClick(point);
}
