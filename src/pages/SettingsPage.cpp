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
#include "components/base/Button.h"
#include "helpers/UserHelper.h"
#include "helpers/AdminHelper.h"
#include "helpers/HostsHelper.h"
#include "helpers/ContentProtectionHelper.h"
#include "helpers/SystemControlHelper.h"
#include "helpers/DNSHelper.h"
#include "components/IME/IMESelectorWidget.h"

SettingsPage::SettingsPage(QWidget *parent) : PageBase(parent)
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
    QObject::connect(adminBtn, &Button::clicked, this, []() {
        AdminHelper::RequestAdminLoop();
    });

    // 3. hosts管理区
    QGroupBox *hostsGroup = new QGroupBox("hosts管理");
    QVBoxLayout *hostsLayout = new QVBoxLayout(hostsGroup);
    Button *addBtn = new Button("添加hosts记录（屏蔽QQ输入法AI）");
    Button *removeBtn = new Button("删除hosts中的记录");
    addBtn->setVariant(Button::Variant::Secondary);
    removeBtn->setVariant(Button::Variant::Destructive);
    hostsLayout->addWidget(addBtn);
    hostsLayout->addWidget(removeBtn);
    mainLayout->addWidget(hostsGroup);
    QObject::connect(addBtn, &Button::clicked, this, [this]() {
        if (HostsHelper::AddHostsEntry()) {
            QMessageBox::information(this, "成功", "已成功添加屏蔽QQ输入法的hosts记录！");
        } else {
            QMessageBox::critical(this, "失败", "添加hosts记录失败！");
        }
    });
    QObject::connect(removeBtn, &Button::clicked, this, [this]() {
        if (HostsHelper::RemoveHostsEntry()) {
            QMessageBox::information(this, "成功", "已删除hosts中的QQ记录！");
        } else {
            QMessageBox::critical(this, "失败", "删除hosts记录失败！");
        }
    });

    // 4. 内容保护区
    QGroupBox *protectGroup = new QGroupBox("内容保护");
    QVBoxLayout *protectLayout = new QVBoxLayout(protectGroup);
    Button *protectBtn = new Button("启用内容保护");
    protectBtn->setVariant(Button::Variant::Outline);
    protectLayout->addWidget(protectBtn);
    mainLayout->addWidget(protectGroup);
    QObject::connect(protectBtn, &Button::clicked, this, [this, protectBtn]() {
        static bool enabled = false;
        enabled = !enabled;
        QWidget *mainWin = this->window(); // 获取主窗口指针
        ContentProtectionHelper::setContentProtection(mainWin, enabled);
        protectBtn->setText(enabled ? "禁用内容保护" : "启用内容保护");
    });

    QGroupBox *dnsGroup = new QGroupBox("DNS断外网管理");
    QVBoxLayout *dnsLayout = new QVBoxLayout(dnsGroup);
    Button *blockBtn = new Button("一键断外网");
    Button *restoreBtn = new Button("恢复DNS");
    blockBtn->setVariant(Button::Variant::Destructive);
    restoreBtn->setVariant(Button::Variant::Secondary);
    dnsLayout->addWidget(blockBtn);
    dnsLayout->addWidget(restoreBtn);
    mainLayout->addWidget(dnsGroup);

    QObject::connect(blockBtn, &Button::clicked, this, [this]() {
        if (DNSHelper::EnableDnsBlock()) {
            QMessageBox::information(this, "成功", "已成功断外网！");
        } else {
            QMessageBox::critical(this, "失败", "断外网失败！");
        }
    });
    QObject::connect(restoreBtn, &Button::clicked, this, [this]() {
        if (DNSHelper::DisableDnsBlock()) {
            QMessageBox::information(this, "成功", "已恢复DNS！");
        } else {
            QMessageBox::critical(this, "失败", "恢复DNS失败！");
        }
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
    QObject::connect(disableBtn, &Button::clicked, this, [this]() {
    SystemControlHelper::DisableTaskManager();
    SystemControlHelper::InstallHook();
    QWidget *mainWin = this->window();
    mainWin->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    mainWin->showFullScreen();
    QMessageBox::information(mainWin, "提示", "已禁用任务管理器并安装钩子！");
});
QObject::connect(enableBtn, &Button::clicked, this, [this]() {
    SystemControlHelper::EnableTaskManager();
    SystemControlHelper::UninstallHook();
    QWidget *mainWin = this->window();
    mainWin->setWindowFlags(Qt::Window);
    mainWin->showNormal();
    QMessageBox::information(mainWin, "提示", "已恢复任务管理器并卸载钩子！");
});

    // 返回登录页面按钮
    Button *backLoginBtn = new Button("返回登录页面");
    backLoginBtn->setVariant(Button::Variant::Outline);
    mainLayout->addWidget(backLoginBtn);
    QObject::connect(backLoginBtn, &Button::clicked, this, [this]() {
        emit routeTo("home");
    });
}

void SettingsPage::setupConnections()
{
    // 保留此函数，但不再做按钮相关连接
} 
