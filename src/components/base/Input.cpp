/*
 * @Author: Libra
 * @Date: 2025-05-06 13:48:43
 * @LastEditors: Libra
 * @Description:
 */
#include "components/base/Input.h"
#include "components/base/ColorManager.h"
#include <QStyle>
#include <QIcon>

Input::Input(QWidget *parent)
    : QLineEdit(parent), m_togglePasswordAction(nullptr)
{
    // Initialize as a regular text input by default
    m_type = Type::Text;
    updateStyle();
}

Input::Input(const QString &text, QWidget *parent)
    : QLineEdit(text, parent), m_togglePasswordAction(nullptr)
{
    // Initialize as a regular text input by default
    m_type = Type::Text;
    updateStyle();
}

void Input::setSize(Size size)
{
    if (m_size == size) {
        return;
    }

    m_size = size;

    // If this is a password input, we need to recreate the toggle action with the new size
    if (m_type == Type::Password) {
        // Save the current state
        bool wasVisible = m_passwordVisible;

        // Remove the old action
        if (m_togglePasswordAction) {
            removeAction(m_togglePasswordAction);
            delete m_togglePasswordAction;
            m_togglePasswordAction = nullptr;
        }

        // Create a new action with the appropriate size
        m_togglePasswordAction = new QAction(this);
        m_togglePasswordAction->setIcon(QIcon(wasVisible ? ":/icons/eye-off.svg" : ":/icons/eye.svg"));

        // Add the action
        addAction(m_togglePasswordAction, QLineEdit::TrailingPosition);
        connect(m_togglePasswordAction, &QAction::triggered, this, &Input::togglePasswordVisibility);
    }

    updateStyle();
}

void Input::setDisabled(bool disabled)
{
    m_disabled = disabled;
    QLineEdit::setEnabled(!disabled);
    updateStyle();
}

void Input::setError(bool error)
{
    m_error = error;
    updateStyle();
}

void Input::setPlaceholder(const QString &placeholder)
{
    QLineEdit::setPlaceholderText(placeholder);
}

void Input::setType(Type type)
{
    if (m_type == type) {
        return;
    }

    m_type = type;

    // Remove any existing toggle action
    if (m_togglePasswordAction) {
        removeAction(m_togglePasswordAction);
        delete m_togglePasswordAction;
        m_togglePasswordAction = nullptr;
    }

    if (m_type == Type::Password) {
        // Set password echo mode
        setEchoMode(m_passwordVisible ? QLineEdit::Normal : QLineEdit::Password);

        // Create and add the toggle action only for password inputs
        m_togglePasswordAction = new QAction(this);
        m_togglePasswordAction->setIcon(QIcon(":/icons/eye.svg"));

        // Determine appropriate icon size based on input size
        QSize iconSize;
        switch (m_size) {
            case Size::Sm:
                iconSize = QSize(16, 16);
                break;
            case Size::Lg:
                iconSize = QSize(24, 24);
                break;
            default:
                iconSize = QSize(20, 20);
                break;
        }

        // Add the action to the input
        addAction(m_togglePasswordAction, QLineEdit::TrailingPosition);
        connect(m_togglePasswordAction, &QAction::triggered, this, &Input::togglePasswordVisibility);
    } else {
        // Normal text input
        setEchoMode(QLineEdit::Normal);
    }

    updateStyle();
}

Input::Type Input::getType() const
{
    return m_type;
}

void Input::setPasswordVisible(bool visible)
{
    if (m_passwordVisible == visible || m_type != Type::Password) {
        return;
    }

    m_passwordVisible = visible;
    setEchoMode(m_passwordVisible ? QLineEdit::Normal : QLineEdit::Password);

    // Update the toggle icon
    if (m_togglePasswordAction) {
        m_togglePasswordAction->setIcon(QIcon(m_passwordVisible ? ":/icons/eye-off.svg" : ":/icons/eye.svg"));
    }
}

bool Input::isPasswordVisible() const
{
    return m_passwordVisible;
}

void Input::togglePasswordVisibility()
{
    setPasswordVisible(!m_passwordVisible);
}

// This method is no longer needed as we create the toggle action directly in setType
void Input::setupPasswordToggle()
{
    // Empty implementation - kept for backward compatibility
}

void Input::updateStyle()
{
    QString styleSheet = QString(
        "QLineEdit {"
        "    border-radius: 6px;"
        "    border: 1px solid %1;"
        "    background-color: %2;"
        "    padding: 8px 12px;"
        "    font-size: 16px;"
        "    %3"
        "}"
        "QLineEdit:focus {"
        "    border-color: %4;"
        "    outline: none;"
        "}"
        "QLineEdit:disabled {"
        "    background-color: %5;"
        "}"
    )
    .arg(m_error ? ColorManager::destructive() : ColorManager::outline(),
         ColorManager::white(),
         getSizeClass(),
         ColorManager::primary(),
         ColorManager::secondaryBg());

    setStyleSheet(styleSheet);

    // Only adjust margins for password inputs
    if (m_type == Type::Password && m_togglePasswordAction) {
        // Determine appropriate padding based on input size
        int iconWidth;
        switch (m_size) {
            case Size::Sm:
                iconWidth = 16;
                break;
            case Size::Lg:
                iconWidth = 24;
                break;
            default:
                iconWidth = 20;
                break;
        }

        // Add right margin to make room for the eye icon (icon width + padding)
        QLineEdit::setTextMargins(4, 8, iconWidth + 16, 8);
    } else {
        // Standard margins for regular text inputs
        QLineEdit::setTextMargins(4, 8, 12, 8);
    }
}

QString Input::getSizeClass() const
{
    switch (m_size) {
        case Size::Default:
            return "height: 40px;";
        case Size::Sm:
            return "height: 32px; font-size: 12px;";
        case Size::Lg:
            return "height: 48px; font-size: 16px;";
        default:
            return "";
    }
}
