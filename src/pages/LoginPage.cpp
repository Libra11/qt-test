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
#include <QUrl>
#include <QFrame>
#include <QSizePolicy>
#include "components/base/Input.h"
#include "components/base/Button.h"
#include <QDebug>
#include "api/ExamApi.h"
#include "components/base/ClickableLabel.h"
#include "components/form/Form.h"
#include "components/form/FormItem.h"
#include "components/base/CustomMessageBox.h"
#include "helpers/NetworkHelper.h"
#include "helpers/SettingsHelper.h"
#include "pages/PageBase.h"

LoginPage::LoginPage(QWidget *parent) : PageBase(parent)
{
    // 移除默认边距
    setContentsMargins(0, 0, 0, 0);

    // 创建主布局
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 左侧容器（灰色背景）
    QFrame *leftContainer = new QFrame;
    leftContainer->setStyleSheet("background-color: #f5f5f5;"); // 灰色背景
    leftContainer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    // 左侧布局
    QVBoxLayout *leftLayout = new QVBoxLayout(leftContainer);
    leftLayout->setAlignment(Qt::AlignCenter);

    // 左侧插画
    QLabel *imgLabel = new QLabel;
    // 保存图片路径以便在resize事件中使用
    m_loginImagePath = ":/icons/login.png";

    // 初始图片宽度设置为左侧区域的60%
    int imgWidth = 0.6 * leftContainer->width();
    if (imgWidth <= 0) imgWidth = 400; // 初始默认值

    imgLabel->setPixmap(QPixmap(m_loginImagePath).scaled(imgWidth, imgWidth, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imgLabel->setAlignment(Qt::AlignCenter);
    imgLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    // 保存imgLabel指针以便在resize事件中使用
    m_imgLabel = imgLabel;

    // 添加到左侧布局
    leftLayout->addWidget(imgLabel);

    // 右侧容器
    QFrame *rightContainer = new QFrame;
    rightContainer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    // 右侧布局
    QVBoxLayout *rightLayout = new QVBoxLayout(rightContainer);
    rightLayout->setAlignment(Qt::AlignCenter);

    // 右侧表单容器（最大宽度500px）
    QWidget *formWidget = new QWidget;
    formWidget->setMaximumWidth(500);
    QVBoxLayout *formLayout = new QVBoxLayout(formWidget);

    formLayout->addStretch();

    QLabel *logoLabel = new QLabel("<b>国考云考试管理机</b>");
    QPixmap iconPixmap(":/icons/logo.png");
    QLabel *iconLabel = new QLabel();
    iconLabel->setPixmap(iconPixmap.scaled(iconPixmap.width() * 80 / iconPixmap.height(), 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    iconLabel->setAlignment(Qt::AlignLeft);
    logoLabel->setAlignment(Qt::AlignLeft);
    logoLabel->setStyleSheet("font-size: 48px; margin: 20px 0px;");
    formLayout->addWidget(iconLabel);
    formLayout->addWidget(logoLabel);

    // 声明验证码图片指针
    ClickableLabel *imageCodeLabel = nullptr;

    // 添加表单容器到右侧布局
    rightLayout->addWidget(formWidget);

    // 添加左右容器到主布局，各占50%
    mainLayout->addWidget(leftContainer, 1);
    mainLayout->addWidget(rightContainer, 1);

    // 创建表单
    form = new Form;
    form->setLayout(Form::Layout::Vertical);
    form->setLabelPosition(Form::LabelPosition::Top);
    form->setLabelWidth(120);

    // 登录表单配置
    QList<FormItem> items = {
        FormItem(
            "loginCode",  // key
            FormItemType::Password,  // type
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
                imageCodeLabel->setFixedSize(100, 52);
                imageCodeLabel->setStyleSheet("border-radius:4px; border: 1px solid #eee;");
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

    // Test
    Button *homeBtn = new Button("去主页");
    QObject::connect(homeBtn, &Button::clicked, [this](){
            emit routeTo("home");
    });
    formLayout->addWidget(homeBtn);
    formLayout->addStretch();
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
        form->setLoading(true);
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
                    QString token = obj["data"].toString();
                    NetworkHelper::instance()->setToken(token);
                    // 持久化loginCode
                    SettingsHelper::setValue("loginCode", loginCode);
                    emit routeTo("examcenter");
                } else {
                    CustomMessageBox::warning(this, "登录失败", obj["message"].toString());
                    fetchImageCode();
                }
                form->setLoading(false);
            },
            [=](QString err) {
                form->setLoading(false);
                CustomMessageBox::warning(this, "网络错误", err);
                fetchImageCode();
            }
        );
    });
}

void LoginPage::resizeEvent(QResizeEvent *event)
{
    PageBase::resizeEvent(event);

    // 调整图片大小为左侧区域的60%宽度
    if (m_imgLabel) {
        // 获取左侧容器的宽度
        QWidget* leftContainer = m_imgLabel->parentWidget();
        if (leftContainer) {
            int containerWidth = leftContainer->width();
            int imgWidth = containerWidth * 0.6; // 60%的宽度

            // 更新图片大小
            m_imgLabel->setPixmap(QPixmap(m_loginImagePath).scaled(
                imgWidth, imgWidth,
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
            ));
        }
    }
}
