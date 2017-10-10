#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>
#include <QList>

class MyGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    MyGraphicsScene(QObject *parent = 0);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
    //virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);

signals:
    void returnSingleClick(QPointF point);
    void returnDoubleClick(QPointF point);
public slots:

private:
    QPointF point;
};

#endif // GRAPHICSSCENE_H
