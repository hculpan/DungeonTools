#-------------------------------------------------
#
# Project created by QtCreator 2019-07-13T21:17:56
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DungeonTools
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        activeencounterdelegate.cpp \
        activeencountertablemodel.cpp \
        db/db.cpp \
        db/encounter.cpp \
        db/monster.cpp \
        db/monstergroup.cpp \
        encountermodel.cpp \
        main.cpp \
        mainwindow.cpp \
        newencounterdialog.cpp \
        sessionsxptablemodel.cpp \
        sessionxpdialog.cpp \
        settings.cpp \
        startencounterdialog.cpp \

HEADERS += \
        activeencounterdelegate.h \
        activeencountertablemodel.h \
        db/db.h \
        db/encounter.h \
        db/monster.h \
        db/monstergroup.h \
        encountermodel.h \
        mainwindow.h \
        newencounterdialog.h \
        sessionsxptablemodel.h \
        sessionxpdialog.h \
        settings.h \
        startencounterdialog.h \

FORMS += \
        mainwindow.ui \
        newencounterdialog.ui \
        sessionxpdialog.ui \
        startencounterdialog.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    DungeonTools.pro.user
