QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    appcontroller.cpp \
    archermodel.cpp \
    assassinmodel.cpp \
    blacksmithmodel.cpp \
    blacksmithview.cpp \
    bossmodel.cpp \
    bossview.cpp \
    charselectcontroller.cpp \
    charselectview.cpp \
    collectibleview.cpp \
    enemymodel.cpp \
    enemyview.cpp \
    entitymodel.cpp \
    gamecontroller.cpp \
    gamemodel.cpp \
    gameovercontroller.cpp \
    gameoverview.cpp \
    gamewoncontroller.cpp \
    gamewonview.cpp \
    goldmodel.cpp \
    hitzonemodel.cpp \
    hitzoneview.cpp \
    huntermodel.cpp \
    itemmodel.cpp \
    itemview.cpp \
    kingmodel.cpp \
    knightmodel.cpp \
    leaderboardcontroller.cpp \
    leaderboardmodel.cpp \
    leaderboardview.cpp \
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
    projectilemodel.cpp \
    projectileview.cpp \
    roommodel.cpp \
    roomview.cpp \
    settingmodel.cpp \
    settingview.cpp \
    settingcontroller.cpp

HEADERS += \
    appcontroller.h \
    archermodel.h \
    assassinmodel.h \
    blacksmithmodel.h \
    blacksmithview.h \
    bossmodel.h \
    bossview.h \
    charselectcontroller.h \
    charselectview.h \
    collectibleview.h \
    config.h \
    enemymodel.h \
    enemyview.h \
    entitymodel.h \
    gamecontroller.h \
    gamemodel.h \
    gameovercontroller.h \
    gameoverview.h \
    gamewoncontroller.h \
    gamewonview.h \
    goldmodel.h \
    hitzonemodel.h \
    hitzoneview.h \
    huntermodel.h \
    itemmodel.h \
    itemview.h \
    kingmodel.h \
    knightmodel.h \
    leaderboardcontroller.h \
    leaderboardmodel.h \
    leaderboardview.h \
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
    projectilemodel.h \
    projectileview.h \
    roommodel.h \
    roomview.h \
    settingmodel.h \
    settingview.h \
    settingcontroller.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc

DISTFILES +=

macx:QMAKE_CXXFLAGS += -Wno-error=implicit-function-declaration

macx:QMAKE_CFLAGS += -Wno-error=implicit-function-declaration
