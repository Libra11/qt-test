/*
 * @Author: Libra
 * @Date: 2025-05-06 14:36:46
 * @LastEditors: Libra
 * @Description: 
 */
#include "pages/SettingsPage.h"
#include <QMessageBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QGroupBox>
#include "components/base/Input.h"
#include "components/base/Button.h"
#include "components/form/Form.h"
#include "components/form/FormItem.h"
#include "helpers/UserHelper.h"
#include "helpers/AdminHelper.h"
#include "helpers/HostsHelper.h"
#include "helpers/ContentProtectionHelper.h"
#include "helpers/SystemControlHelper.h"
#include "components/IME/IMESelectorWidget.h"
#include <QDebug>

SettingsPage::SettingsPage(QWidget *parent) : QWidget(parent)
{
    setupUI();
    setupConnections();
}

void SettingsPage::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

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

    // 4. 内容保护区
    QGroupBox *protectGroup = new QGroupBox("内容保护");
    QVBoxLayout *protectLayout = new QVBoxLayout(protectGroup);
    Button *protectBtn = new Button("启用内容保护");
    protectBtn->setVariant(Button::Variant::Outline);
    protectLayout->addWidget(protectBtn);
    mainLayout->addWidget(protectGroup);

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

    // 示例：配置驱动的表单批量添加
    QList<FormItem> items = {
        {"username", "input", "请输入用户名", {}, true},
        {"password", "input", "请输入密码", {}, true},
        {"gender", "select", "请选择性别", {"男", "女"}, true}
    };
    Form* form = new Form;
    form->setupByConfig(items);
    mainLayout->addWidget(form);
}

void SettingsPage::setupConnections()
{
    // 获取管理员权限按钮
    Button *adminBtn = findChild<Button*>();
    if (adminBtn) {
        QObject::connect(adminBtn, &Button::clicked, [this]() {
            AdminHelper::RequestAdminLoop();
        });
    }

    // Hosts管理按钮
    QList<Button*> buttons = findChildren<Button*>();
    for (Button* btn : buttons) {
        if (btn->text() == "添加hosts记录（屏蔽百度）") {
            QObject::connect(btn, &Button::clicked, [this]() {
                if (HostsHelper::AddHostsEntry()) {
                    QMessageBox::information(this, "成功", "已成功添加屏蔽百度的hosts记录！");
                } else {
                    QMessageBox::critical(this, "失败", "添加hosts记录失败！");
                }
            });
        } else if (btn->text() == "删除hosts中的记录") {
            QObject::connect(btn, &Button::clicked, [this]() {
                if (HostsHelper::RemoveHostsEntry()) {
                    QMessageBox::information(this, "成功", "已删除hosts中的百度记录！");
                } else {
                    QMessageBox::critical(this, "失败", "删除hosts记录失败！");
                }
            });
        }
    }

    // 内容保护按钮
    Button *protectBtn = nullptr;
    for (Button* btn : buttons) {
        if (btn->text() == "启用内容保护") {
            protectBtn = btn;
            break;
        }
    }
    if (protectBtn) {
        QObject::connect(protectBtn, &Button::clicked, [this, protectBtn]() {
            static bool enabled = false;
            enabled = !enabled;
            ContentProtectionHelper::setContentProtection(this, enabled);
            protectBtn->setText(enabled ? "禁用内容保护" : "启用内容保护");
        });
    }

    // 系统控制按钮
    Button *disableBtn = nullptr;
    Button *enableBtn = nullptr;
    for (Button* btn : buttons) {
        if (btn->text() == "霸屏") {
            disableBtn = btn;
        } else if (btn->text() == "取消霸屏") {
            enableBtn = btn;
        }
    }
    if (disableBtn && enableBtn) {
        QObject::connect(disableBtn, &Button::clicked, [this]() {
            SystemControlHelper::DisableTaskManager();
            SystemControlHelper::InstallHook();
            this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
            this->showFullScreen();
            QMessageBox::information(this, "提示", "已禁用任务管理器并安装钩子！");
        });
        QObject::connect(enableBtn, &Button::clicked, [this]() {
            SystemControlHelper::EnableTaskManager();
            SystemControlHelper::UninstallHook();
            this->setWindowFlags(Qt::Window);
            this->showNormal();
            QMessageBox::information(this, "提示", "已恢复任务管理器并卸载钩子！");
        });
    }

    // 表单提交
    Form* form = findChild<Form*>();
    if (form) {
        QObject::connect(form, &Form::submitted, [](const QMap<QString, QString>& values){
            qDebug() << "表单提交:" << values;
        });
    }
} 