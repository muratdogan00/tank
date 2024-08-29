/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *object;
    QLabel *tank;
    QLabel *target2;
    QLabel *target3;
    QLabel *target1;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        object = new QPushButton(centralwidget);
        object->setObjectName("object");
        object->setGeometry(QRect(340, 390, 41, 41));
        object->setTabletTracking(false);
        object->setAutoFillBackground(false);
        object->setStyleSheet(QString::fromUtf8("background-color: rgb(160, 255, 231);\n"
"color: rgb(192, 219, 255);"));
        QIcon icon;
        icon.addFile(QString::fromUtf8("../Ba\305\237l\304\261ks\304\261z2.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        object->setIcon(icon);
        object->setIconSize(QSize(64, 64));
        tank = new QLabel(centralwidget);
        tank->setObjectName("tank");
        tank->setEnabled(true);
        tank->setGeometry(QRect(400, 360, 61, 41));
        tank->setAutoFillBackground(false);
        tank->setPixmap(QPixmap(QString::fromUtf8("../Ba\305\237l\304\261ks\304\261z2.png")));
        tank->setScaledContents(true);
        tank->setWordWrap(true);
        target2 = new QLabel(centralwidget);
        target2->setObjectName("target2");
        target2->setGeometry(QRect(350, 80, 71, 51));
        target2->setPixmap(QPixmap(QString::fromUtf8("../best-png-target-11.png")));
        target2->setScaledContents(true);
        target3 = new QLabel(centralwidget);
        target3->setObjectName("target3");
        target3->setGeometry(QRect(550, 80, 71, 51));
        target3->setPixmap(QPixmap(QString::fromUtf8("../best-png-target-11.png")));
        target3->setScaledContents(true);
        target1 = new QLabel(centralwidget);
        target1->setObjectName("target1");
        target1->setGeometry(QRect(140, 80, 71, 51));
        target1->setPixmap(QPixmap(QString::fromUtf8("../best-png-target-11.png")));
        target1->setScaledContents(true);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        object->setText(QString());
        tank->setText(QString());
        target2->setText(QString());
        target3->setText(QString());
        target1->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
