#include "ui_mainwindow.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
                                        , ui(new Ui::MainWindow)
                                        , gamepad(nullptr)
                                        , map_pixmap(":\\images\\map.png")
                                        , map2_pixmap(":\\images\\map2.png")
                                        , map3_pixmap(":\\images\\map3.png")
                                        , map4_pixmap(":\\images\\map4.png")
                                        , tank_pixmap(":images\\tank.png")
                                        // , tank2_pixmap(":images\\tank2.png")
                                        // , tank3_pixmap(":images\\tank3.png")
                                        , target_tank_pixmap(":\\images\\target_tank.png")
                                        , bullet_pixmap(":\\images\\bullet.png")
                                        , tank_pixmap_item(nullptr)
                                        , is_keyboard_control_active(false)
                                        , hit_control(false)
                                        , amount_of_bullet(15)
                                        , keyboard_move_X(0)
                                        , keyboard_move_Y(0)
                                        , left_stick_X(0)
                                        , left_stick_Y(0)
                                        , current_angle(0)
                                        , tank_speed(0)
{
    ui->setupUi(this);

    // background_pixmap = background_pixmap.scaled(this->size(), Qt::IgnoreAspectRatio);
    tank_pixmap = tank_pixmap.scaled(tank_pixmap.width() / 25, tank_pixmap.height() / 25, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    target_tank_pixmap = target_tank_pixmap.scaled(target_tank_pixmap.width() / 10, target_tank_pixmap.height() / 10, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    bullet_pixmap = bullet_pixmap.scaled(106, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // palette.setBrush(QPalette::Window, background_pixmap);

    this->showMaximized();
    this->setFocusPolicy(Qt::StrongFocus);
    this->setFocus();
    // this->setPalette(palette);

    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 5000, 5000);
    scene->setBackgroundBrush(QBrush(map_pixmap));

    tank_pixmap_item = new QGraphicsPixmapItem(tank_pixmap);
    tank_pixmap_item->setPos(scene->width() / 2 - 50, scene->height() / 2 - 25);

    scene->addItem(tank_pixmap_item);

    view = new QGraphicsView(scene, this);
    view->setFixedSize(1920, 1080);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setCentralWidget(view);

    graphics_ASI = new qfi_ASI;
    graphics_ASI->setParent(view);
    graphics_ASI->setGeometry(10, view->height() - graphics_ASI->height() + 200, graphics_ASI->width(), graphics_ASI->height());
    graphics_ASI->setStyleSheet("background-color: rgba(255, 255, 255,0);");
    graphics_ASI->setFixedHeight(200);
    graphics_ASI->setFixedWidth(200);
    graphics_ASI->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphics_ASI->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scene->addWidget(graphics_ASI);

    for (size_t i = 0; i < 3; ++i)
    {
        target_pixmap_item = new QGraphicsPixmapItem(target_tank_pixmap);
        target_pixmap_item->setPos(QRandomGenerator::global()->bounded(width() + 3000), QRandomGenerator::global()->bounded(height()) + 3000);

        scene->addItem(target_pixmap_item);

        targets.append(target_pixmap_item);
    }

    for (size_t i = 0; i < 15; ++i)
    {
        bullet_widget = new QWidget;

        bullet_label = new QLabel(bullet_widget);
        bullet_label->setPixmap(bullet_pixmap);
        bullet_label->setFixedSize(bullet_pixmap.size());

        bullet_layout = new QVBoxLayout(bullet_label);
        bullet_layout->addWidget(bullet_label);

        bullet_widget->setLayout(bullet_layout);
        bullet_widget->setParent(view);
        bullet_widget->setGeometry(1800, view->height() - bullet_widget->height() + i * 25, bullet_widget->width(), bullet_widget->height());

        Bullets.append(bullet_widget);
    }

    for (size_t i = 0; i < 3; ++i)
    {
        target_tank_widget = new QWidget;

        target_tank_label = new QLabel(target_tank_widget);
        target_tank_label->setPixmap(target_tank_pixmap);
        target_tank_label->setFixedSize(target_tank_pixmap.size());

        target_tank_layout = new QVBoxLayout(target_tank_label);
        target_tank_layout->addWidget(target_tank_label);

        target_tank_widget->setLayout(target_tank_layout);
        target_tank_widget->setParent(view);
        target_tank_widget->setGeometry(1600 + (i * 40), view->height() - target_tank_widget->height() + 300, target_tank_widget->width(), target_tank_widget->height());

        Targets.append(target_tank_widget);
    }

    gamepad_timer = new QTimer(this);
    connect(gamepad_timer, &QTimer::timeout, this, &MainWindow::handle_gamepad_input);
    gamepad_timer->start(16);

    bullet_timer = new QTimer(this);
    connect(bullet_timer, &QTimer::timeout, this, &MainWindow::update);
    bullet_timer->start(1000);

    sdl_timer = new QTimer(this);
    connect(sdl_timer, &QTimer::timeout, this, &MainWindow::update);
    sdl_timer->start(16);

    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK|SDL_INIT_GAMECONTROLLER) != 0) std::cerr << "sdl does not exist: " << SDL_GetError() << std::endl;
    else
    {
        for (size_t i = 0; i < static_cast<size_t>(SDL_NumJoysticks()); ++i)
        {
            if (SDL_IsGameController(i))
            {
                gamepad = SDL_GameControllerOpen(i);

                if (gamepad)
                {
                    std::cout << "Gamepad connected: " << SDL_GameControllerName(gamepad) << std::endl;

                    return;
                }
                else std::cerr << "Gamepad could not open " << i << ": " << SDL_GetError() << std::endl;
            }
        }
    }

    std::cout << "Gamepad is connected" << std::endl;
}

MainWindow::~MainWindow()
{
    if (gamepad) SDL_GameControllerClose(gamepad);

    SDL_Quit();

    delete ui;
}

void MainWindow::change_map(const QPixmap& new_map)
{
    scene->setBackgroundBrush(QBrush(new_map));
}

void MainWindow::change_tank(const QPixmap& new_tank)
{
    tank_pixmap_item->setPixmap(new_tank);
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    switch(event->key())
    {
        case Qt::Key_Up:
            keyboard_move_Y = +3;
            qDebug() << "Up is pressed";
            break;
        case Qt::Key_Down:
            keyboard_move_Y = -3;
            qDebug() << "Down is pressed";
            break;
        case Qt::Key_Left:
            current_angle -= 5;
            qDebug() << "Left is pressed";
            break;
        case Qt::Key_Right:
            current_angle += 5;
            qDebug() << "Right is pressed";
            break;
        case Qt::Key_Space:
            fire_bullet();
            if (Bullets.isEmpty() == false) Bullets.takeFirst()->hide();
            qDebug() << "Space is pressed";
            break;
        case Qt::Key_1:
            change_map(map_pixmap);
            qDebug() << "Background changed to map_pixmap";
            break;
        case Qt::Key_2:
            change_map(map2_pixmap);
            qDebug() << "Background changed to map2_pixmap";
            break;
        case Qt::Key_3:
            change_map(map3_pixmap);
            qDebug() << "Background changed to map3_pixmap";
            break;
        case Qt::Key_4:
            change_map(map4_pixmap);
            qDebug() << "Background changed to map4_pixmap";
            break;
        case Qt::Key_7:
            change_tank(tank_pixmap);
            qDebug() << "Background changed to map_pixmap";
            break;
        // case Qt::Key_8:
        //     change_tank(tank2_pixmap);
        //     qDebug() << "Background changed to map2_pixmap";
        //     break;
        // case Qt::Key_9:
        //     change_tank(tank3_pixmap);
        //     qDebug() << "Background changed to map3_pixmap";
        //     break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    switch(event->key())
    {
        case Qt::Key_Left:
        case Qt::Key_Right:
            keyboard_move_X = 0;
            break;
        case Qt::Key_Up:
        case Qt::Key_Down:
            keyboard_move_Y = 0;
            break;
    }
}

void MainWindow::update()
{
    handle_gamepad_input();

    move_object();

    rotate_object();

    update_bullets();
}

void MainWindow::handle_gamepad_input()
{
    SDL_Event gamepad_input;

    while (SDL_PollEvent(&gamepad_input))
    {
        if (gamepad_input.type == SDL_QUIT) QApplication::quit();
        else if (gamepad_input.type == SDL_CONTROLLERBUTTONDOWN)
        {
            if (gamepad_input.cbutton.button == SDL_CONTROLLER_BUTTON_A)
            {
                current_angle += 1;

                std::cout << "A is pressed" << std::endl;
            }
            else if (gamepad_input.cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
            {
                fire_bullet();

                std::cout << "R1 is pressed" << std::endl;
            }
        }
        else if (gamepad_input.type == SDL_CONTROLLERAXISMOTION)
        {
            if (gamepad_input.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX)
            {
                if (gamepad_input.caxis.value < 0)
                {
                    left_stick_X = gamepad_input.caxis.value;

                    std::cout << "Left" << std::endl;
                }
                else if (gamepad_input.caxis.value > 0)
                {
                    left_stick_X = gamepad_input.caxis.value;

                    std::cout << "Right" << std::endl;
                }
            }
            else if (gamepad_input.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
            {
                if (gamepad_input.caxis.value < 0)
                {
                    left_stick_Y = gamepad_input.caxis.value;

                    std::cout << "Up" << std::endl;
                }
                else if (gamepad_input.caxis.value > 0)
                {
                    left_stick_Y = gamepad_input.caxis.value;

                    std::cout << "Down" << std::endl;
                }
            }
            else if (gamepad_input.cbutton.button == SDL_CONTROLLER_AXIS_TRIGGERRIGHT)
            {
                fire_bullet();

                std::cout << "R2 is pressed" << std::endl;
            }
        }
    }
}

void MainWindow::move_object()
{
    QPointF tank_position = tank_pixmap_item->pos();

    if (abs(left_stick_X) > 0)
    {
        if (left_stick_X < 40) current_angle = current_angle + (left_stick_X / 15000);
        else if (left_stick_X > 40) current_angle = current_angle + (left_stick_X / 15000);
    }
    if (abs(left_stick_Y) > 0)
    {
        tank_position.setX(tank_position.x() - qSin(qDegreesToRadians(current_angle)) * (left_stick_Y / 9600));
        tank_position.setY(tank_position.y() + qCos(qDegreesToRadians(current_angle)) * (left_stick_Y / 9600));
    }

    tank_position.setX(tank_position.x() + qSin(qDegreesToRadians(current_angle)) * keyboard_move_Y);
    tank_position.setY(tank_position.y() - qCos(qDegreesToRadians(current_angle)) * keyboard_move_Y);

    tank_speed = 5 * (left_stick_Y / 2500);

    graphics_ASI->setAirspeed(abs(tank_speed));
    graphics_ASI->redraw();

    tank_position.setX(tank_position.x() + qSin(qDegreesToRadians(current_angle)) * keyboard_move_Y);
    tank_position.setY(tank_position.y() - qCos(qDegreesToRadians(current_angle)) * keyboard_move_Y);

    if((qSin(qDegreesToRadians(current_angle)) > 0) && (qCos(qDegreesToRadians(current_angle)) > 0) && (keyboard_move_Y == 1))
        graphics_ASI->setAirspeed((qCos(qDegreesToRadians(current_angle)) * keyboard_move_Y * 40) + (qSin(qDegreesToRadians(current_angle)) * keyboard_move_Y * 40));
    else if((qSin(qDegreesToRadians(current_angle)) > 0) && ((qCos(qDegreesToRadians(current_angle)) < 0) && (keyboard_move_Y == 1)))
        graphics_ASI->setAirspeed(-(qCos(qDegreesToRadians(current_angle)) * keyboard_move_Y * 40) + (qSin(qDegreesToRadians(current_angle)) * keyboard_move_Y * 40));
    else if((qSin(qDegreesToRadians(current_angle)) < 0) && (qCos(qDegreesToRadians(current_angle)) < 0) && (keyboard_move_Y == 1))
        graphics_ASI->setAirspeed(-(qCos(qDegreesToRadians(current_angle)) * keyboard_move_Y * 40) - (qSin(qDegreesToRadians(current_angle)) * keyboard_move_Y * 40));
    else if(keyboard_move_Y == 1)
        graphics_ASI->setAirspeed((qCos(qDegreesToRadians(current_angle)) * keyboard_move_Y * 40) - (qSin(qDegreesToRadians(current_angle)) * keyboard_move_Y * 40));
    else if ((keyboard_move_Y == 0) && (left_stick_Y == 0))
        graphics_ASI->setAirspeed(0);

    graphics_ASI->redraw();

    tank_pixmap_item->setPos(tank_position);

    view->centerOn(tank_position);

    // QPointF view_position = view->pos();

    // graphics_ASI->centerOn(view_position);

    qDebug() << "Tank position: " << tank_position;
}

void MainWindow::rotate_object()
{
    QPointF tank_position = tank_pixmap_item->pos();

    tank_pixmap_item->setTransformOriginPoint((tank_pixmap.width() / 2.0), (tank_pixmap.height() - 30 / 2.0));
    tank_pixmap_item->setRotation(current_angle);
    tank_pixmap_item->setPos(tank_position);
}

void MainWindow::fire_bullet()
{
    static QGraphicsTextItem* ammo_status = nullptr;

    if (amount_of_bullet > 0)
    {
        QPointF tank_position = tank_pixmap_item->pos();

        QRectF tank_rectangle = tank_pixmap_item->boundingRect();

        Bullet* bullet = new Bullet(tank_position.x() + tank_rectangle.width() / 2, tank_position.y() + tank_rectangle.height() / 2, current_angle);

        scene->addItem(bullet);

        bullets.append(bullet);

        amount_of_bullet--;

        if (ammo_status == nullptr)
        {
            ammo_status = scene->addText("Remaining bullet: " + QString::number(amount_of_bullet));
            ammo_status->setDefaultTextColor(Qt::black);
            ammo_status->setPos(500, 0);
        }
        else
        {
            ammo_status->setPlainText("Remaining bullet: " + QString::number(amount_of_bullet));
            ammo_status->setDefaultTextColor(Qt::black);
        }
    }
    else
    {
        if (ammo_status == nullptr)
        {
            ammo_status = scene->addText("Out of ammo");
            ammo_status->setDefaultTextColor(Qt::red);
            ammo_status->setPos(500, 0);
        }
        else
        {
            ammo_status->setPlainText("Out of ammo");
            ammo_status->setDefaultTextColor(Qt::red);
        }
    }
}

void MainWindow::update_bullets()
{
    static QGraphicsTextItem* statusText = nullptr;
    static QGraphicsTextItem* statusText2 = nullptr;
    static QGraphicsTextItem* statusText3 = nullptr;

    for (auto it = bullets.begin(); it != bullets.end();)
    {
        Bullet* bullet = *it;

        bullet->advance();

        bool shouldDelete = bullet->shouldDelete;

        int bulletCount = bullets.size();

        statusText2 = scene->addText("atılan bullet: " + QString::number(bulletCount));
        statusText2->setPos(150, 10);

        bool collision = false;

        for (QGraphicsPixmapItem* target : targets)
        {
            if (bullet->collidesWithItem(target))
            {
                scene->removeItem(target);

                targets.removeOne(target);

                delete target;

                Targets.takeFirst()->hide();

                collision = true;

                qDebug() << "Target Hit!";

                if (statusText == nullptr)
                {
                    statusText = scene->addText("Target Hit!");
                    statusText->setDefaultTextColor(Qt::red);
                    statusText->setPos(10, 10);
                }
                else
                {
                    statusText->setPlainText("Target Hit!");
                    statusText->setDefaultTextColor(Qt::red);
                }

                break;
            }
        }

        if (collision == true || shouldDelete == true)
        {
            scene->removeItem(bullet);

            qDebug() << "Bullet is deleted";

            it = bullets.erase(it);

            delete bullet;

            int bulletCount1 = bullets.size();

            statusText3 = scene->addText("Remaining bullet:" + QString::number(bulletCount1));
            statusText3->setPos(250, 10);
        }
        else ++it;
    }

    if (hit_control == false)
    {
        if (targets.isEmpty() == true)
        {
            qDebug() << "All targets destroyed!";

            statusText->setPlainText("All targets destroyed!");
            statusText->setDefaultTextColor(Qt::green);

            hit_control = true;
        }
    }
}




