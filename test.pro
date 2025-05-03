QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += include

SOURCES += \
    src/AdminHelper.cpp \
    src/ContentProtectionHelper.cpp \
    src/HostsHelper.cpp \
    src/IMESelectorWidget.cpp \
    src/SystemControlHelper.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/components/Button.cpp \
    src/components/Input.cpp \
    src/components/Form.cpp \
    src/components/Select.cpp

HEADERS += \
    include/AdminHelper.h \
    include/ContentProtectionHelper.h \
    include/HostsHelper.h \
    include/IMESelectorWidget.h \
    include/SystemControlHelper.h \
    include/mainwindow.h \
    include/components/Button.h \
    include/components/Input.h \
    include/components/ColorManager.h \
    include/components/Form.h \
    include/components/Select.h \
    include/components/FormItem.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -lImm32 -lole32 -luuid -loleaut32 -lUser32
