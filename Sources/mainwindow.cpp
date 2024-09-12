#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <SDL.h>
#include <iostream>
#include <stdio.h>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QRandomGenerator>
#include <QImage>
#include<QBrush>
//#include <QWebEngineView>
//#include <QGeoMap>
#include <QtLocation>
#include <QGeoServiceProvider>
//#include <QGeoMapType>
#include <QGeoCoordinate>
#include <QGeoPositionInfoSource>
#include <QVBoxLayout>
#include <QQuickItem>
#include<QGraphicsProxyWidget>
#include <QQuickWidget>
#include <cmath>
#include <QPointF>
//#include <QLineEdit>
#include "tank.h"






MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , gamepad(nullptr)
    , sdlTimer(new QTimer(this))
    , keyboardMoveX(0)
    , keyboardMoveY(0)
    , currentAngle(0)
    ,text1(false)
    ,b(15)
    ,c(0)
    ,d(0)
    ,shoot(false)
    ,isAligned(false)
    ,deneme(false)
    ,leftStickX(0)
    , leftStickY(0)





{


    ui->setupUi(this);
    this->showMaximized();


    graphicsASI = new qfi_ASI;
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);


    view->setFixedSize(1920,1080);
    setCentralWidget(view);
    scene->setSceneRect(0,0,5000,5000);

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scene->setBackgroundBrush(QBrush(QImage("C:/Users/murat/OneDrive/Belgeler/gamepaddeneme/map13.png")));

    graphicsASI->setParent(view);

    graphicsASI->setFixedHeight(200);
    graphicsASI->setFixedWidth(200);

    graphicsASI->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsASI->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    graphicsASI->setGeometry(10, view->height() - graphicsASI->height() - 80, graphicsASI->width(), graphicsASI->height());

    graphicsASI->setStyleSheet("background-color: rgba(255, 255, 255,0);");

    QPixmap targetpixmap("C:/Users/murat/OneDrive/Belgeler/gamepaddeneme/tank22.png");

    QPixmap scaledtargetPixmap = targetpixmap.scaled(targetpixmap.width()/10, targetpixmap.height()/10, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    tank2 = new tank("C:/Users/murat/OneDrive/Belgeler/gamepaddeneme/tank4.png", 1000, 700, scene);

    setupCoordWidget();

    for (int i = 0; i < 3; ++i) {
        QGraphicsPixmapItem *target = scene->addPixmap(scaledtargetPixmap);
        target->setPos(QRandomGenerator::global()->bounded(width()+1000),
                       QRandomGenerator::global()->bounded(height()+1500));
        //target->setBrush(Qt::blue);
        targets.append(target);
    }

    for (size_t i = 0; i < 15; ++i)
    {
        bullet_widget = new QWidget;

        bullet_label = new QLabel(bullet_widget);
        QPixmap bulletpixmap(":/images/bullet2.png");

        bulletpixmap = bulletpixmap.scaled(106, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        bullet_label->setPixmap(bulletpixmap);
        bullet_label->setFixedSize(bulletpixmap.size());

        bullet_layout = new QVBoxLayout(bullet_label);
        bullet_layout-> addWidget(bullet_label);

        bullet_widget->setLayout(bullet_layout);
        bullet_widget->setParent(view);
        bullet_widget->setGeometry(1800, view->height() - bullet_widget->height() + i * 25, bullet_widget->width(), bullet_widget->height());



        Bullets.append(bullet_widget);
    }


    for (size_t i = 0; i < 3; ++i)
    {
        target_tank_widget = new QWidget;

        target_tank_label = new QLabel(target_tank_widget);
        QPixmap tanktargetpixmap(":/images/tank22.png");
        tanktargetpixmap = tanktargetpixmap.scaled(tanktargetpixmap.width() / 10, tanktargetpixmap.height() / 10, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        target_tank_label->setPixmap(tanktargetpixmap);

        target_tank_label->setFixedSize(tanktargetpixmap.size());

        target_tank_layout = new QVBoxLayout(target_tank_label);
        target_tank_layout->addWidget(target_tank_label);

        target_tank_widget->setLayout(target_tank_layout);
        target_tank_widget->setParent(view);
        target_tank_widget->setGeometry(1600 + (i * 40), view->height() - target_tank_widget->height() + 300, target_tank_widget->width(), target_tank_widget->height());

        Targets.append(target_tank_widget);
    }

    this->setFocusPolicy(Qt::StrongFocus);
    this->setFocus();


    bulletTimer = new QTimer(this);
    connect(bulletTimer, &QTimer::timeout, this, &MainWindow::update);
    bulletTimer->start(2000);

    connect(sdlTimer, &QTimer::timeout, this, &MainWindow::update);
    sdlTimer->start(16);


    this->setFocusPolicy(Qt::StrongFocus);


    SDL_GameController *gamepad =nullptr;

    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK|SDL_INIT_GAMECONTROLLER) !=0)
    {
        std::cerr<< "sdl doesnot exist:"<< SDL_GetError() << std::endl;

    }else{


        for (int i = 0; i < SDL_NumJoysticks(); ++i) {
            if (SDL_IsGameController(i)) {
                gamepad = SDL_GameControllerOpen(i);
                if (gamepad) {
                    std::cout << "Gamepad connected: " << SDL_GameControllerName(gamepad) << std::endl;
                    return;
                } else {
                    std::cerr << "Could not open gamepad " << i << ": " << SDL_GetError() << std::endl;
                }
            }
        }
        //
        //std::cerr << "No gamepad found!" << std::endl;
    }

}

MainWindow::~MainWindow()
{
    if (gamepad) {
        SDL_GameControllerClose(gamepad);
    }
    SDL_Quit();
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_Left:  keyboardMoveX = -1;
        qDebug() << " pressed key";
        break;

    case Qt::Key_Right: keyboardMoveX = 1;
        qDebug() << " pressed key";
        break;
    case Qt::Key_Up:    keyboardMoveY = +1;
        qDebug() << " pressed key";
        break;
    case Qt::Key_Down:  keyboardMoveY = -1;
        qDebug() << " pressed key";
        break;
    case Qt::Key_A:     currentAngle -= 2; break;
    case Qt::Key_D:     currentAngle += 2; break;
    case Qt::Key_Space: fireBullet();
        if (Bullets.isEmpty() == false) Bullets.takeFirst()->hide();
        qDebug() << "Space is pressed";
        break;

    case Qt::Key_O:     startTankAI();
        break;
    case Qt::Key_M:    stopTankAI();
        break;

    case Qt::Key_C:    showCoordWidget();
        break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_Left:
    case Qt::Key_Right: keyboardMoveX = 0; break;
    case Qt::Key_Up:
    case Qt::Key_Down:  keyboardMoveY = 0; break;
    }
}

void MainWindow::update()
{

    handleGamepadInput();


    tank2->move(keyboardMoveY,leftStickY,currentAngle);
    tank2->rotate(keyboardMoveX,leftStickX,currentAngle);
    moveObject();
    rotateObject();

    updateBullets();

}

void MainWindow::handleGamepadInput()
{

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            QApplication::quit();
        } else if (e.type == SDL_CONTROLLERBUTTONDOWN){
            if (e.cbutton.button == SDL_CONTROLLER_BUTTON_A) {
                startTankAI();
                std::cout << "Press A" << std::endl;




            }
            if (e.cbutton.button == SDL_CONTROLLER_BUTTON_B) {
                stopTankAI();
                std::cout << "Press B" << std::endl;




            }
            if (e.cbutton.button == SDL_CONTROLLER_BUTTON_Y) {
                showCoordWidget();
                std::cout << "Press Y" << std::endl;




            }
            else  if (e.cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) {
                // currentAngle +=5;
                fireBullet();

                std::cout << "Press A" << std::endl;



            }

        } else if (e.type == SDL_CONTROLLERAXISMOTION) {

            if (e.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX) {
                if (e.caxis.value < 0) {
                    leftStickX=e.caxis.value;

                    std::cout << "Left" << std::endl;
                } else if (e.caxis.value > 0) {
                    leftStickX=e.caxis.value;
                    //currentAngle+=5;
                    //leftStickX=e.caxis.value;
                    std::cout << "Right" << std::endl;
                }
            } else if (e.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY) {
                if (e.caxis.value < 0) {
                    leftStickY = e.caxis.value;


                    std::cout << "Up" << std::endl;
                } else if (e.caxis.value > 0) {
                    leftStickY = e.caxis.value;

                    std::cout << "Down" << std::endl;
                }

            }
            else if (e.cbutton.button == SDL_CONTROLLER_AXIS_TRIGGERRIGHT) {
                //currentAngle +=5;
                fireBullet();
                std::cout << "Press rb" << std::endl;



            }
        }
    }
}

void MainWindow::moveObject()
{

    QPointF position = tank2->pos();

    if (abs(leftStickX) > 0) {
        if(leftStickX<40){
            currentAngle=currentAngle + (leftStickX/15000);


        }else if(leftStickX>40){
            currentAngle=currentAngle + (leftStickX/15000);

        }

    }
    if (abs(leftStickY) > 0) {


        c =(leftStickY / 2500)*3.2;
        graphicsASI->setAirspeed(abs(c));
        graphicsASI->redraw();

    }

    if(qSin(qDegreesToRadians(currentAngle))>0 && qCos(qDegreesToRadians(currentAngle))>0 && keyboardMoveY==1){
        graphicsASI->setAirspeed((qCos(qDegreesToRadians(currentAngle))* keyboardMoveY*40)+(qSin(qDegreesToRadians(currentAngle))*keyboardMoveY*40));
    }
    else if(qSin(qDegreesToRadians(currentAngle))>0 && qCos(qDegreesToRadians(currentAngle))<0 && keyboardMoveY==1){
        graphicsASI->setAirspeed(-(qCos(qDegreesToRadians(currentAngle))* keyboardMoveY*40)+(qSin(qDegreesToRadians(currentAngle))*keyboardMoveY*40));
    }
    else if(qSin(qDegreesToRadians(currentAngle))<0 && qCos(qDegreesToRadians(currentAngle))<0 && keyboardMoveY==1){
        graphicsASI->setAirspeed(-1*((qCos(qDegreesToRadians(currentAngle))* keyboardMoveY*40)+(qSin(qDegreesToRadians(currentAngle))*keyboardMoveY*40)));
    }
    else if(keyboardMoveY==1){
        graphicsASI->setAirspeed(+(qCos(qDegreesToRadians(currentAngle))* keyboardMoveY*40)-(qSin(qDegreesToRadians(currentAngle))*keyboardMoveY*40));
    }
    else if (keyboardMoveY == 0 && leftStickY == 0 && deneme==false){
        graphicsASI->setAirspeed(0);
    }


    graphicsASI->redraw();



    tank2->setPos(position);
    view->centerOn(position);

    qDebug() << "Tank position:" << position;

}
void MainWindow::rotateObject()
{

}

void MainWindow::fireBullet() {


    if(b>0){
        QPointF tankPos = tank2->pos();
        QRectF tankRect = tank2->boundingRect();
        Bullet *bullet = new Bullet(tankPos.x() + tankRect.width() / 2,
                                    tankPos.y() + tankRect.height() / 2 ,
                                    currentAngle);

        scene->addItem(bullet);
        bullets.append(bullet);
        b--;
    }
}
void MainWindow::updateBullets() {



    for (auto it = bullets.begin(); it != bullets.end();) {
        Bullet *bullet = *it;
        bullet->advance();

        bool shouldDelete = bullet->shouldDelete;


        bool collision = false;
        for (QGraphicsPixmapItem *target : targets) {
            if (bullet->collidesWithItem(target)) {
                scene->removeItem(target);
                targets.removeOne(target);
                delete target;
                Targets.takeFirst()->hide();

                collision = true;
                qDebug() << "target destroyed";

                break;
            }
        }


        if (collision || shouldDelete) {
            scene->removeItem(bullet);
            qDebug() << "bullet deleted";
            it = bullets.erase(it);
            delete bullet;


        } else {
            ++it;

        }
    }
    if(text1==false){
        if (targets.isEmpty()) {
            qDebug() << "Targets destroyed";

            ;

            text1=true;

        }
    }
}
QPointF MainWindow::findNearestTarget()
{
    QPointF tankPos = tank2->pos();
    QPointF nearestTargetPos;
    qreal minDistance = std::numeric_limits<qreal>::max();

    for (QGraphicsPixmapItem *target : targets) {
        QPointF targetPos = target->pos();

        qreal distance = std::sqrt(std::pow(targetPos.x() - tankPos.x(), 2) +
                                   std::pow(targetPos.y() - tankPos.y(), 2));


        if (distance < minDistance) {
            minDistance = distance;
            nearestTargetPos = targetPos;
        }
    }

    return nearestTargetPos;
}

void MainWindow::updateTankAI()
{
    QPointF targetPos = findNearestTarget();
    QPointF tankPos = tank2->pos();

    if(shoot == false && currentAngle !=0 &&deneme3==false ){
        if (currentAngle > 0) {
            currentAngle -= 1;
        } else if (currentAngle < 0) {
            currentAngle += 1;
        }

        rotateObject();
        if(currentAngle==0){
            shoot=false;
            isAligned =false;
            deneme=false;
            deneme3=true;
            if (targets.empty()){
                stopTankAI();
            }
        }
    }

    if(tankPos.x() > targetPos.x() && tankPos.y() < targetPos.y() && deneme==false  ){

        qreal targetAngle3 = QLineF(targetPos, tankPos).angle();
        qreal fark = 90 + targetAngle3;
        if(shoot ==false){
            if (qAbs(fark + currentAngle) > 1 ) {
                currentAngle += -1;
                rotateObject();
                tank2->rotate(keyboardMoveX,leftStickX,currentAngle);
            }
            else{
                isAligned = true;
            }

        }
    }

    else   if(tankPos.x()> targetPos.x() && tankPos.y() > targetPos.y() && deneme==false ){


        qreal targetAngle = QLineF(tankPos, targetPos).angle();
        qreal fark = targetAngle - 90;
        if(shoot ==false){
            if (qAbs(fark +currentAngle) > 1 ) {
                currentAngle += -1;
                rotateObject();

                tank2->rotate(keyboardMoveX,leftStickX,currentAngle);
            }
            else{

                isAligned = true;
            }

        }
    }

    else if(tankPos.x()< targetPos.x() && tankPos.y() < targetPos.y() && deneme==false ){

        qreal targetAngle3 = QLineF(targetPos, tankPos).angle();
        qreal fark = 270 - targetAngle3;
        if(shoot ==false){
            if (qAbs(fark - currentAngle) > 1 ) {
                currentAngle += 1;
                rotateObject();
                tank2->rotate(keyboardMoveX,leftStickX,currentAngle);

            }
            else{
                isAligned = true;
            }

        }
    }
    else if(tankPos.x()< targetPos.x() && tankPos.y() > targetPos.y() && deneme==false  ){


        qreal targetAngle = QLineF(tankPos, targetPos).angle();
        qreal fark = 90 - targetAngle;
        if(shoot ==false){
            if (qAbs(fark - currentAngle) > 1 ) {
                currentAngle += 1;
                rotateObject();
                tank2->rotate(keyboardMoveX,leftStickX,currentAngle);


            }
            else{
                isAligned = true;
            }


        }
    }

    if(isAligned){

        deneme =true;


        qreal speed = 3.0;
        qreal dx = speed * qSin(qDegreesToRadians(currentAngle));
        qreal dy = -speed * qCos(qDegreesToRadians(currentAngle));

        tank2->setPos(tankPos + QPointF(dx, dy));
        graphicsASI->setAirspeed(30);
        graphicsASI->redraw();
    }

    if (QLineF(tankPos, targetPos).length() < 200 && shoot==false && currentAngle !=0) {
        fireBullet();
        if (Bullets.isEmpty() == false) Bullets.takeFirst()->hide();
        shoot=true;
    }
    if(shoot == true ){
        graphicsASI->setAirspeed(0);
        graphicsASI->redraw();
        if (currentAngle > 0) {
            currentAngle -= 1;
        } else if (currentAngle < 0) {
            currentAngle += 1;
        }

        rotateObject();
        if(currentAngle==0){
            shoot=false;
            isAligned =false;
            deneme=false;
            if (targets.empty()){
                stopTankAI();
            }
        }
    }

}

void MainWindow::startTankAI()
{
    if (tankAITimer == nullptr) {
        tankAITimer = new QTimer(this);
        connect(tankAITimer, &QTimer::timeout, this, &MainWindow::updateTankAI);
    }

    tankAITimer->start(50);
}

void MainWindow::stopTankAI()
{
    if (tankAITimer != nullptr) {
        tankAITimer->stop();
    }
}

void MainWindow:: initializeTank() {

    QRectF tankRect = tank2->boundingRect();


    tankFrontOffset = QPointF(tankRect.height() / 2 +40, 0);
}


void MainWindow::setupCoordWidget()
{
    coordWidget = new QWidget(this);
    coordWidget->setGeometry(10, 10, 200, 80);
    coordWidget->setStyleSheet("background-color: rgba(255, 255, 255, 200);");

    QVBoxLayout *layout = new QVBoxLayout(coordWidget);

    QLineEdit *xCoordInput = new QLineEdit(coordWidget);
    QLineEdit *yCoordInput = new QLineEdit(coordWidget);

    layout->addWidget(new QLabel("X Coordinate:"));
    layout->addWidget(xCoordInput);
    layout->addWidget(new QLabel("Y Coordinate:"));
    layout->addWidget(yCoordInput);

    coordWidget->setLayout(layout);
    coordWidget->hide();

    auto processCoordinates = [this, xCoordInput, yCoordInput]() {
        bool ok;
        int x = xCoordInput->text().toInt(&ok);
        if (!ok) return;
        int y = yCoordInput->text().toInt(&ok);
        if (!ok) return;

        goToCoordinates(x, y);
        hideCoordWidget();
    };

    connect(xCoordInput, &QLineEdit::returnPressed, yCoordInput, qOverload<>(&QLineEdit::setFocus));
    connect(yCoordInput, &QLineEdit::returnPressed, processCoordinates);
}

void MainWindow::showCoordWidget()
{
    if (coordWidget) {
        coordWidget->show();
        coordWidget->setFocus();
        deneme3 = true;

        QLineEdit *xCoordInput = coordWidget->findChild<QLineEdit*>();
        if (xCoordInput) {
            xCoordInput->setFocus();
        }
    }
}

void MainWindow::goToCoordinates(int x, int y)
{
    QPointF targetPos(x, y);

    QTimer *moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, [this, targetPos, moveTimer]() {
        QPointF tankPos = tank2->pos();
        QLineF line(tankPos, targetPos);


        qreal targetAngle;
        if(tankPos.x() > targetPos.x() && tankPos.y() < targetPos.y()) {
            targetAngle = QLineF(targetPos, tankPos).angle();
            qreal fark = 90 + targetAngle;
            if(qAbs(fark + currentAngle) > 1) {
                currentAngle += -1;
                rotateObject();
                tank2->rotate(keyboardMoveX,leftStickX,currentAngle);
                return;
            }
            else{
                isAligned=true;
            }

        } else if(tankPos.x() > targetPos.x() && tankPos.y() > targetPos.y()) {
            targetAngle = QLineF(tankPos, targetPos).angle();
            qreal fark = targetAngle - 90;
            if(qAbs(fark + currentAngle) > 1) {
                currentAngle += -1;
                rotateObject();
                tank2->rotate(keyboardMoveX,leftStickX,currentAngle);
                return;
            }
            else{
                isAligned=true;
            }

        } else if(tankPos.x() < targetPos.x() && tankPos.y() < targetPos.y()) {
            targetAngle = QLineF(targetPos, tankPos).angle();
            qreal fark = 270 - targetAngle;
            if(qAbs(fark - currentAngle) > 1) {
                currentAngle += 1;
                rotateObject();
                tank2->rotate(keyboardMoveX,leftStickX,currentAngle);
                return;
            }
            else{
                isAligned=true;
            }

        } else if(tankPos.x() < targetPos.x() && tankPos.y() > targetPos.y()) {
            targetAngle = QLineF(tankPos, targetPos).angle();
            qreal fark = 90 - targetAngle;
            if(qAbs(fark - currentAngle) > 1) {
                currentAngle += 1;
                tank2->rotate(keyboardMoveX,leftStickX,currentAngle);
                rotateObject();
                return;
            }
            else{
                isAligned=true;
            }

        }
        if(isAligned){


            if(line.length() > 5) {
                QPointF step = line.unitVector().p2() - line.p1();
                tank2->setPos(tankPos + step * 3);

            } else {

                tank2->setPos(targetPos);
                moveTimer->stop();
                moveTimer->deleteLater();
            }
        }
    }
            );

    moveTimer->start(50);
}


void MainWindow::hideCoordWidget()
{
    coordWidget->hide();
    deneme3 = false;
    deneme=false;
    isAligned=false;
}




