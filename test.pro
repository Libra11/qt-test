QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT       += svg
RESOURCES += resources/icons.qrc
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
    src/helpers/AdminHelper.cpp \
    src/helpers/ContentProtectionHelper.cpp \
    src/helpers/HostsHelper.cpp \
    src/helpers/UserHelper.cpp \
    src/components/IME/IMESelectorWidget.cpp \
    src/helpers/SystemControlHelper.cpp \
    src/main.cpp \
    src/components/base/Button.cpp \
    src/components/base/Input.cpp \
    src/components/form/Form.cpp \
    src/components/base/Select.cpp \
    src/mainwindow.cpp \
    src/pages/HomePage.cpp \
    src/pages/SettingsPage.cpp \
    src/pages/PageManager.cpp \
    src/pages/LoginPage.cpp \
    src/helpers/NetworkHelper.cpp \
    src/api/ExamApi.cpp

HEADERS += \
    include/helpers/AdminHelper.h \
    include/helpers/ContentProtectionHelper.h \
    include/helpers/HostsHelper.h \
    include/helpers/UserHelper.h \
    include/components/IME/IMESelectorWidget.h \
    include/helpers/SystemControlHelper.h \
    include/components/base/Button.h \
    include/components/base/Input.h \
    include/components/base/ColorManager.h \
    include/components/form/Form.h \
    include/components/base/Select.h \
    include/components/form/FormItem.h \
    include/mainwindow.h \
    include/pages/HomePage.h \
    include/pages/SettingsPage.h \
    include/pages/PageManager.h \
    include/pages/LoginPage.h \
    include/helpers/NetworkHelper.h \
    include/api/ExamApi.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -lImm32 -lole32 -luuid -loleaut32 -lUser32
