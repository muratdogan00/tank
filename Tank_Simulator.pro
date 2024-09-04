QT       += core gui location positioning quick quickwidgets svgwidgets

# SDL2_PATH = C:\Users\alperen\Documents\Tank\SDL2-2.30.7

# INCLUDEPATH += C:\Users\alperen\Documents\Tank\SDL2-2.30.7\include
# LIBS += -LC:\Users\alperen\Documents\Tank\SDL2-2.30.7\lib\x64 -lSDL2

INCLUDEPATH += C:\Users\alperen\Documents\Tank_Simulator\include
LIBS += -LC:\Users\alperen\Documents\Tank_Simulator\lib -lSDL2

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bullet.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    bullet.h \
    mainwindow.h

FORMS += \
    mainwindow.ui
    include(qfi/qfi.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES += \
    map.qml
