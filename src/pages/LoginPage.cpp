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

LoginPage::LoginPage(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // 左侧插画
    QLabel *imgLabel = new QLabel;
    imgLabel->setPixmap(QPixmap(":/icons/login_illustration.png").scaled(400, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imgLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(imgLabel, 1);

    // 右侧表单
    QWidget *formWidget = new QWidget;
    QVBoxLayout *formLayout = new QVBoxLayout(formWidget);

    QLabel *logoLabel = new QLabel("<b>国考云考试管理机</b>");
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setStyleSheet("font-size: 28px; margin-bottom: 20px;");
    formLayout->addWidget(logoLabel);

    Input *loginCodeInput = new Input;
    loginCodeInput->setPlaceholder("考场序列号");
    formLayout->addWidget(loginCodeInput);

    QHBoxLayout *codeLayout = new QHBoxLayout;
    Input *imageCodeInput = new Input;
    imageCodeInput->setPlaceholder("请输入验证码");
    QLabel *imageCodeLabel = new QLabel;
    imageCodeLabel->setFixedSize(100, 36);
    imageCodeLabel->setStyleSheet("background:#eee; border:1px solid #ccc;");
    codeLayout->addWidget(imageCodeInput, 1);
    codeLayout->addWidget(imageCodeLabel);
    formLayout->addLayout(codeLayout);

    Button *refreshBtn = new Button("换一张");
    formLayout->addWidget(refreshBtn);

    Button *loginBtn = new Button("考场注册");
    loginBtn->setVariant(Button::Variant::Outline);
    formLayout->addWidget(loginBtn);

    formLayout->addStretch();
    mainLayout->addWidget(formWidget, 1);

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
                if (pix.loadFromData(QByteArray::fromBase64(base64.toUtf8())) && !pix.isNull()) {
                    imageCodeLabel->setPixmap(pix.scaled(imageCodeLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                } else {
                    imageCodeLabel->setText("加载失败");
                }
            },
            [=](QString err) {
                imageCodeLabel->setText("加载失败" + err);
            }
        );
    };
    fetchImageCode();

    QObject::connect(refreshBtn, &Button::clicked, fetchImageCode);

    // 登录
    QObject::connect(loginBtn, &Button::clicked, [=]() {
        QString loginCode = loginCodeInput->text().trimmed();
        QString imageCode = imageCodeInput->text().trimmed();
        if (loginCode.isEmpty() || imageCode.isEmpty()) {
            QMessageBox::warning(this, "提示", "请填写所有信息");
            return;
        }
        ExamApi::login(loginCode, imageCode, m_imageKey,
            [=](QJsonObject obj) {
                if (obj["code"].toInt() == 0) {
                    emit loginSuccess();
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
