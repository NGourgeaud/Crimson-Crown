QT += testlib widgets gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../CrimsonCrown

SOURCES += tst_testleaderboard.cpp \
           ../CrimsonCrown/leaderboardmodel.cpp
