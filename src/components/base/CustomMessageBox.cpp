/*
 * @Author: Libra
 * @Date: 2025-05-10 16:30:00
 * @LastEditors: Libra
 * @Description: Custom styled message box component
 */
#include "components/base/CustomMessageBox.h"
#include "components/base/Button.h"
#include "components/base/ColorManager.h"
#include <QApplication>
#include <QStyle>
#include <QScreen>
#include <QGuiApplication>
#include <QSvgRenderer>
#include <QPainter>

CustomMessageBox::CustomMessageBox(QWidget *parent)
    : QDialog(parent),
      m_icon(NoIcon),
      m_standardButtons(Ok),
      m_defaultButton(NoButton),
      m_clickedButton(NoButton)
{
    setupUi();
}

CustomMessageBox::CustomMessageBox(Icon icon, const QString &title, const QString &text,
                     StandardButton buttons, QWidget *parent)
    : QDialog(parent),
      m_icon(icon),
      m_text(text),
      m_standardButtons(buttons),
      m_defaultButton(NoButton),
      m_clickedButton(NoButton)
{
    setupUi();
    setWindowTitle(title);
}

CustomMessageBox::CustomMessageBox(const QString &iconPath, const QString &title, const QString &text,
                     StandardButton buttons, int iconSize, QWidget *parent)
    : QDialog(parent),
      m_icon(NoIcon),
      m_text(text),
      m_standardButtons(buttons),
      m_defaultButton(NoButton),
      m_clickedButton(NoButton)
{
    setupUi();
    setWindowTitle(title);
    setCustomIcon(iconPath, iconSize);
}

void CustomMessageBox::setupUi()
{
    // Set window flags
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // Create layouts
    m_mainLayout = new QVBoxLayout(this);
    m_contentLayout = new QHBoxLayout();
    m_buttonLayout = new QHBoxLayout();

    // Create widgets
    m_iconLabel = new QLabel(this);
    m_iconLabel->setFixedSize(48, 48);
    m_iconLabel->setScaledContents(true);

    m_textLabel = new QLabel(m_text, this);
    m_textLabel->setWordWrap(true);
    m_textLabel->setTextFormat(Qt::RichText);
    QFont textFont = m_textLabel->font();
    textFont.setPointSize(textFont.pointSize() + 1);
    textFont.setBold(true);
    m_textLabel->setFont(textFont);

    m_informativeTextLabel = new QLabel(m_informativeText, this);
    m_informativeTextLabel->setWordWrap(true);
    m_informativeTextLabel->setTextFormat(Qt::RichText);
    m_informativeTextLabel->setVisible(!m_informativeText.isEmpty());

    // Create text layout
    QVBoxLayout *textLayout = new QVBoxLayout();
    textLayout->addWidget(m_textLabel);
    textLayout->addWidget(m_informativeTextLabel);
    textLayout->addStretch();

    // Add widgets to content layout
    m_contentLayout->addWidget(m_iconLabel);
    m_contentLayout->addLayout(textLayout, 1);

    // Add layouts to main layout
    m_mainLayout->addLayout(m_contentLayout);
    m_mainLayout->addSpacing(20);
    m_mainLayout->addLayout(m_buttonLayout);

    // Set icon
    setIcon(m_icon);

    // Create buttons
    createButtons(m_standardButtons);

    // Set minimum size
    setMinimumWidth(400);

    // Style the dialog
    setStyleSheet(QString(
        "QDialog {"
        "    background-color: %1;"
        "    border-radius: 8px;"
        "}"
        "QLabel {"
        "    color: %2;"
        "}"
    ).arg(ColorManager::white(), ColorManager::black()));
}

void CustomMessageBox::setIcon(Icon icon)
{
    m_icon = icon;
    m_iconLabel->setPixmap(standardIcon(icon));
    m_iconLabel->setVisible(icon != NoIcon);
}

void CustomMessageBox::setCustomIcon(const QString &iconPath, int size)
{
    // 重置内置图标
    m_icon = NoIcon;

    // 加载SVG图标
    QPixmap pixmap;
    if (iconPath.endsWith(".svg", Qt::CaseInsensitive)) {
        QSvgRenderer renderer(iconPath);
        pixmap = QPixmap(size, size);
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        renderer.render(&painter);
    } else {
        // 如果不是SVG，尝试作为普通图像加载
        pixmap.load(iconPath);
        pixmap = pixmap.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    if (!pixmap.isNull()) {
        m_iconLabel->setPixmap(pixmap);
        m_iconLabel->setFixedSize(size, size);
        m_iconLabel->setVisible(true);
    }
}

void CustomMessageBox::setText(const QString &text)
{
    m_text = text;
    m_textLabel->setText(text);
}

void CustomMessageBox::setInformativeText(const QString &text)
{
    m_informativeText = text;
    m_informativeTextLabel->setText(text);
    m_informativeTextLabel->setVisible(!text.isEmpty());
}

void CustomMessageBox::setTitle(const QString &title)
{
    setWindowTitle(title);
}

void CustomMessageBox::setStandardButtons(StandardButton buttons)
{
    // Clear existing buttons
    for (auto button : m_buttonMap.values()) {
        m_buttonLayout->removeWidget(button);
        delete button;
    }
    m_buttonMap.clear();

    m_standardButtons = buttons;
    createButtons(buttons);
}

void CustomMessageBox::setDefaultButton(StandardButton button)
{
    m_defaultButton = button;

    if (m_buttonMap.contains(button)) {
        m_buttonMap[button]->setFocus();
    }
}

CustomMessageBox::StandardButton CustomMessageBox::standardButton(Button *button) const
{
    return m_buttonMap.key(button, NoButton);
}

Button *CustomMessageBox::button(StandardButton which) const
{
    return m_buttonMap.value(which, nullptr);
}

CustomMessageBox::StandardButton CustomMessageBox::execMessageBox()
{
    // Center the dialog on the screen
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);

    // Execute the dialog
    QDialog::exec();
    return m_clickedButton;
}

void CustomMessageBox::createButtons(StandardButton buttons)
{
    // Add spacer to push buttons to the right
    m_buttonLayout->addStretch();

    // Create buttons in the standard order
    const StandardButton buttonList[] = {
        Help, Ok, Yes, No, Abort, Retry, Ignore,
        Cancel, Close, Apply, Reset, RestoreDefaults
    };

    for (StandardButton standardButton : buttonList) {
        if (buttons & standardButton) {
            Button *button = createButton(standardButton);
            m_buttonLayout->addWidget(button);
            m_buttonMap[standardButton] = button;

            // Set default button
            if (m_defaultButton == NoButton) {
                // If no default button specified, use the first one
                m_defaultButton = standardButton;
                button->setFocus();
            } else if (m_defaultButton == standardButton) {
                button->setFocus();
            }
        }
    }
}

Button *CustomMessageBox::createButton(StandardButton standardButton)
{
    Button *button = new Button(buttonText(standardButton), this);

    // Style the button based on its role
    switch (standardButton) {
        case Ok:
        case Yes:
        case Apply:
            button->setVariant(Button::Variant::Default);
            break;
        case Cancel:
        case Close:
            button->setVariant(Button::Variant::Outline);
            break;
        case No:
        case Abort:
            button->setVariant(Button::Variant::Destructive);
            break;
        default:
            button->setVariant(Button::Variant::Secondary);
            break;
    }

    // Set size
    button->setSize(Button::Size::Sm);

    // Connect button click
    connect(button, &Button::clicked, this, &CustomMessageBox::buttonClicked);

    return button;
}

void CustomMessageBox::buttonClicked()
{
    Button *clickedButton = qobject_cast<Button*>(sender());
    if (clickedButton) {
        m_clickedButton = standardButton(clickedButton);
        accept();
    }
}

QPixmap CustomMessageBox::standardIcon(Icon icon) const
{
    QStyle::StandardPixmap pixmap;

    switch (icon) {
        case Information:
            pixmap = QStyle::SP_MessageBoxInformation;
            break;
        case Warning:
            pixmap = QStyle::SP_MessageBoxWarning;
            break;
        case Critical:
            pixmap = QStyle::SP_MessageBoxCritical;
            break;
        case Question:
            pixmap = QStyle::SP_MessageBoxQuestion;
            break;
        default:
            return QPixmap();
    }

    return QApplication::style()->standardPixmap(pixmap);
}

QString CustomMessageBox::buttonText(StandardButton button) const
{
    switch (button) {
        case Ok:
            return tr("确定");
        case Cancel:
            return tr("取消");
        case Yes:
            return tr("是");
        case No:
            return tr("否");
        case Abort:
            return tr("中止");
        case Retry:
            return tr("重试");
        case Ignore:
            return tr("忽略");
        case Close:
            return tr("关闭");
        case Help:
            return tr("帮助");
        case Apply:
            return tr("应用");
        case Reset:
            return tr("重置");
        case RestoreDefaults:
            return tr("恢复默认");
        default:
            return QString();
    }
}

// Static convenience methods
CustomMessageBox::StandardButton CustomMessageBox::information(QWidget *parent, const QString &title,
                                                 const QString &text, StandardButton buttons,
                                                 StandardButton defaultButton)
{
    CustomMessageBox msgBox(Information, title, text, buttons, parent);
    msgBox.setDefaultButton(defaultButton);
    return msgBox.execMessageBox();
}

CustomMessageBox::StandardButton CustomMessageBox::question(QWidget *parent, const QString &title,
                                              const QString &text, StandardButton buttons,
                                              StandardButton defaultButton)
{
    CustomMessageBox msgBox(Question, title, text, buttons, parent);
    msgBox.setDefaultButton(defaultButton);
    return msgBox.execMessageBox();
}

CustomMessageBox::StandardButton CustomMessageBox::warning(QWidget *parent, const QString &title,
                                             const QString &text, StandardButton buttons,
                                             StandardButton defaultButton)
{
    CustomMessageBox msgBox(Warning, title, text, buttons, parent);
    msgBox.setDefaultButton(defaultButton);
    return msgBox.execMessageBox();
}

CustomMessageBox::StandardButton CustomMessageBox::critical(QWidget *parent, const QString &title,
                                              const QString &text, StandardButton buttons,
                                              StandardButton defaultButton)
{
    CustomMessageBox msgBox(Critical, title, text, buttons, parent);
    msgBox.setDefaultButton(defaultButton);
    return msgBox.execMessageBox();
}

CustomMessageBox::StandardButton CustomMessageBox::customIcon(const QString &iconPath, QWidget *parent,
                                              const QString &title, const QString &text,
                                              StandardButton buttons, StandardButton defaultButton,
                                              int iconSize)
{
    CustomMessageBox msgBox(iconPath, title, text, buttons, iconSize, parent);
    msgBox.setDefaultButton(defaultButton);
    return msgBox.execMessageBox();
}
