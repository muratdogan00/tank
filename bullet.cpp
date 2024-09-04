#include "bullet.h"

#include <QBrush>

Bullet::Bullet(qreal x, qreal y, qreal angle) : QGraphicsEllipseItem(0, 0, 10, 10)
{
    setPos(x, y);

    angle_rad = qDegreesToRadians(angle);

    bullet_speed = 30;

    setBrush(Qt::black);
}

void Bullet::advance()
{
    setPos(x() + qSin(angle_rad) * bullet_speed, y() - qCos(angle_rad) * bullet_speed);

    if (pos().x() > 5000 || pos().x() < 0 || pos().y() > 5000 || pos().y() < 0)
    {
        qDebug() << "Bullet marked for deletion";

        shouldDelete = true;
    }
}
