QT       += core gui network multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20 console

QMAKE_CXXFLAGS += "-Wno-c++20-extensions"
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



SOURCES += \
    joystickhandler.cpp \
    joysticksetupdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    rovcameracapture.cpp \
    rovcommunication.cpp \
    rovdataparser.cpp \
    thrustersetupdialog.cpp

HEADERS += \
    bithelpers.h \
    joystick.h \
    joystickhandler.h \
    joysticksetupdialog.h \
    mainwindow.h \
    rovcameracapture.h \
    rovcommunication.h \
    rovdataparser.h \
    rovdatatypes.h \
    thrustersetupdialog.h

FORMS += \
    joysticksetupdialog.ui \
    mainwindow.ui \
    thrustersetupdialog.ui

RESOURCES += \
    resources.qrc


unix{
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv4
    PKGCONFIG += sdl2
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    placeholder.jpg
