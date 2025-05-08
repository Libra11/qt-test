#include "pages/HomePage.h"
#include <QMessageBox>
#include "components/form/Form.h"
#include "components/devtools/DataViewerPanel.h"
#include "helpers/SettingsHelper.h"
#include <QDebug>

HomePage::HomePage(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

void HomePage::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    Form* form = new Form(this);

    QList<FormItem> items = {
        { "username", FormItemType::Input, "用户名", "请输入用户名", true },
        { "password", FormItemType::Input, "密码", "请输入密码", true }
    };

    form->setupByConfig(items);

    // 设置必填
    form->setRequired("username", true);
    form->setRequired("password", true);

    // 提交按钮文本
    form->setSubmitText("登录");

    // 监听提交
    connect(form, &Form::submitted, this, [](const QMap<QString, QString>& values) {
        qDebug() << "提交的内容:" << values;
    });

    mainLayout->addWidget(form);

    Form* form2 = new Form(this);

    QList<FormItem> items2 = {
        { "email", FormItemType::Input, "邮箱", "请输入邮箱", true },
        { "gender", FormItemType::Select, "性别", "请选择性别", true }
    };

    items2[1].options = QStringList{ "男", "女", "保密" };


    form2->setupByConfig(items2);

    // 设置必填
    form2->setRequired("email", true);
    form2->setRequired("gender", true);

    // 提交按钮
    form2->setSubmitText("注册");

    connect(form2, &Form::submitted, this, [](const QMap<QString, QString>& values) {
        qDebug() << "注册信息:" << values;
    });

    mainLayout->addWidget(form2);

    Form* form3 = new Form(this);

    QList<FormItem> items3 = {
        { "username", FormItemType::Input, "用户名", "请输入用户名", true },
        { "captcha", FormItemType::Custom, "验证码", "", true,
          [](QWidget* parent) -> QWidget* {
              QWidget* widget = new QWidget(parent);
              QHBoxLayout* hLayout = new QHBoxLayout(widget);
              Input* input = new Input(widget);
              input->setPlaceholder("输入验证码");
              Button* btn = new Button("获取验证码", widget);
              hLayout->addWidget(input);
              hLayout->addWidget(btn);
              hLayout->setContentsMargins(0, 0, 0, 0);
              return widget;
          }
        }
    };

    form3->setupByConfig(items3);
    form3->setSubmitText("提交");

    connect(form3, &Form::submitted, this, [](const QMap<QString, QString>& values) {
        qDebug() << "提交的数据:" << values;
    });

    mainLayout->addWidget(form3);



    Button *backBtn = new Button("返回登录页");
    QObject::connect(backBtn,&Button::clicked, [=](){
        emit(backToLoginPage());
    });
    mainLayout->addWidget(backBtn);

    DataViewerPanel *panel = new DataViewerPanel();
    panel->loadSettings(&SettingsHelper::instance());
    panel->show();
}
