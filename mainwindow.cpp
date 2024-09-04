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




Bullet::Bullet(qreal x, qreal y, qreal angle) : QGraphicsEllipseItem(0, 0, 10, 10) {
    setPos(x, y);

    angle_rad = qDegreesToRadians(angle);
    speed = 25; // Mermi hızı
    setBrush(Qt::red);
    //bool shouldDelete = false;
}

void Bullet::advance() {
    setPos(x() + qSin(angle_rad) * speed, y() - qCos(angle_rad) * speed);

    if (pos().x() > 5000 || pos().x() < 0 || pos().y() > 5000 || pos().y() < 0) {

        qDebug() << "bullet marked for deletion";
        shouldDelete = true;


    }


}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , gamepad(nullptr)
    , sdlTimer(new QTimer(this))
    //, isKeyboardControlActive(false)
    , keyboardMoveX(0)
    , keyboardMoveY(0)
    , currentAngle(0)

    //, targetAngle(0)
    ,text1(false)
    ,b(10)
    ,c(0)
    ,leftStickX(0)
    , leftStickY(0)





{


    ui->setupUi(this);
    this->showMaximized();

    //qfi_EADI *graphicsEADI = new qfi_EADI;


    //QGraphicsRectItem *rect = new QGraphicsRectItem(0, 0, 200, 200);
    graphicsASI = new qfi_ASI;

    //QQuickView *view2 = new QQuickView();








    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);
    tank1 = new QGraphicsPixmapItem();
    view->setFixedSize(1920,1080);
    //this->centralWidget()->setStyleSheet("background-image:url(\"bkg.jpg\"); background-position: center; ");
    setCentralWidget(view);
    scene->setSceneRect(0,0,5000,5000);


    QQuickWidget *mapWidget = new QQuickWidget(this);
    mapWidget->setSource(QUrl::fromLocalFile("QML/Map.qml"));

    mapWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    mapWidget->setFixedSize(600, 800);



   // QGraphicsProxyWidget *proxyWidget = scene->addWidget(mapWidget);
    //proxyWidget->setZValue(-1); // Hari
    //QPixmap bkgrnd("C:/Users/murat/OneDrive/Resimler/Screenshot 2024-08-27 100813.png");
    //bkgrnd = bkgrnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    //QPalette palette;
    //palette.setBrush(QPalette::Window, bkgrnd);
    //this-> setPalette(palette);
    //scene-> setPalette(palette);
    label1 = scene->addRect(0,0,60,30);
    //backgroundBrush(QBrush("C:/Users/murat/OneDrive/Resimler/Screenshot 2024-08-27 100813.png"));
    //QBrush bgrn("C:/Users/murat/OneDrive/Resimler/devre.png");
    scene->setBackgroundBrush(QBrush(QImage("C:/Users/murat/OneDrive/Belgeler/gamepaddeneme/download.png")));
    scene->addWidget(mapWidget);

    //scene->addWidget(gra)


    label1-> setPos(1800,0);
    //scene->addWidget(graphicsEADI);

    //rect->setPos(0,0);

    QGeoServiceProvider serviceProvider("osm"); // "osm" for OpenStreetMap
    //QGeoMapType mapType = serviceProvider.mapTypes().first();
    QGeoCoordinate centerCoordinate(37.7749, -122.4194); // Example: San Francisco coordinates

    //QGeoMap *geoMap = serviceProvider.createMap(mapType);
   // geoMap->setCenter(centerCoordinate);
    //geoMap->setZoomLevel(14);

    // Create a map item
    //QGraphicsGeoMap *mapItem = new QGraphicsGeoMap(geoMap);

    // Add the map item to the scene
    //scene.addItem(mapItem);


    //scene->addWidget(graphicsASI);


    graphicsASI->setParent(view);

    graphicsASI->setFixedHeight(200);
    graphicsASI->setFixedWidth(200);

    graphicsASI->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsASI->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //graphicsASI->setGeometry(0,815,200,200);

    graphicsASI->setGeometry(10, view->height() - graphicsASI->height() - 80, graphicsASI->width(), graphicsASI->height());

    graphicsASI->setStyleSheet("background-color: rgba(255, 255, 255,0);");





    //raphicsASI->setAirspeed(100);

    //graphicsASI->redraw();

   // graphicsASI->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);










    QPixmap tankPixmap("C:/Users/murat/OneDrive/Belgeler/gamepaddeneme/tank4.png");
    QPixmap targetpixmap("C:/Users/murat/OneDrive/Belgeler/best-png-target-11.png");
    QPixmap scaledTankPixmap = tankPixmap.scaled(tankPixmap.width() / 15, tankPixmap.height() / 15, Qt::KeepAspectRatio, Qt::SmoothTransformation);
     QPixmap scaledtargetPixmap = targetpixmap.scaled(targetpixmap.width() / 15, targetpixmap.height() / 15, Qt::KeepAspectRatio, Qt::SmoothTransformation);
     tank1 = new QGraphicsPixmapItem(scaledTankPixmap);

    //tank1 = scene->addPixmap(scaledTankPixmap);
     scene->addItem(tank1);
    tank1->setPos(800,700);

     label2 = scene->addRect(0,0,100,20);
    label2->setPos(-100,10);




    for (int i = 0; i < 3; ++i) {
        QGraphicsPixmapItem *target = scene->addPixmap(scaledtargetPixmap);
        target->setPos(QRandomGenerator::global()->bounded(width()+1000),
                       QRandomGenerator::global()->bounded(height()+1500));
        //target->setBrush(Qt::blue);
        targets.append(target);
    }



    //scene = new QGraphicsScene(this);
    //QGraphicsView *view = new QGraphicsView(scene, this);
    //QPixmap pix("C:/Users/murat/OneDrive/Belgeler/Başlıksız2.png");
    //QPixmap pix2("C:/Users/murat/OneDrive/Belgeler/best-png-target-11.png");

    //ui->tank->setPixmap(pix);

    //ui->target1->setPixmap(pix2);

    //ui->target2->setPixmap(pix2);

    //ui->target3->setPixmap(pix2);




     //target4 = new QGraphicsPixmapItem(pix2);
     //target5 = new QGraphicsPixmapItem(pix2);
     //target6 = new QGraphicsPixmapItem(pix2);

     //scene->addItem(target4);
     //scene->addItem(target5);
     //scene->addItem(target6);

     //target4->setPos(100, 100);
     //target5->setPos(300, 100);
     //target6->setPos(500, 100);

   // ui->tank->raise();
     //scene = new QGraphicsScene(this);

    this->setFocusPolicy(Qt::StrongFocus);
    this->setFocus();

    //ui->label;
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::handleGamepadInput);
    timer->start(16);
       bulletTimer = new QTimer(this);
       connect(bulletTimer, &QTimer::timeout, this, &MainWindow::update);
       bulletTimer->start(1000);

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
    std::cout<<"joystick connected"<< std::endl;
    //bool running = true;






   // SDL_GameControllerClose(gamepad);
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
            qDebug() << " tuşa basıldı";
            break;

        case Qt::Key_Right: keyboardMoveX = 1;
            qDebug() << " tuşa basıldı";
            break;
        case Qt::Key_Up:    keyboardMoveY = +1;
            qDebug() << " tuşa basıldı";
            break;
        case Qt::Key_Down:  keyboardMoveY = -1;
qDebug() << " tuşa basıldı";
            break;
        case Qt::Key_A:     currentAngle -= 5; break;
        case Qt::Key_D:     currentAngle += 5; break;
        case Qt::Key_Space: fireBullet();
         qDebug() << " tuşa basıldı";
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
        //QPixmap pix("C:/Users/murat/OneDrive/Belgeler/Başlıksız2.png");

         //ui->tank->setPixmap(pix);
        handleGamepadInput();
        moveObject();
        rotateObject();

        updateBullets();

    }

    void MainWindow::handleGamepadInput()
    {
        //QPointF position = tank1->pos();

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                QApplication::quit();
            } else if (e.type == SDL_CONTROLLERBUTTONDOWN){
                if (e.cbutton.button == SDL_CONTROLLER_BUTTON_A) {
                    currentAngle +=5;
                    std::cout << "Press A" << std::endl;



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
        //QPoint position = ui->tank->pos();
        QPointF position = tank1->pos();




        // Gamepad hareketi

        if (abs(leftStickX) > 0) {
            if(leftStickX<40){
                currentAngle=currentAngle + (leftStickX/15000);
                //leftStickX=40;

            }else if(leftStickX>40){
                currentAngle=currentAngle + (leftStickX/15000);
                //leftStickX=40;
            }

        }
        if (abs(leftStickY) > 0) {
           //position.setY(position.y() + leftStickY / 3200);
            position.setX(position.x() - qSin(qDegreesToRadians(currentAngle))*(leftStickY / 3200));
            position.setY(position.y() + qCos(qDegreesToRadians(currentAngle))* (leftStickY / 3200));
            c =(leftStickY / 2500)*3.2;
            graphicsASI->setAirspeed(abs(c));
           graphicsASI->redraw();

        }

        // Klavye hareketi
        position.setX(position.x() + qSin(qDegreesToRadians(currentAngle))*keyboardMoveY);
        position.setY(position.y() - qCos(qDegreesToRadians(currentAngle))* keyboardMoveY);

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
        else if (keyboardMoveY == 0 && leftStickY == 0){
            graphicsASI->setAirspeed(0);
        }

        //graphicsASI->setAirspeed(-(qCos(qDegreesToRadians(currentAngle))* keyboardMoveY*40)+(qSin(qDegreesToRadians(currentAngle))*keyboardMoveY*40));
        graphicsASI->redraw();




        //ui->tank->move(position);

        tank1->setPos(position);
        view->centerOn(position);
        //QPointF position2 = view->pos();
        //graphicsASI->centerOn(position2);
        //graphicsASI->(10, view->height() - graphicsASI->height() - 10,200,200);
       // graphicsASI->move(10, view->height() - graphicsASI->height() - 10);
        qDebug() << "Tank position:" << position;
    }
    void MainWindow::rotateObject()
    {
        QPointF positionrect = label1->pos();


        QPointF position = tank1->pos();
        QPixmap originalPixmap("C:/Users/murat/OneDrive/Belgeler/gamepaddeneme/tank4.png");
        QPixmap scaledTankPixmap = originalPixmap.scaled(originalPixmap.width() / 15, originalPixmap.height() / 15, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        //QTransform transform;



        QPointF center = QPointF((scaledTankPixmap.width()) / 2.0, scaledTankPixmap.height()-30 / 2.0);
        //transform.translate(center.x(), center.y());
       // transform.rotate(currentAngle);
        //transform.translate(-center.x(), -center.y());


       // transform.rotate(currentAngle);
        label1->setRotation(currentAngle);

        //QPixmap rotatedPixmap = originalPixmap.transformed(transform, Qt::SmoothTransformation);

        //QPixmap rotatedscaledTankPixmap = rotatedPixmap.scaled(rotatedPixmap.width() / 20, rotatedPixmap.height() / 20, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        //scene->removeItem(tank1);
        //delete tank1;
        //tank1 = scene->addPixmap(rotatedscaledTankPixmap);
        tank1->setTransformOriginPoint(center);
        tank1->setRotation(currentAngle);
        tank1->setPos(position);
        //tank1->setTransformOriginPoint(center);



        label1 = scene->addRect(0,0,60,30);
        label1->setPos(positionrect);

       //graphicsASI->setAirspeed(45);

       //graphicsASI->redraw();



        //ui->tank->setPixmap(rotatedPixmap);
    }

    void MainWindow::fireBullet() {
        static QGraphicsTextItem *statusText = nullptr;

        if(b>0){
        QPointF tankPos = tank1->pos();
        //Bullet *bullet = new Bullet(tankPos.x() + ui->tank->width() / 2,
          //                          tankPos.y() + ui->tank->height() / 2,
            //                        currentAngle);
        QRectF tankRect = tank1->boundingRect();
        Bullet *bullet = new Bullet(tankPos.x() + tankRect.width() / 2,
                                    tankPos.y() + tankRect.height() / 2,
                                    currentAngle);
        scene->addItem(bullet);
        bullets.append(bullet);



        b--;


        QString bstring = QString::number(b);
        if (statusText == nullptr) {
            statusText = scene->addText("remaining bullet:"+bstring);
            statusText->setDefaultTextColor(Qt::red);
            statusText->setPos(500, 0);
        } else {
            statusText->setPlainText("remaining bullet:"+bstring);
            statusText->setDefaultTextColor(Qt::red);
        }
        }
        else{
            if (statusText == nullptr) {
                statusText = scene->addText("empty");
                statusText->setDefaultTextColor(Qt::red);
                statusText->setPos(500, 0);
            } else {
                statusText->setPlainText("empty");
                statusText->setDefaultTextColor(Qt::red);
            }

        }



    }
    void MainWindow::updateBullets() {

       // QGraphicsTextItem *textItem = nullptr;
        static QGraphicsTextItem *statusText = nullptr;
        static QGraphicsTextItem *statusText2 = nullptr;
        static QGraphicsTextItem *statusText3 = nullptr;




        for (auto it = bullets.begin(); it != bullets.end();) {
            Bullet *bullet = *it;
            bullet->advance();
            //QPointF bulletpos = bullet->pos();
             bool shouldDelete = bullet->shouldDelete;
             int bulletCount = bullets.size();
             QString bstring = QString::number(bulletCount);
             statusText2 = scene->addText("atılan bullet:" +bstring);
             statusText2->setPos(70, 10);




            bool collision = false;
            for (QGraphicsPixmapItem *target : targets) {
                if (bullet->collidesWithItem(target)) {
                    scene->removeItem(target);
                    targets.removeOne(target);
                    delete target;
                    collision = true;
                    qDebug() << "Hedef vuruldu!";

                    if (statusText == nullptr) {
                        statusText = scene->addText("Hedef vuruldu!");
                        statusText->setDefaultTextColor(Qt::red);
                        statusText->setPos(10, 10); // Sol üst köşeye yakın bir konum
                    } else {
                        statusText->setPlainText("Hedef vuruldu!");
                        statusText->setDefaultTextColor(Qt::red);
                    }



                   // label1-> scene()->addText("hedef vuruldu");


                   // QGraphicsTextItem *text = scene->addText("Hedef vuruldu!");
                    //text->setDefaultTextColor(Qt::red);
                    //text->setPos(tank1->pos());
                    //ui->label->setText("Hedef vuruldu");


                    break;
                }
            }


            if (collision || shouldDelete) {
                scene->removeItem(bullet);
                qDebug() << "bullet deleted";
                it = bullets.erase(it);
                delete bullet;
                int bulletCount1 = bullets.size();
                QString bstring2 = QString::number(bulletCount1);


                statusText3 = scene->addText("remaining bullet:" +bstring2);
                 statusText3->setPos(200, 10);

                 //qDebug() << "bullet deleted";



            } else {
                ++it;

            }
        }
        if(text1==false){
        if (targets.isEmpty()) {
            qDebug() << "Tüm hedefler vuruldu!";
            statusText->setPlainText("Tüm hedefler vuruldu!");
            statusText->setDefaultTextColor(Qt::green);
            //if (textItem != nullptr) {
              //  if (scene->items().contains(textItem)) {
                //    scene->removeItem(textItem);
                //}
                //delete textItem;
                //textItem = nullptr;
            //}

            //textItem = scene->addText("Tüm hedefler vuruldu!");
            //textItem->setDefaultTextColor(Qt::green);
           // textItem->setPos(0,0);


            //label1-> scene()->addText("tüm hedefler vuruldu");
            //QGraphicsTextItem *text = scene->addText("Tüm hedefler vuruldu!");

            //text->setDefaultTextColor(Qt::green);
            //text->setPos(0,0);
            // ui->label->setText("tüm hedefler vuruldu");

            text1=true;



        }
        }
    }




