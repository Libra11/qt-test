#ifndef BUTTON_H
#define BUTTON_H

#include <QPushButton>
#include <QString>
#include <QIcon>
#include <QMovie>
#include <QLabel>

class Button : public QPushButton {
    Q_OBJECT

public:
    enum class Variant {
        Default,
        Destructive,
        Outline,
        Secondary,
        Ghost,
        Link
    };

    enum class Size {
        Default,
        Sm,
        Lg,
        Icon
    };

    explicit Button(QWidget *parent = nullptr);
    explicit Button(const QString &text, QWidget *parent = nullptr);
    explicit Button(const QIcon &icon, QWidget *parent = nullptr);
    explicit Button(const QIcon &icon, const QString &text, QWidget *parent = nullptr);

    void setVariant(Variant variant);
    void setSize(Size size);
    void setDisabled(bool disabled);

    // Icon methods
    void setButtonIcon(const QIcon &icon);
    void setIconSize(const QSize &size);

    // Loading state methods
    void setLoading(bool loading);
    bool isLoading() const;

    // Reimplement QPushButton methods
    void setText(const QString &text);

private:
    void updateStyle();
    QString getVariantClass() const;
    QString getSizeClass() const;
    void setupLoadingAnimation();
    void updateLoadingAnimation();

    Variant m_variant = Variant::Default;
    Size m_size = Size::Default;
    bool m_disabled = false;
    bool m_loading = false;

    QString m_originalText;
    QIcon m_originalIcon;
    QLabel* m_loadingIconLabel = nullptr;
    QMovie* m_loadingAnimation = nullptr;
};

#endif // BUTTON_H