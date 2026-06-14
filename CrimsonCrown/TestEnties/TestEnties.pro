QT += testlib gui widgets

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
            ../CrimsonCrown/hitzonemodel.cpp \
            ../CrimsonCrown/charselectview.cpp \
            ../CrimsonCrown/menuview.cpp \
            ../CrimsonCrown/gamemodel.cpp \
            ../CrimsonCrown/leaderboardmodel.cpp \
            ../CrimsonCrown/playermodel.cpp \
            ../CrimsonCrown/roommodel.cpp \
            ../CrimsonCrown/obstaclemodel.cpp \
            ../CrimsonCrown/goldmodel.cpp \
            ../CrimsonCrown/itemmodel.cpp \
            ../CrimsonCrown/blacksmithmodel.cpp \
            ../CrimsonCrown/assassinmodel.cpp \
            ../CrimsonCrown/huntermodel.cpp

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
            ../CrimsonCrown/hitzonemodel.h \
            ../CrimsonCrown/charselectview.h \
            ../CrimsonCrown/menuview.h \
            ../CrimsonCrown/gamemodel.h \
            ../CrimsonCrown/leaderboardmodel.h \
            ../CrimsonCrown/playermodel.h \
            ../CrimsonCrown/roommodel.h \
            ../CrimsonCrown/obstaclemodel.h \
            ../CrimsonCrown/goldmodel.h \
            ../CrimsonCrown/itemmodel.h \
            ../CrimsonCrown/blacksmithmodel.h \
            ../CrimsonCrown/assassinmodel.h \
            ../CrimsonCrown/huntermodel.h

INCLUDEPATH += ../CrimsonCrown
