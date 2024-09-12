#ifndef TANK_H
#define TANK_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QPointF>
#include <QtMath>

class tank : public QGraphicsPixmapItem
{
public:
    tank(const QString& pixmapPath, qreal startX, qreal startY, QGraphicsScene* scene);

    void move(int keyboardMoveY,int leftStickY, float currentAngle);
    void rotate(int keyboardMoveX, int leftStickX, float currentAngle);

    qreal getAngle() const { return currentangle; }
    void setAngle(qreal angle) { currentangle = angle; }
    float currentangle;


private:
    QPixmap tankPixmap;

};

#endif // TANK_H
