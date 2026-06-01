QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    appcontroller.cpp \
    main.cpp \
    mainwindow.cpp \
    menucontroller.cpp \
    menuview.cpp \
    enemymodel.cpp \
    entitymodel.cpp \
    playermodel.cpp

HEADERS += \
    appcontroller.h \
    config.h \
    mainwindow.h \
    menucontroller.h \
    menuview.h \
    enemymodel.h \
    entitymodel.h \
    playermodel.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc

DISTFILES +=
