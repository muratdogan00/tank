

#include "mainwindow.h"
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <iostream>

#undef main




#include <QApplication>

int main(int argc, char *argv[])
{
    //SDL_GameController *gamepad =nullptr;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();


}
