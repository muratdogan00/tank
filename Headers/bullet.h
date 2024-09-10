#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsEllipseItem>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QBrush>

class Bullet : public QGraphicsEllipseItem
{
public:
    Bullet(qreal x, qreal y, qreal angle);
    void advance();
    bool shouldDelete = false;

private:
    qreal angle_rad;
    qreal bullet_speed;
};

#endif // BULLET_H
