#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "bullet.h"

#include "tank.h"

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
#include <qfi/qfi_ASI.h>
//#include <QWebEngineView>
#include <QGeoServiceProvider>
#include <QGeoCoordinate>
#include <QQuickView>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickWidget>
#include <QPointF>
#include <QLineEdit>
#include <QVBoxLayout>








QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


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
    QPointF findNearestTarget();
    void updateTankAI();
    void startTankAI();
    void stopTankAI();
    //void moveTowardsTarget(QPointF targetPos);
    void initializeTank();
    // void entercoordinate();
    //void gocoordinate();
    void setupCoordWidget();
    void showCoordWidget();
    void hideCoordWidget();
    void goToCoordinates(int x, int y);

    //void on_graphicsEADI_rubberBandChanged(const QRect &rect, const QPointF &from, const QPointF &to);

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
    // QGraphicsPixmapItem *tank1 =nullptr;
    QGraphicsRectItem *label1;
    QGraphicsRectItem *label2;
    qfi_ASI  *graphicsASI;
    // QQmlApplicationEngine *engine;
    //  QQuickItem *mapItem;
    QQuickWidget *mapWidget;
    QWidget *coordWidget;
    //QLineEdit *xCoordInput;
    //QLineEdit *yCoordInput;

    //QLabel *target1;

    // bool isKeyboardControlActive;
    int keyboardMoveX;
    int keyboardMoveY;
    float currentAngle;
    //float targetAngle;
    bool text1;
    int b;
    float c;
    float d;
    bool shoot;
    bool isAligned;
    //QPointF defaultPosition;
    bool deneme;
    bool deneme2;
    bool deneme3;
    tank *tank2;

    //int a;

    int leftStickX;
    int leftStickY;
    QList<Bullet*> bullets;
    QList<QWidget*> Bullets;
    QList<QWidget*> Targets;
    QTimer *bulletTimer;
    QTimer *tankAITimer = nullptr;
    QPointF tankFrontOffset;
    QPointF coordinate;
    QWidget* bullet_widget;
    QWidget* target_tank_widget;

    QLabel* bullet_label;
    QLabel* target_tank_label;

    QVBoxLayout *bullet_layout;
    QVBoxLayout *target_tank_layout;

};
#endif // MAINWINDOW_H
