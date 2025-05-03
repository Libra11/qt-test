#include "components/base/Input.h"
#include "components/base/ColorManager.h"
#include <QStyle>

Input::Input(QWidget *parent)
    : QLineEdit(parent)
{
    updateStyle();
}

Input::Input(const QString &text, QWidget *parent)
    : QLineEdit(text, parent)
{
    updateStyle();
}

void Input::setSize(Size size)
{
    m_size = size;
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
