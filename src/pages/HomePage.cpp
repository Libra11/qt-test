#include "pages/HomePage.h"
#include <QMessageBox>
#include <QTimer>
#include <QGroupBox>
#include <QLabel>
#include "components/form/Form.h"
#include "components/devtools/DataViewerPanel.h"
#include "components/base/CustomMessageBox.h"
#include "helpers/SettingsHelper.h"
#include "components/base/TagSelect.h"
#include <QDebug>

HomePage::HomePage(QWidget *parent) : PageBase(parent)
{
    setupUI();
}

void HomePage::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 1. 按钮示例区域
    QGroupBox *buttonGroupBox = new QGroupBox("按钮示例");
    QVBoxLayout *buttonLayout = new QVBoxLayout(buttonGroupBox);

    // 1.1 Icon 按钮示例
    QLabel *iconButtonLabel = new QLabel("Icon 按钮示例");
    iconButtonLabel->setStyleSheet("font-weight: bold;");
    buttonLayout->addWidget(iconButtonLabel);

    QHBoxLayout *iconButtonsLayout = new QHBoxLayout();

    // 纯图标按钮
    Button *iconOnlyBtn = new Button(QIcon(":/icons/loading.svg"));
    iconOnlyBtn->setSize(Button::Size::Icon);
    iconOnlyBtn->setVariant(Button::Variant::Default);

    // 带图标和文字的按钮
    Button *iconTextBtn = new Button(QIcon(":/icons/loading.svg"), "带图标的按钮");
    iconTextBtn->setVariant(Button::Variant::Default);

    // 轮廓风格的图标按钮
    Button *iconOutlineBtn = new Button(QIcon(":/icons/loading.svg"), "轮廓风格");
    iconOutlineBtn->setVariant(Button::Variant::Outline);

    iconButtonsLayout->addWidget(iconOnlyBtn);
    iconButtonsLayout->addWidget(iconTextBtn);
    iconButtonsLayout->addWidget(iconOutlineBtn);
    buttonLayout->addLayout(iconButtonsLayout);

    // 1.2 Loading 按钮示例
    QLabel *loadingButtonLabel = new QLabel("Loading 按钮示例");
    loadingButtonLabel->setStyleSheet("font-weight: bold; margin-top: 10px;");
    buttonLayout->addWidget(loadingButtonLabel);

    QHBoxLayout *loadingButtonsLayout = new QHBoxLayout();

    // 普通 Loading 按钮
    Button *loadingBtn = new Button("点击加载");
    loadingBtn->setVariant(Button::Variant::Default);
    connect(loadingBtn, &Button::clicked, [loadingBtn]() {
        loadingBtn->setLoading(true);
        QTimer::singleShot(2000, [loadingBtn]() {
            loadingBtn->setLoading(false);
        });
    });

    // 带图标的 Loading 按钮
    Button *iconLoadingBtn = new Button(QIcon(":/icons/loading.svg"), "带图标加载");
    iconLoadingBtn->setVariant(Button::Variant::Default);
    connect(iconLoadingBtn, &Button::clicked, [iconLoadingBtn]() {
        iconLoadingBtn->setLoading(true);
        QTimer::singleShot(2000, [iconLoadingBtn]() {
            iconLoadingBtn->setLoading(false);
        });
    });

    // 纯图标 Loading 按钮
    Button *iconOnlyLoadingBtn = new Button(QIcon(":/icons/loading.svg"));
    iconOnlyLoadingBtn->setSize(Button::Size::Icon);
    iconOnlyLoadingBtn->setVariant(Button::Variant::Default);
    connect(iconOnlyLoadingBtn, &Button::clicked, [iconOnlyLoadingBtn]() {
        iconOnlyLoadingBtn->setLoading(true);
        QTimer::singleShot(2000, [iconOnlyLoadingBtn]() {
            iconOnlyLoadingBtn->setLoading(false);
        });
    });

    loadingButtonsLayout->addWidget(loadingBtn);
    loadingButtonsLayout->addWidget(iconLoadingBtn);
    loadingButtonsLayout->addWidget(iconOnlyLoadingBtn);
    buttonLayout->addLayout(loadingButtonsLayout);

    mainLayout->addWidget(buttonGroupBox);

    // 2. 表单示例区域
    QGroupBox *formGroupBox = new QGroupBox("表单示例");
    QVBoxLayout *formLayout = new QVBoxLayout(formGroupBox);

    // 2.1 带 Loading 状态的登录表单
    QLabel *loginFormLabel = new QLabel("登录表单（提交时显示 Loading）");
    loginFormLabel->setStyleSheet("font-weight: bold;");
    formLayout->addWidget(loginFormLabel);

    Form* loginForm = new Form(this);

    QList<FormItem> loginItems = {
        { "username", FormItemType::Input, "用户名", "请输入用户名", true },
        { "password", FormItemType::Password, "密码", "请输入密码", true }
    };

    loginForm->setupByConfig(loginItems);
    loginForm->setRequired("username", true);
    loginForm->setRequired("password", true);

    // 设置提交按钮文本
    loginForm->setSubmitText("登录");

    // 监听提交，模拟异步请求
    connect(loginForm, &Form::submitted, this, [loginForm](const QMap<QString, QString>& values) {
        qDebug() << "登录表单提交:" << values;

        // 模拟 API 请求延迟
        QTimer::singleShot(2000, [loginForm]() {
            // 请求完成，停止 loading 状态
            loginForm->setLoading(false);
            qDebug() << "登录请求完成!";
        });
    });

    formLayout->addWidget(loginForm);

    // 2.2 带验证码的注册表单
    QLabel *registerFormLabel = new QLabel("注册表单（带验证码按钮）");
    registerFormLabel->setStyleSheet("font-weight: bold; margin-top: 10px;");
    formLayout->addWidget(registerFormLabel);

    Form* registerForm = new Form(this);

    QList<FormItem> registerItems = {
        { "email", FormItemType::Input, "邮箱", "请输入邮箱", true },
        { "captcha", FormItemType::Custom, "验证码", "", true,
          [](QWidget* parent) -> QWidget* {
              QWidget* widget = new QWidget(parent);
              QHBoxLayout* hLayout = new QHBoxLayout(widget);
              Input* input = new Input(widget);
              input->setObjectName("captcha");
              input->setPlaceholder("输入验证码");

              // 使用带 loading 状态的获取验证码按钮
              Button* captchaBtn = new Button("获取验证码", widget);
              captchaBtn->setVariant(Button::Variant::Outline);

              // 点击获取验证码
              QObject::connect(captchaBtn, &Button::clicked, [captchaBtn]() {
                  if (!captchaBtn->isLoading()) {
                      captchaBtn->setLoading(true);
                      captchaBtn->setEnabled(false);

                      // 模拟发送验证码
                      QTimer::singleShot(2000, [captchaBtn]() {
                          captchaBtn->setLoading(false);
                          captchaBtn->setText("重新获取(60s)");

                          // 模拟倒计时
                          static int countdown = 60;
                          QTimer* timer = new QTimer(captchaBtn);
                          QObject::connect(timer, &QTimer::timeout, [captchaBtn, timer]() {
                              countdown--;
                              if (countdown <= 0) {
                                  timer->stop();
                                  captchaBtn->setText("获取验证码");
                                  captchaBtn->setEnabled(true);
                                  countdown = 60;
                                  timer->deleteLater();
                              } else {
                                  captchaBtn->setText(QString("重新获取(%1s)").arg(countdown));
                              }
                          });
                          timer->start(1000);
                      });
                  }
              });

              hLayout->addWidget(input);
              hLayout->addWidget(captchaBtn);
              hLayout->setContentsMargins(0, 0, 0, 0);
              return widget;
          }
        },
        { "password", FormItemType::Password, "密码", "请输入密码", true }
    };

    // 添加邮箱验证规则
    registerItems[0].validationRules.append({
        [](const QString& val) { return val.contains("@"); },
        "请输入有效的邮箱地址"
    });

    registerForm->setupByConfig(registerItems);
    registerForm->setRequired("email", true);
    registerForm->setRequired("captcha", true);
    registerForm->setRequired("password", true);

    // 设置提交按钮
    registerForm->setSubmitText("注册");

    // 监听提交
    connect(registerForm, &Form::submitted, this, [registerForm](const QMap<QString, QString>& values) {
        qDebug() << "注册表单提交:" << values;

        // 模拟 API 请求延迟
        QTimer::singleShot(3000, [registerForm]() {
            // 请求完成，停止 loading 状态
            registerForm->setLoading(false);
            qDebug() << "注册请求完成!";
        });
    });

    formLayout->addWidget(registerForm);

    mainLayout->addWidget(formGroupBox);

    // 3. 密码输入框示例区域
    QGroupBox *passwordGroupBox = new QGroupBox("密码输入框示例");
    QVBoxLayout *passwordLayout = new QVBoxLayout(passwordGroupBox);

    QLabel *passwordLabel = new QLabel("密码输入框支持显示/隐藏切换");
    passwordLabel->setStyleSheet("font-weight: bold;");
    passwordLayout->addWidget(passwordLabel);

    // 创建不同尺寸的密码输入框
    QHBoxLayout *passwordInputsLayout = new QHBoxLayout();

    // 小尺寸密码输入框
    QVBoxLayout *smallPasswordLayout = new QVBoxLayout();
    QLabel *smallLabel = new QLabel("小尺寸");
    Input *smallPasswordInput = new Input();
    smallPasswordInput->setPlaceholder("请输入密码");
    smallPasswordInput->setType(Input::Type::Password);
    smallPasswordInput->setSize(Input::Size::Sm);
    smallPasswordLayout->addWidget(smallLabel);
    smallPasswordLayout->addWidget(smallPasswordInput);

    // 默认尺寸密码输入框
    QVBoxLayout *defaultPasswordLayout = new QVBoxLayout();
    QLabel *defaultLabel = new QLabel("默认尺寸");
    Input *defaultPasswordInput = new Input();
    defaultPasswordInput->setPlaceholder("请输入密码");
    defaultPasswordInput->setType(Input::Type::Password);
    defaultPasswordLayout->addWidget(defaultLabel);
    defaultPasswordLayout->addWidget(defaultPasswordInput);

    // 大尺寸密码输入框
    QVBoxLayout *largePasswordLayout = new QVBoxLayout();
    QLabel *largeLabel = new QLabel("大尺寸");
    Input *largePasswordInput = new Input();
    largePasswordInput->setPlaceholder("请输入密码");
    largePasswordInput->setType(Input::Type::Password);
    largePasswordInput->setSize(Input::Size::Lg);
    largePasswordLayout->addWidget(largeLabel);
    largePasswordLayout->addWidget(largePasswordInput);

    // 普通文本输入框（对比）
    QVBoxLayout *regularTextLayout = new QVBoxLayout();
    QLabel *regularLabel = new QLabel("普通文本输入框（对比）");
    Input *regularTextInput = new Input();
    regularTextInput->setPlaceholder("这是普通文本输入框");
    regularTextInput->setType(Input::Type::Text); // 明确设置为文本类型
    regularTextLayout->addWidget(regularLabel);
    regularTextLayout->addWidget(regularTextInput);

    passwordInputsLayout->addLayout(smallPasswordLayout);
    passwordInputsLayout->addLayout(defaultPasswordLayout);
    passwordInputsLayout->addLayout(largePasswordLayout);
    passwordInputsLayout->addLayout(regularTextLayout);

    passwordLayout->addLayout(passwordInputsLayout);

    // 添加说明文本
    QLabel *passwordHelpText = new QLabel("点击密码输入框右侧的眼睛图标可以切换密码显示/隐藏状态");
    passwordHelpText->setStyleSheet("color: #666; margin-top: 10px;");
    passwordLayout->addWidget(passwordHelpText);

    mainLayout->addWidget(passwordGroupBox);

    // 4. 消息框示例区域
    QGroupBox *messageBoxGroupBox = new QGroupBox("消息框示例");
    QVBoxLayout *messageBoxLayout = new QVBoxLayout(messageBoxGroupBox);

    QLabel *messageBoxLabel = new QLabel("自定义样式的消息框");
    messageBoxLabel->setStyleSheet("font-weight: bold;");
    messageBoxLayout->addWidget(messageBoxLabel);

    // 创建不同类型的消息框按钮
    QHBoxLayout *messageBoxButtonsLayout = new QHBoxLayout();

    // 信息消息框
    Button *infoBtn = new Button("信息消息框");
    infoBtn->setVariant(Button::Variant::Default);
    connect(infoBtn, &Button::clicked, [this]() {
        CustomMessageBox::information(this, "信息", "这是一个信息消息框示例。\n自定义样式美观大方。");
    });

    // 警告消息框
    Button *warningBtn = new Button("警告消息框");
    warningBtn->setVariant(Button::Variant::Secondary);
    connect(warningBtn, &Button::clicked, [this]() {
        CustomMessageBox::warning(this, "警告", "这是一个警告消息框示例。\n请注意可能存在的风险。");
    });

    // 错误消息框
    Button *errorBtn = new Button("错误消息框");
    errorBtn->setVariant(Button::Variant::Destructive);
    connect(errorBtn, &Button::clicked, [this]() {
        CustomMessageBox::critical(this, "错误", "这是一个错误消息框示例。\n操作失败，请重试。");
    });

    // 确认消息框
    Button *confirmBtn = new Button("确认消息框");
    confirmBtn->setVariant(Button::Variant::Outline);
    connect(confirmBtn, &Button::clicked, [this]() {
        CustomMessageBox::StandardButton result = CustomMessageBox::question(
            this,
            "确认操作",
            "您确定要执行此操作吗？",
            CustomMessageBox::StandardButton(CustomMessageBox::Yes | CustomMessageBox::No)
        );

        if (result == CustomMessageBox::Yes) {
            CustomMessageBox::information(this, "结果", "您选择了\"是\"");
        } else {
            CustomMessageBox::information(this, "结果", "您选择了\"否\"");
        }
    });

    // 自定义消息框
    Button *customBtn = new Button("自定义消息框");
    customBtn->setVariant(Button::Variant::Ghost);
    connect(customBtn, &Button::clicked, [this]() {
        CustomMessageBox msgBox(CustomMessageBox::Question, "自定义消息框", "这是一个带有详细信息的自定义消息框。");
        msgBox.setInformativeText("您可以添加更多详细信息来解释此消息框的目的。\n这是第二行文本。");
        msgBox.setStandardButtons(CustomMessageBox::StandardButton(CustomMessageBox::Ok | CustomMessageBox::Cancel | CustomMessageBox::Help));
        msgBox.setDefaultButton(CustomMessageBox::Ok);

        CustomMessageBox::StandardButton result = msgBox.execMessageBox();

        QString resultText;
        switch (result) {
            case CustomMessageBox::Ok:
                resultText = "确定";
                break;
            case CustomMessageBox::Cancel:
                resultText = "取消";
                break;
            case CustomMessageBox::Help:
                resultText = "帮助";
                break;
            default:
                resultText = "未知";
                break;
        }

        CustomMessageBox::information(this, "结果", QString("您点击了\"%1\"按钮").arg(resultText));
    });

    // 自定义SVG图标消息框 - 静态方法
    Button *svgIconBtn = new Button("自定义SVG图标");
    svgIconBtn->setVariant(Button::Variant::Default);
    svgIconBtn->setSize(Button::Size::Sm);
    connect(svgIconBtn, &Button::clicked, [this]() {
        // 使用加载图标作为示例
        CustomMessageBox::customIcon(
            ":/icons/loading.svg",
            this,
            "自定义SVG图标",
            "这是一个使用自定义SVG图标的消息框示例。\n您可以使用任何SVG图标。",
            CustomMessageBox::Ok
        );
    });

    // 自定义SVG图标消息框 - 实例方法
    Button *svgIconInstanceBtn = new Button("自定义实例");
    svgIconInstanceBtn->setVariant(Button::Variant::Secondary);
    svgIconInstanceBtn->setSize(Button::Size::Sm);
    connect(svgIconInstanceBtn, &Button::clicked, [this]() {
        // 创建自定义消息框实例
        CustomMessageBox msgBox(":/icons/arrow.svg", "自定义实例", "这是使用实例方法创建的自定义图标消息框。",
                               CustomMessageBox::StandardButton(CustomMessageBox::Ok | CustomMessageBox::Cancel),
                               32, this);

        // 设置更多属性
        msgBox.setInformativeText("您可以在创建后设置更多属性，比如这段详细说明文本。");

        // 显示消息框并获取结果
        CustomMessageBox::StandardButton result = msgBox.execMessageBox();

        if (result == CustomMessageBox::Ok) {
            CustomMessageBox::information(this, "结果", "您点击了\"确定\"按钮");
        } else {
            CustomMessageBox::information(this, "结果", "您点击了\"取消\"按钮");
        }
    });

    messageBoxButtonsLayout->addWidget(infoBtn);
    messageBoxButtonsLayout->addWidget(warningBtn);
    messageBoxButtonsLayout->addWidget(errorBtn);
    messageBoxButtonsLayout->addWidget(confirmBtn);
    messageBoxButtonsLayout->addWidget(customBtn);
    messageBoxButtonsLayout->addWidget(svgIconBtn);
    messageBoxButtonsLayout->addWidget(svgIconInstanceBtn);

    messageBoxLayout->addLayout(messageBoxButtonsLayout);

    // 添加说明文本
    QLabel *messageBoxHelpText = new QLabel("点击按钮查看不同类型的消息框示例");
    messageBoxHelpText->setStyleSheet("color: #666; margin-top: 10px;");
    messageBoxLayout->addWidget(messageBoxHelpText);

    mainLayout->addWidget(messageBoxGroupBox);


    QGroupBox *tagSelectGroupBox = new QGroupBox("密码输入框示例");
    QVBoxLayout *tagSelectLayout = new QVBoxLayout(tagSelectGroupBox);
    TagSelect *tagSelect = new TagSelect;
    tagSelect->setOptions(QStringList() << "Option A" << "Option B" << "Option C" << "Option D" << "Option E");

    QLabel *label = new QLabel("已选项：");

    // 使用指针捕获而不是引用捕获
    QObject::connect(tagSelect, &TagSelect::selectionChanged, [label](const QStringList &items) {
        label->setText("已选项：" + items.join(", "));
    });

    tagSelectLayout->addWidget(tagSelect);
    tagSelectLayout->addWidget(label);
    mainLayout->addWidget(tagSelectGroupBox);

    // 返回按钮
    Button *backBtn = new Button(QIcon(":/icons/arrow.svg"), "返回登录页");
    backBtn->setVariant(Button::Variant::Ghost);
    QObject::connect(backBtn, &Button::clicked, [=](){
        emit routeTo("login");
    });
    mainLayout->addWidget(backBtn);
}
