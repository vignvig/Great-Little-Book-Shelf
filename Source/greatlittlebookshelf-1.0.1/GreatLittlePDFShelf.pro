#-------------------------------------------------
#
# Project created by QtCreator 2013-07-24T21:22:51
#
#-------------------------------------------------

QT       += core gui

TARGET = greatlittlebookshelf
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    ebookdocument.cpp \
    watchfolder.cpp

HEADERS  += mainwindow.h \
    ebookdocument.h \
    watchfolder.h

FORMS    += mainwindow.ui \
    watchfolder.ui

RESOURCES += \
    Resource.qrc

INCLUDEPATH  += /usr/include/poppler/qt4
LIBS         += -L/usr/lib -lpoppler-qt4
