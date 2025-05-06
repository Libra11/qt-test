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
#include "components/IME/IMESelectorWidget.h"
#include "helpers/AdminHelper.h"
#include "helpers/HostsHelper.h"
#include "helpers/ContentProtectionHelper.h"
#include "helpers/SystemControlHelper.h"
#include "helpers/UserHelper.h"
#include "components/base/Button.h"
#include "components/form/Form.h"
#include "components/form/FormItem.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 主窗口
    QWidget window;
    window.setWindowTitle("输入法切换 + hosts管理 + 内容保护 + 霸屏");
    window.resize(600, 600);

    QVBoxLayout *mainLayout = new QVBoxLayout(&window);

    // 显示当前用户状态
    QGroupBox *userGroup = new QGroupBox("当前用户状态");
    QVBoxLayout *userLayout = new QVBoxLayout(userGroup);
    QLabel *userLabel = new QLabel();
    QString username = UserHelper::GetCurrentUsername();
    bool isGuest = UserHelper::IsGuestUser();
    userLabel->setText(QString("当前用户: %1\n是否为Guest用户: %2")
                      .arg(username)
                      .arg(isGuest ? "是" : "否"));
    userLayout->addWidget(userLabel);
    mainLayout->addWidget(userGroup);

    // 1. 输入法切换区
    QGroupBox *imeGroup = new QGroupBox("输入法切换");
    QVBoxLayout *imeLayout = new QVBoxLayout(imeGroup);
    auto *imeWidget = new IMESelectorWidget;
    imeLayout->addWidget(imeWidget);
    mainLayout->addWidget(imeGroup);

    // 2. 获取管理员权限
    QGroupBox *adminGroup = new QGroupBox("管理员权限");
    QVBoxLayout *adminLayout = new QVBoxLayout(adminGroup);
    Button *adminBtn = new Button("请求管理员权限");
    adminBtn->setVariant(Button::Variant::Outline);
    adminLayout->addWidget(adminBtn);
    mainLayout->addWidget(adminGroup);

    QObject::connect(adminBtn, &Button::clicked, [&]() {
        AdminHelper::RequestAdminLoop();
    });

    // 3. hosts管理区
    QGroupBox *hostsGroup = new QGroupBox("hosts管理");
    QVBoxLayout *hostsLayout = new QVBoxLayout(hostsGroup);
    Button *addBtn = new Button("添加hosts记录（屏蔽百度）");
    Button *removeBtn = new Button("删除hosts中的记录");
    addBtn->setVariant(Button::Variant::Secondary);
    removeBtn->setVariant(Button::Variant::Destructive);
    hostsLayout->addWidget(addBtn);
    hostsLayout->addWidget(removeBtn);
    mainLayout->addWidget(hostsGroup);

    QObject::connect(addBtn, &Button::clicked, [&]() {
        if (HostsHelper::AddHostsEntry()) {
            QMessageBox::information(&window, "成功", "已成功添加屏蔽百度的hosts记录！");
        } else {
            QMessageBox::critical(&window, "失败", "添加hosts记录失败！");
        }
    });
    QObject::connect(removeBtn, &Button::clicked, [&]() {
        if (HostsHelper::RemoveHostsEntry()) {
            QMessageBox::information(&window, "成功", "已删除hosts中的百度记录！");
        } else {
            QMessageBox::critical(&window, "失败", "删除hosts记录失败！");
        }
    });

    // 4. 内容保护区
    QGroupBox *protectGroup = new QGroupBox("内容保护");
    QVBoxLayout *protectLayout = new QVBoxLayout(protectGroup);
    Button *protectBtn = new Button("启用内容保护");
    protectBtn->setVariant(Button::Variant::Outline);
    protectLayout->addWidget(protectBtn);
    mainLayout->addWidget(protectGroup);

    QObject::connect(protectBtn, &Button::clicked, [&]() {
        static bool enabled = false;
        enabled = !enabled;
        ContentProtectionHelper::setContentProtection(&window, enabled);
        protectBtn->setText(enabled ? "禁用内容保护" : "启用内容保护");
    });

    // 5. 系统控制区
    QGroupBox *sysGroup = new QGroupBox("系统控制");
    QVBoxLayout *sysLayout = new QVBoxLayout(sysGroup);
    Button *disableBtn = new Button("霸屏");
    Button *enableBtn = new Button("取消霸屏");
    disableBtn->setVariant(Button::Variant::Destructive);
    enableBtn->setVariant(Button::Variant::Secondary);
    sysLayout->addWidget(disableBtn);
    sysLayout->addWidget(enableBtn);
    mainLayout->addWidget(sysGroup);

    QObject::connect(disableBtn, &Button::clicked, [&]() {
        SystemControlHelper::DisableTaskManager();
        SystemControlHelper::InstallHook();
        window.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        window.showFullScreen();
        QMessageBox::information(&window, "提示", "已禁用任务管理器并安装钩子！");
    });
    QObject::connect(enableBtn, &Button::clicked, [&]() {
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

    // 示例：配置驱动的表单批量添加
    QList<FormItem> items = {
        {"username", "input", "请输入用户名", {}, true},
        {"password", "input", "请输入密码", {}, true},
        {"gender", "select", "请选择性别", {"男", "女"}, true}
    };
    Form* form = new Form;
    form->setupByConfig(items);
    mainLayout->addWidget(form);

    QObject::connect(form, &Form::submitted, [](const QMap<QString, QString>& values){
        qDebug() << "表单提交:" << values;
    });

    window.show();
    return app.exec();
}
