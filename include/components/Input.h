#ifndef INPUT_H
#define INPUT_H

#include <QLineEdit>
#include <QString>

class Input : public QLineEdit {
    Q_OBJECT

public:
    enum class Size {
        Default,
        Sm,
        Lg
    };

    explicit Input(QWidget *parent = nullptr);
    explicit Input(const QString &text, QWidget *parent = nullptr);

    void setSize(Size size);
    void setDisabled(bool disabled);
    void setError(bool error);
    void setPlaceholder(const QString &placeholder);

private:
    void updateStyle();
    QString getSizeClass() const;

    Size m_size = Size::Default;
    bool m_disabled = false;
    bool m_error = false;
};

#endif // INPUT_H 