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
#include <QTranslator>
#include "components/IME/IMESelectorWidget.h"
#include "helpers/AdminHelper.h"
#include "helpers/HostsHelper.h"
#include "helpers/ContentProtectionHelper.h"
#include "helpers/SystemControlHelper.h"
#include "helpers/LanguageManager.h"
#include "helpers/UserHelper.h"
#include "components/base/Button.h"
#include "pages/HomePage.h"
#include "pages/SettingsPage.h"
#include "pages/PageManager.h"
#include "pages/LoginPage.h"
#include "pages/ExamCenterPage.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 主窗口
    QWidget window;
    window.setWindowTitle("国考云考试管理机");
    window.setObjectName("window");
    window.setStyleSheet(R"(
        #window {
            background: #ffffff;
         }
    )");
    window.resize(1200, 700);

    // 创建堆叠窗口部件用于页面切换
    QStackedWidget *stackedWidget = new QStackedWidget(&window);
    PageManager *pageManager = new PageManager(stackedWidget);

    // 注册页面
    pageManager->registerPage("login", new LoginPage);
    pageManager->registerPage("settings", new SettingsPage);
    pageManager->registerPage("home", new HomePage);
    pageManager->registerPage("examcenter", new ExamCenterPage);

    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(&window);
    mainLayout->addWidget(stackedWidget);

    // 启动时显示登录页
    pageManager->switchTo("login");


    // 退出时恢复系统设置
    QObject::connect(&app, &QApplication::aboutToQuit, []() {
        SystemControlHelper::UninstallHook();
        SystemControlHelper::EnableTaskManager();
    });

    // 加载默认语言（可以保存配置决定默认用什么语言）
    LanguageManager::instance()->loadLanguage("zh_CN");

    // 语言切换时刷新主窗口标题
    QObject::connect(LanguageManager::instance(), &LanguageManager::languageChanged, [&]() {
        window.setWindowTitle(QObject::tr("国考云考试管理机"));
    });
    window.show();
    return app.exec();
}
