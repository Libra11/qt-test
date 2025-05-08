/*
 * @Author: Libra
 * @Date: 2025-05-06 15:01:42
 * @LastEditors: Libra
 * @Description: 
 */
#include "pages/LoginPage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QMessageBox>
#include <QUrl>
#include "components/base/Input.h"
#include "components/base/Button.h"
#include <QDebug>
#include "api/ExamApi.h"
#include "components/base/ClickableLabel.h"
#include "components/form/Form.h"
#include "components/form/FormItem.h"
#include "helpers/NetworkHelper.h"
#include "helpers/SettingsHelper.h"
#include "pages/PageBase.h"

LoginPage::LoginPage(QWidget *parent) : PageBase(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // 左侧插画
    QLabel *imgLabel = new QLabel;
    imgLabel->setPixmap(QPixmap(":/icons/login_illustration.png").scaled(400, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imgLabel->setAlignment(Qt::AlignCenter);
    Button *testBtn = new Button("去主页");
    mainLayout->addWidget(testBtn, 1);
    QObject::connect(testBtn, &Button::clicked, [=]() {
            emit routeTo("home");
    });
    //    mainLayout->addWidget(imgLabel, 1);

    // 右侧表单
    QWidget *formWidget = new QWidget;
    QVBoxLayout *formLayout = new QVBoxLayout(formWidget);

    formLayout->addStretch();

    QLabel *logoLabel = new QLabel("<b>国考云考试管理机</b>");
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setStyleSheet("font-size: 28px; margin-bottom: 20px;");
    formLayout->addWidget(logoLabel);

    // 声明验证码图片指针
    ClickableLabel *imageCodeLabel = nullptr;

    // 创建表单
    form = new Form;
    form->setLayout(Form::Layout::Vertical);
    form->setLabelPosition(Form::LabelPosition::Top);
    form->setLabelWidth(120);

    // 登录表单配置
    QList<FormItem> items = {
        FormItem(
            "loginCode",  // key
            FormItemType::Input,  // type
            tr("考场序列号"),  // label
            tr("请输入考场序列号"),  // placeholder
            true,  // required
            nullptr  // customWidgetFactory 为 nullptr，因为这是 Input 类型
        ),
        FormItem(
            "imageCode",  // key
            FormItemType::Custom,  // type
            tr("验证码"),  // label
            tr("请输入验证码"),  // placeholder（Custom 类型通常为空）
            true,  // required
            [&imageCodeLabel](QWidget* parent) -> QWidget* {  // customWidgetFactory
                QWidget* w = new QWidget(parent);
                QHBoxLayout* h = new QHBoxLayout(w);
                Input* input = new Input(w);
                input->setObjectName("imageCode");
                input->setPlaceholder("请输入验证码");
                imageCodeLabel = new ClickableLabel(w);
                imageCodeLabel->setFixedSize(100, 36);
                imageCodeLabel->setStyleSheet("background:#eee; border:1px solid #ccc;");
                h->addWidget(input, 1);
                h->addWidget(imageCodeLabel);
                h->setContentsMargins(0,0,0,0);
                return w;
            }
        )
    };

    form->setupByConfig(items);
    form->setSubmitText(tr("考场注册"));
    formLayout->addWidget(form);
    formLayout->addStretch();
    mainLayout->addWidget(formWidget, 1);

    // 自动填充loginCode
    QString savedLoginCode = SettingsHelper::value("loginCode").toString();
    if (!savedLoginCode.isEmpty()) {
        form->setValues({{"loginCode", savedLoginCode}});
    }

    // 获取验证码图片
    auto fetchImageCode = [=]() {
        ExamApi::getLoginImageCode(
            [=](QJsonObject obj) {
                QJsonObject d = obj["data"].toObject();
                QString base64 = d["imageCode"].toString();
                m_imageKey = d["key"].toString();
                // 去掉前缀
                if (base64.startsWith("data:image")) {
                    int commaIdx = base64.indexOf(',');
                    if (commaIdx != -1) {
                        base64 = base64.mid(commaIdx + 1);
                    }
                }
                QPixmap pix;
                if (imageCodeLabel && pix.loadFromData(QByteArray::fromBase64(base64.toUtf8())) && !pix.isNull()) {
                    imageCodeLabel->setPixmap(pix.scaled(imageCodeLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                } else if (imageCodeLabel) {
                    imageCodeLabel->setText(tr("加载失败"));
                }
            },
            [=](QString err) {
                if (imageCodeLabel) imageCodeLabel->setText("加载失败" + err);
            }
        );
    };
    fetchImageCode();

    // 验证码图片点击刷新
    if (imageCodeLabel) {
        QObject::connect(imageCodeLabel, &ClickableLabel::clicked, fetchImageCode);
    }

    // 监听验证错误
    QObject::connect(form, &Form::validationError, [](const QString& key, const QString& error) {
        qDebug() << "Validation error:" << key << error;
    });

    // 表单提交
    QObject::connect(form, &Form::submitted, [=](const QMap<QString, QString>& values) {
        QString loginCode = values.value("loginCode").trimmed();
        QString imageCode;
        // 获取自定义控件中的输入框内容
        QWidget* customWidget = form->getCustomWidget("imageCode");
        if (customWidget) {
            Input* input = customWidget->findChild<Input*>("imageCode");
            if (input) imageCode = input->text().trimmed();
        }

        ExamApi::login(loginCode, imageCode, m_imageKey,
            [=](QJsonObject obj) {
                if (obj["code"].toInt() == 0) {
                    // 存储token
                    QString token = obj["data"].toObject()["token"].toString();
                    NetworkHelper::instance()->setToken(token);
                    // 持久化loginCode
                    SettingsHelper::setValue("loginCode", loginCode);
                    emit routeTo("settings");
                } else {
                    QMessageBox::warning(this, "登录失败", obj["msg"].toString());
                    fetchImageCode();
                }
            },
            [=](QString err) {
                QMessageBox::warning(this, "网络错误", err);
                fetchImageCode();
            }
        );
    });
}
