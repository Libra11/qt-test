/*
 * @Author: Libra
 * @Date: 2025-05-03 13:55:09
 * @LastEditors: Libra
 * @Description: Button component with icon and loading support
 */
#include "components/base/Button.h"
#include "components/base/ColorManager.h"
#include <QStyle>
#include <QHBoxLayout>
#include <QApplication>

Button::Button(QWidget *parent)
    : QPushButton(parent)
{
    setFixedHeight(50); // 默认高度 50px
    setupLoadingAnimation();
    updateStyle();
}

Button::Button(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
{
    m_originalText = text;
    setFixedHeight(50); // 默认高度 50px
    setupLoadingAnimation();
    updateStyle();
}

Button::Button(const QIcon &icon, QWidget *parent)
    : QPushButton(parent)
{
    m_originalIcon = icon;
    setIcon(icon);
    setFixedHeight(50);
    setupLoadingAnimation();
    updateStyle();
}

Button::Button(const QIcon &icon, const QString &text, QWidget *parent)
    : QPushButton(text, parent)
{
    m_originalText = text;
    m_originalIcon = icon;
    setIcon(icon);
    setFixedHeight(50);
    setupLoadingAnimation();
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

void Button::setButtonIcon(const QIcon &icon)
{
    m_originalIcon = icon;
    if (!m_loading) {
        setIcon(icon);
    }
    updateStyle();
}

void Button::setIconSize(const QSize &size)
{
    QPushButton::setIconSize(size);
}

void Button::setLoading(bool loading)
{
    if (m_loading == loading) {
        return;
    }

    m_loading = loading;

    if (loading) {
        // Save current text and icon
        if (m_originalText.isEmpty()) {
            m_originalText = text();
        }
        if (m_originalIcon.isNull() && !icon().isNull()) {
            m_originalIcon = icon();
        }

        // Show loading animation
        if (m_loadingAnimation) {
            m_loadingAnimation->start();
            if (m_loadingIconLabel) {
                m_loadingIconLabel->show();
            }
        }

        // Keep text but add space for the loading icon
        if (m_size != Size::Icon) {
            setText(" " + m_originalText); // Add space for the loading icon
        } else {
            setText("");
        }

        // Hide the original icon during loading
        setIcon(QIcon());
    } else {
        // Restore original text and icon
        setText(m_originalText);
        if (!m_originalIcon.isNull()) {
            setIcon(m_originalIcon);
        }

        // Stop loading animation
        if (m_loadingAnimation) {
            m_loadingAnimation->stop();
            if (m_loadingIconLabel) {
                m_loadingIconLabel->hide();
            }
        }
    }

    updateLoadingAnimation();
    updateStyle();
}

bool Button::isLoading() const
{
    return m_loading;
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

void Button::setupLoadingAnimation()
{
    // Create loading label if it doesn't exist
    if (!m_loadingIconLabel) {
        m_loadingIconLabel = new QLabel(this);
        m_loadingIconLabel->setFixedSize(16, 16);
        m_loadingIconLabel->setScaledContents(true);
        m_loadingIconLabel->hide(); // Hide by default

        // Create loading animation
        m_loadingAnimation = new QMovie(":/icons/loading.svg", QByteArray(), this);
        m_loadingAnimation->setScaledSize(QSize(16, 16));
        m_loadingIconLabel->setMovie(m_loadingAnimation);

        // Set the color of the loading icon to match the button text color
        QString styleSheet = QString("QLabel { background-color: transparent; }");
        m_loadingIconLabel->setStyleSheet(styleSheet);
    }
}

void Button::updateLoadingAnimation()
{
    if (!m_loadingIconLabel) {
        return;
    }

    if (m_loading) {
        // Position the loading icon
        QSize iconSize(16, 16);

        if (m_size == Size::Sm) {
            iconSize = QSize(12, 12);
        } else if (m_size == Size::Lg) {
            iconSize = QSize(20, 20);
        }

        m_loadingAnimation->setScaledSize(iconSize);
        m_loadingIconLabel->setFixedSize(iconSize);

        // Position the loading icon
        int x, y;

        if (m_size == Size::Icon) {
            // Center the icon for icon-only buttons
            x = (width() - iconSize.width()) / 2;
            y = (height() - iconSize.height()) / 2;
        } else {
            // Position the icon to the left of the text
            x = 12; // Left padding
            y = (height() - iconSize.height()) / 2;
        }

        m_loadingIconLabel->move(x, y);
        m_loadingIconLabel->show();
    } else {
        m_loadingIconLabel->hide();
    }
}
