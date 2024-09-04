#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "bullet.h"

#include <SDL.h>
#include <qlabel.h>
#include <qfi/qfi_ASI.h>

#include <iostream>
#include <QMainWindow>
#include <QKeyEvent>
#include <QImage>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QRandomGenerator>
#include <QList>
#include <QtMath>
#include <QGeoServiceProvider>
#include <QGeoCoordinate>
#include <QQuickView>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickWidget>

// #include <QtLocation>
// #include <QGeoPositionInfoSource>
// #include <QGraphicsProxyWidget>
// #include <stdio.h>
// #include <QBrush>
// #include <QVBoxLayout>
//#include <QGeoMapType>
// #include <QVBoxLayout>


QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

    private slots:
        void move_object();
        void update();
        void rotate_object();
        void key_release_event(QKeyEvent *event);
        void key_press_event(QKeyEvent *event);
        void handle_gamepad_input();
        void fire_bullet();
        void update_bullets();

    private:
        Ui::MainWindow* ui;

        SDL_GameController* gamepad;

        QPalette palette;

        QTimer* gamepad_timer;
        QTimer* bullet_timer;
        QTimer* sdl_timer;

        QPixmap background_pixmap;
        QPixmap tank_pixmap;
        QPixmap target_pixmap;

        QGraphicsScene* scene;

        QGraphicsView* view;

        QList<QGraphicsPixmapItem*> targets;
        QList<Bullet*> bullets;

        QGraphicsPixmapItem* tank_pixmap_item;
        QGraphicsPixmapItem* target_pixmap_item;

        QGraphicsRectItem* label2;

        qfi_ASI* graphics_ASI;

        QQmlApplicationEngine* engine;

        QQuickItem* map_quick_item;

        QQuickWidget* map_quick_widget;

        bool is_keyboard_control_active;
        bool hit_control;

        int amount_of_ammo;
        int keyboard_move_X;
        int keyboard_move_Y;
        int left_stick_X;
        int left_stick_Y;

        float current_angle;
        float tank_speed;
};

#endif // MAINWINDOW_H
