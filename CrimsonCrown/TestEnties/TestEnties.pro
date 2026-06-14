QT += testlib gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_testentity.cpp \
    ../CrimsonCrown/archermodel.cpp \
    ../CrimsonCrown/bossmodel.cpp \
    ../CrimsonCrown/clergymodel.cpp \
    ../CrimsonCrown/enemymodel.cpp \
    ../CrimsonCrown/entitymodel.cpp \
    ../CrimsonCrown/kingmodel.cpp \
    ../CrimsonCrown/knightmodel.cpp \
    ../CrimsonCrown/lancermodel.cpp \
    ../CrimsonCrown/lordmodel.cpp \
    ../CrimsonCrown/projectilemodel.cpp \
    ../CrimsonCrown/hitzonemodel.cpp

HEADERS += \
    ../CrimsonCrown/archermodel.h \
    ../CrimsonCrown/bossmodel.h \
    ../CrimsonCrown/clergymodel.h \
    ../CrimsonCrown/enemymodel.h \
    ../CrimsonCrown/entitymodel.h \
    ../CrimsonCrown/kingmodel.h \
    ../CrimsonCrown/knightmodel.h \
    ../CrimsonCrown/lancermodel.h \
    ../CrimsonCrown/lordmodel.h \
    ../CrimsonCrown/projectilemodel.h \
    ../CrimsonCrown/hitzonemodel.h

INCLUDEPATH += ../CrimsonCrown
