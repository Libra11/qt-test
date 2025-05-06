/*
 * @Author: Libra
 * @Date: 2025-05-02 03:38:24
 * @LastEditors: Libra
 * @Description: 
 */
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QGroupBox>
#include <QtDebug>
#include <QLabel>
#include <QStackedWidget>
#include "components/IME/IMESelectorWidget.h"
#include "helpers/AdminHelper.h"
#include "helpers/HostsHelper.h"
#include "helpers/ContentProtectionHelper.h"
#include "helpers/SystemControlHelper.h"
#include "helpers/UserHelper.h"
#include "components/base/Button.h"
#include "pages/HomePage.h"
#include "pages/SettingsPage.h"
#include "pages/PageManager.h"
#include "pages/LoginPage.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 主窗口
    QWidget window;
    window.setWindowTitle("国考云考试管理机");
    window.resize(1200, 700);

    // 创建堆叠窗口部件用于页面切换
    QStackedWidget *stackedWidget = new QStackedWidget(&window);
    PageManager *pageManager = new PageManager(stackedWidget);
    
    // 注册页面
    pageManager->registerPage("login", new LoginPage);
    pageManager->registerPage("settings", new SettingsPage);

    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(&window);
    mainLayout->addWidget(stackedWidget);

    // 启动时显示登录页
    pageManager->switchTo("login");

    // 登录成功后跳转到设置页
    LoginPage* loginPage = qobject_cast<LoginPage*>(pageManager->getPage("login"));
    if (loginPage) {
        QObject::connect(loginPage, &LoginPage::loginSuccess, [pageManager]() {
            pageManager->switchTo("settings");
        });
    }

    SettingsPage* settingsPage = qobject_cast<SettingsPage*>(pageManager->getPage("settings"));
    if (settingsPage) {
        QObject::connect(settingsPage, &SettingsPage::backToHome, [pageManager]() {
            pageManager->switchTo("login");
        });
    }

    // 退出时恢复系统设置
    QObject::connect(&app, &QApplication::aboutToQuit, []() {
        SystemControlHelper::UninstallHook();
        SystemControlHelper::EnableTaskManager();
    });

    window.show();
    return app.exec();
}
