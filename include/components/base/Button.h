#ifndef BUTTON_H
#define BUTTON_H

#include <QPushButton>
#include <QString>

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

    void setVariant(Variant variant);
    void setSize(Size size);
    void setDisabled(bool disabled);

private:
    void updateStyle();
    QString getVariantClass() const;
    QString getSizeClass() const;

    Variant m_variant = Variant::Default;
    Size m_size = Size::Default;
    bool m_disabled = false;
};

#endif // BUTTON_H 