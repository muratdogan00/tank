#include "bullet.h"

#include <QBrush>

Bullet::Bullet(qreal x, qreal y, qreal angle) : QGraphicsPixmapItem()
{
    setPos(x, y);

    angle_rad = qDegreesToRadians(angle);

    bullet_speed = 15;
    QPixmap bulletpixmap(":/images/bullet.png");

    bulletpixmap = bulletpixmap.scaled(6, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->setPixmap(bulletpixmap);
    //this->setFixedSize(bulletpixmap.size());

    //setBrush(Qt::black);
}

void Bullet::advance()
{
    QPointF center = QPointF((bulletpixmap.width()) / 2.0, bulletpixmap.height() / 2.0);
    this->setTransformOriginPoint(center);
    qreal angle=qRadiansToDegrees(angle_rad);
    this->setRotation(angle);
    setPos(x() + qSin(angle_rad) * bullet_speed, y() - qCos(angle_rad) * bullet_speed);

    if (pos().x() > 5000 || pos().x() < 0 || pos().y() > 5000 || pos().y() < 0)
    {
        qDebug() << "Bullet marked for deletion";

        shouldDelete = true;
    }
}
