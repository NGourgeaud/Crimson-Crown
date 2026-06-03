QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    appcontroller.cpp \
    blacksmithmodel.cpp \
    blacksmithview.cpp \
    bossmodel.cpp \
    charselectcontroller.cpp \
    charselectview.cpp \
    enemymodel.cpp \
    enemyview.cpp \
    entitymodel.cpp \
    gamecontroller.cpp \
    gamemodel.cpp \
    lordmodel.cpp \
    gameview.cpp \
    main.cpp \
    mainwindow.cpp \
    menucontroller.cpp \
    menuview.cpp \
    playermodel.cpp \
    lancermodel.cpp \
    obstaclemodel.cpp \
    obstacleview.cpp \
    playerview.cpp \
    roommodel.cpp \
    roomview.cpp

HEADERS += \
    appcontroller.h \
    blacksmithmodel.h \
    blacksmithview.h \
    bossmodel.h \
    charselectcontroller.h \
    charselectview.h \
    config.h \
    enemymodel.h \
    enemyview.h \
    entitymodel.h \
    gamecontroller.h \
    gamemodel.h \
    lordmodel.h \
    gameview.h \
    mainwindow.h \
    menucontroller.h \
    menuview.h \
    playermodel.h \
    lancermodel.h \
    obstaclemodel.h \
    obstacleview.h \
    playerview.h \
    roommodel.h \
    roomview.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc

macx:QMAKE_CXXFLAGS += -Wno-error=implicit-function-declaration
macx:QMAKE_CFLAGS += -Wno-error=implicit-function-declaration