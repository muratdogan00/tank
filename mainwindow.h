#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <SDL.h>
//#include <iostream>
#include <qlabel.h>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QRandomGenerator>
#include <QList>
#include <QGraphicsEllipseItem>
#include <QtMath>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
class Bullet : public QGraphicsEllipseItem {
public:
    Bullet(qreal x, qreal y, qreal angle);
    void advance();
    bool shouldDelete = false;

private:
    qreal angle_rad;
    qreal speed;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //void checkSDLEvents();
    void moveObject();
    //void handleKeyboardEvents(SDL_Event &e);
    void update();
    void rotateObject();
    void keyReleaseEvent(QKeyEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void handleGamepadInput();

    void fireBullet();
    void updateBullets();

private:
    Ui::MainWindow *ui;
    SDL_GameController *gamepad;
    QTimer *sdlTimer;
   // QLabel *tank;

    QGraphicsScene *scene;
    QGraphicsView *view;

    QList<QGraphicsPixmapItem*> targets;
    //QGraphicsPixmapItem *target4;
    //QGraphicsPixmapItem *target5;
    //QGraphicsPixmapItem *target6;
    QGraphicsPixmapItem *tank1 =nullptr;
    QGraphicsRectItem *label1;
     QGraphicsRectItem *label2;

    //QLabel *target1;

    bool isKeyboardControlActive;
    int keyboardMoveX;
    int keyboardMoveY;
    float currentAngle;
    //float targetAngle;
    bool text1;
    int b;

    //int a;

    int leftStickX;
    int leftStickY;
    QList<Bullet*> bullets; // Mermileri saklayan liste
    QTimer *bulletTimer;
};
#endif // MAINWINDOW_H
