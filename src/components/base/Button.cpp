/*
 * @Author: Libra
 * @Date: 2025-05-03 13:55:09
 * @LastEditors: Libra
 * @Description: 
 */
#include "components/base/Button.h"
#include "components/base/ColorManager.h"
#include <QStyle>

Button::Button(QWidget *parent)
    : QPushButton(parent)
{
    setFixedHeight(50); // 默认高度 40px
    updateStyle();
}

Button::Button(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
{
    setFixedHeight(50); // 默认高度 40px
    updateStyle();
}

void Button::setVariant(Variant variant)
{
    m_variant = variant;
    updateStyle();
}

void Button::setSize(Size size)
{
    m_size = size;
    updateStyle();
}

void Button::setDisabled(bool disabled)
{
    m_disabled = disabled;
    QPushButton::setEnabled(!disabled);
    updateStyle();
}

void Button::updateStyle()
{
    QString baseStyle = QString(
        "QPushButton {"
        "    border-radius: 6px;"
        "    font-weight: 500;"
        "    %1"
        "    %2"
        "}"
        "QPushButton:disabled {"
        "    opacity: 0.5;"
        "}"
    ).arg(getVariantClass(), getSizeClass());

    QString hoverStyle;
    switch (m_variant) {
        case Variant::Default:
            hoverStyle = QString(
                "QPushButton:hover {"
                "    background-color: %1;"
                "    color: %2;"
                "}"
            ).arg(ColorManager::primaryHover(), ColorManager::white());
            break;
        case Variant::Destructive:
            hoverStyle = QString(
                "QPushButton:hover {"
                "    background-color: %1;"
                "    color: %2;"
                "}"
            ).arg(ColorManager::destructiveHover(), ColorManager::white());
            break;
        case Variant::Outline:
            hoverStyle = QString(
                "QPushButton:hover {"
                "    background-color: %1;"
                "    color: %2;"
                "    border: 1px solid %3;"
                "}"
            ).arg(ColorManager::outlineHoverBg(), ColorManager::primary(), ColorManager::outlineHoverBorder());
            break;
        case Variant::Secondary:
            hoverStyle = QString(
                "QPushButton:hover {"
                "    background-color: %1;"
                "    color: %2;"
                "}"
            ).arg(ColorManager::secondaryHoverBg(), ColorManager::primary());
            break;
        case Variant::Ghost:
            hoverStyle = QString(
                "QPushButton:hover {"
                "    background-color: %1;"
                "    color: %2;"
                "}"
            ).arg(ColorManager::ghostHoverBg(), ColorManager::primary());
            break;
        case Variant::Link:
            hoverStyle = QString(
                "QPushButton:hover {"
                "    color: %1;"
                "    text-decoration: underline;"
                "}"
            ).arg(ColorManager::linkHover());
            break;
        default:
            hoverStyle = "";
    }

    setStyleSheet(baseStyle + hoverStyle);
}

QString Button::getVariantClass() const
{
    switch (m_variant) {
        case Variant::Default:
            return QString("background-color: %1; color: %2; border: none;")
                .arg(ColorManager::primary(), ColorManager::white());
        case Variant::Destructive:
            return QString("background-color: %1; color: %2; border: none;")
                .arg(ColorManager::destructive(), ColorManager::white());
        case Variant::Outline:
            return QString("background-color: transparent; color: %1; border: 1px solid %2;")
                .arg(ColorManager::primary(), ColorManager::outline());
        case Variant::Secondary:
            return QString("background-color: %1; color: %2; border: none;")
                .arg(ColorManager::secondaryBg(), ColorManager::primary());
        case Variant::Ghost:
            return QString("background-color: transparent; color: %1; border: none;")
                .arg(ColorManager::primary());
        case Variant::Link:
            return QString("background-color: transparent; color: %1; border: none; text-decoration: underline;")
                .arg(ColorManager::primary());
        default:
            return "";
    }
}

QString Button::getSizeClass() const
{
    switch (m_size) {
        case Size::Default:
            return "padding: 8px 16px; font-size: 16px;";
        case Size::Sm:
            return "padding: 6px 12px; font-size: 14px;";
        case Size::Lg:
            return "padding: 10px 20px; font-size: 18px;";
        case Size::Icon:
            return "padding: 8px; font-size: 16px; min-width: 32px; min-height: 32px;";
        default:
            return "";
    }
} 
