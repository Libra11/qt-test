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
    src/components/base/TagSelect.cpp \
    src/helpers/AdminHelper.cpp \
    src/helpers/ContentProtectionHelper.cpp \
    src/helpers/HostsHelper.cpp \
    src/helpers/UserHelper.cpp \
    src/components/IME/IMESelectorWidget.cpp \
    src/helpers/SystemControlHelper.cpp \
    src/layout/FlowLayout.cpp \
    src/main.cpp \
    src/components/base/Button.cpp \
    src/components/base/Input.cpp \
    src/components/form/Form.cpp \
    src/components/form/FormSignals.cpp \
    src/components/form/FormValidation.cpp \
    src/components/form/FormLayoutManager.cpp \
    src/components/form/FormCustomWidget.cpp \
    src/components/form/FormInput.cpp \
    src/components/form/FormDropDown.cpp \
    src/components/base/DropDown.cpp \
    src/components/base/CustomMessageBox.cpp \
    src/components/devtools/DataViewerPanel.cpp \
    src/mainwindow.cpp \
    src/pages/HomePage.cpp \
    src/pages/SettingsPage.cpp \
    src/pages/PageManager.cpp \
    src/pages/LoginPage.cpp \
    src/pages/ExamCenterPage.cpp \
    src/helpers/NetworkHelper.cpp \
    src/helpers/LanguageManager.cpp \
    src/api/ExamApi.cpp \
    src/components/base/ClickableLabel.cpp \
    src/helpers/SettingsHelper.cpp \
    src/helpers/DNSHelper.cpp \
    src/components/base/CardWidget.cpp \
    src/components/base/Tab.cpp \
    src/components/base/Table.cpp \
    src/pages/ExamManagementPage.cpp

HEADERS += \
    include/components/base/TagSelect.h \
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
    include/components/form/FormCustomWidget.h \
    include/components/form/FormInput.h \
    include/components/form/FormDropDown.h \
    include/components/form/FormSignals.h \
    include/components/form/FormValidation.h \
    include/components/form/FormLayoutManager.h \
    include/components/base/DropDown.h \
    include/components/base/CustomMessageBox.h \
    include/components/devtools/DataViewerPanel.h \
    include/components/form/FormItem.h \
    include/layout/FlowLayout.h \
    include/mainwindow.h \
    include/pages/HomePage.h \
    include/pages/SettingsPage.h \
    include/pages/PageManager.h \
    include/pages/LoginPage.h \
    include/pages/PageBase.h \
    include/pages/ExamCenterPage.h \
    include/helpers/NetworkHelper.h \
    include/helpers/LanguageManager.h \
    include/api/ExamApi.h \
    include/components/base/ClickableLabel.h \
    include/helpers/SettingsHelper.h \
    include/helpers/DNSHelper.h \
    include/components/base/CardWidget.h \
    include/components/base/Tab.h \
    include/components/base/Table.h \
    include/pages/ExamManagementPage.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    i18n/translations/zh_CN.ts \
    i18n/translations/en.ts \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -lImm32 -lole32 -luuid -loleaut32 -lUser32
