/*
 * @Author: Libra
 * @Date: 2025-05-06 13:48:43
 * @LastEditors: Libra
 * @Description: 
 */
#ifndef INPUT_H
#define INPUT_H

#include <QLineEdit>
#include <QString>
#include <QAction>

class Input : public QLineEdit {
    Q_OBJECT

public:
    enum class Size {
        Default,
        Sm,
        Lg
    };

    enum class Type {
        Text,
        Password
    };

    explicit Input(QWidget *parent = nullptr);
    explicit Input(const QString &text, QWidget *parent = nullptr);

    void setSize(Size size);
    void setDisabled(bool disabled);
    void setError(bool error);
    void setPlaceholder(const QString &placeholder);

    // Password related methods
    void setType(Type type);
    Type getType() const;
    void setPasswordVisible(bool visible);
    bool isPasswordVisible() const;

private slots:
    void togglePasswordVisibility();

private:
    void updateStyle();
    QString getSizeClass() const;
    void setupPasswordToggle();

    Size m_size = Size::Default;
    Type m_type = Type::Text;
    bool m_disabled = false;
    bool m_error = false;
    bool m_passwordVisible = false;
    QAction *m_togglePasswordAction = nullptr;
};

#endif // INPUT_H