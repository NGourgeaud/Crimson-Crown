QT += testlib widgets gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../CrimsonCrown

# Headers de l'interface
HEADERS += \
    ../CrimsonCrown/menuview.h \
    ../CrimsonCrown/charselectview.h \
    ../CrimsonCrown/entitymodel.h

# On teste uniquement tst_menus.cpp
SOURCES += \
    tst_testmenus.cpp \
    ../CrimsonCrown/menuview.cpp \
    ../CrimsonCrown/charselectview.cpp
