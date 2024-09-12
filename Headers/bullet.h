#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsEllipseItem>

class Bullet : public QGraphicsPixmapItem
{
public:
    Bullet(qreal x, qreal y, qreal angle);
    void advance();
    bool shouldDelete = false;
    QPixmap bulletpixmap;



private:
    qreal angle_rad;
    qreal bullet_speed;
};

#endif // BULLET_H
