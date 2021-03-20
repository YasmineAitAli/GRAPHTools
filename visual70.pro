QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets svg

CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += \
    arete.cpp \
    graphe.cpp \
    main.cpp \
    mainwindow.cpp \
    sommet.cpp

HEADERS += \
    arete.h \
    graphe.h \
    mainwindow.h \
    sommet.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc
RC_ICONS = Karbre.ico

