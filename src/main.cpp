#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QGroupBox>
#include <QLabel>
#include "IMESelectorWidget.h"
#include "AdminHelper.h"
#include "HostsHelper.h"
#include "ContentProtectionHelper.h"
#include "SystemControlHelper.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 主窗口
    QWidget window;
    window.setWindowTitle("输入法切换 + hosts管理 + 内容保护 + 霸屏");
    window.resize(600, 600);

    QVBoxLayout *mainLayout = new QVBoxLayout(&window);

    // 1. 输入法切换区
    QGroupBox *imeGroup = new QGroupBox("输入法切换");
    QVBoxLayout *imeLayout = new QVBoxLayout(imeGroup);
    auto *imeWidget = new IMESelectorWidget;
    imeLayout->addWidget(imeWidget);
    mainLayout->addWidget(imeGroup);

    // 2. 获取管理员权限
    QGroupBox *adminGroup = new QGroupBox("管理员权限");
    QVBoxLayout *adminLayout = new QVBoxLayout(adminGroup);
    QPushButton *adminBtn = new QPushButton("请求管理员权限");
    adminLayout->addWidget(adminBtn);
    mainLayout->addWidget(adminGroup);

    QObject::connect(adminBtn, &QPushButton::clicked, [&]() {
        AdminHelper::RequestAdminLoop();
    });

    // 3. hosts管理区
    QGroupBox *hostsGroup = new QGroupBox("hosts管理");
    QVBoxLayout *hostsLayout = new QVBoxLayout(hostsGroup);
    QPushButton *addBtn = new QPushButton("添加hosts记录（屏蔽百度）");
    QPushButton *removeBtn = new QPushButton("删除hosts中的记录");
    hostsLayout->addWidget(addBtn);
    hostsLayout->addWidget(removeBtn);
    mainLayout->addWidget(hostsGroup);

    QObject::connect(addBtn, &QPushButton::clicked, [&]() {
        if (HostsHelper::AddHostsEntry()) {
            QMessageBox::information(&window, "成功", "已成功添加屏蔽百度的hosts记录！");
        } else {
            QMessageBox::critical(&window, "失败", "添加hosts记录失败！");
        }
    });
    QObject::connect(removeBtn, &QPushButton::clicked, [&]() {
        if (HostsHelper::RemoveHostsEntry()) {
            QMessageBox::information(&window, "成功", "已删除hosts中的百度记录！");
        } else {
            QMessageBox::critical(&window, "失败", "删除hosts记录失败！");
        }
    });

    // 4. 内容保护区
    QGroupBox *protectGroup = new QGroupBox("内容保护");
    QVBoxLayout *protectLayout = new QVBoxLayout(protectGroup);
    QPushButton *protectBtn = new QPushButton("启用内容保护");
    protectLayout->addWidget(protectBtn);
    mainLayout->addWidget(protectGroup);

    QObject::connect(protectBtn, &QPushButton::clicked, [&]() {
        static bool enabled = false;
        enabled = !enabled;
        ContentProtectionHelper::setContentProtection(&window, enabled);
        protectBtn->setText(enabled ? "禁用内容保护" : "启用内容保护");
    });

    // 5. 系统控制区
    QGroupBox *sysGroup = new QGroupBox("系统控制");
    QVBoxLayout *sysLayout = new QVBoxLayout(sysGroup);
    QPushButton *disableBtn = new QPushButton("霸屏");
    QPushButton *enableBtn = new QPushButton("取消霸屏");
    sysLayout->addWidget(disableBtn);
    sysLayout->addWidget(enableBtn);
    mainLayout->addWidget(sysGroup);

    QObject::connect(disableBtn, &QPushButton::clicked, [&]() {
        SystemControlHelper::DisableTaskManager();
        SystemControlHelper::InstallHook();
        window.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        window.showFullScreen();
        QMessageBox::information(&window, "提示", "已禁用任务管理器并安装钩子！");
    });
    QObject::connect(enableBtn, &QPushButton::clicked, [&]() {
        SystemControlHelper::EnableTaskManager();
        SystemControlHelper::UninstallHook();
        window.setWindowFlags(Qt::Window);
        window.showNormal();
        QMessageBox::information(&window, "提示", "已恢复任务管理器并卸载钩子！");
    });

    // 退出时恢复
    QObject::connect(&app, &QApplication::aboutToQuit, [&]() {
        SystemControlHelper::UninstallHook();
        SystemControlHelper::EnableTaskManager();
    });

    window.show();
    return app.exec();
}
