#include "tank.h"


#include "Tank.h"
#include <QGraphicsScene>
#include <QTransform>



tank::tank(const QString& pixmapPath, qreal startX, qreal startY, QGraphicsScene* scene) {
    tankPixmap = QPixmap(pixmapPath).scaled(QPixmap(pixmapPath).width() / 20, QPixmap(pixmapPath).height() / 20, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setPixmap(tankPixmap);
    setPos(startX,startY);
    currentangle=0;
    scene->addItem(this);
}

void tank::move(int keyboardMoveY,int leftStickY, float currentAngle){
    QPointF position = this->pos();
    if (abs(leftStickY) > 0) {
        //position.setY(position.y() + leftStickY / 3200);
        position.setX(position.x() - qSin(qDegreesToRadians(currentAngle))*(leftStickY / 3200));
        position.setY(position.y() + qCos(qDegreesToRadians(currentAngle))* (leftStickY / 3200));
    }

    position.setX(position.x() + qSin(qDegreesToRadians(currentAngle))*keyboardMoveY*5);
    position.setY(position.y() - qCos(qDegreesToRadians(currentAngle))* keyboardMoveY*5);

    setPos(position);


}

void tank::rotate(int keyboardMoveX, int leftStickX, float currentAngle){

    if (abs(leftStickX) > 0) {
        if(leftStickX<40){
            currentAngle=currentAngle + (leftStickX/15000);
            //leftStickX=40;

        }else if(leftStickX>40){
            currentAngle=currentAngle + (leftStickX/15000);
            //leftStickX=40;
        }
    }
    if (keyboardMoveX != 0) {
        currentangle += keyboardMoveX * 2;
    }

    QPointF center = QPointF((tankPixmap.width()) / 2.0, tankPixmap.height()-30 / 2.0);

    this->setTransformOriginPoint(center);
    //currentAngle =d;
    this->setRotation(currentAngle);
}
