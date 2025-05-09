/*
 * @Author: Libra
 * @Date: 2025-05-10 16:30:00
 * @LastEditors: Libra
 * @Description: Custom styled message box component
 */
#ifndef CUSTOMMESSAGEBOX_H
#define CUSTOMMESSAGEBOX_H

#include <QDialog>
#include <QString>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QMap>
#include <functional>

// Forward declare Button class
class Button;

// Our custom MessageBox class
class CustomMessageBox : public QDialog {
    Q_OBJECT

public:
    // Match QMessageBox's standard buttons and icon types for easy migration
    enum StandardButton {
        NoButton           = 0,
        Ok                 = 0x00000400,
        Cancel             = 0x00000800,
        Yes                = 0x00004000,
        No                 = 0x00010000,
        Abort              = 0x00040000,
        Retry              = 0x00080000,
        Ignore             = 0x00100000,
        Close              = 0x00200000,
        Help               = 0x01000000,
        Apply              = 0x02000000,
        Reset              = 0x04000000,
        RestoreDefaults    = 0x08000000
    };

    enum Icon {
        NoIcon,
        Information,
        Warning,
        Critical,
        Question
    };

    // Static convenience methods (similar to QMessageBox)
    static StandardButton information(QWidget *parent, const QString &title,
                                     const QString &text, StandardButton buttons = Ok,
                                     StandardButton defaultButton = NoButton);

    static StandardButton question(QWidget *parent, const QString &title,
                                  const QString &text, StandardButton buttons = StandardButton(Yes | No),
                                  StandardButton defaultButton = NoButton);

    static StandardButton warning(QWidget *parent, const QString &title,
                                 const QString &text, StandardButton buttons = Ok,
                                 StandardButton defaultButton = NoButton);

    static StandardButton critical(QWidget *parent, const QString &title,
                                  const QString &text, StandardButton buttons = Ok,
                                  StandardButton defaultButton = NoButton);

    // Static convenience methods with custom icons
    static StandardButton customIcon(const QString &iconPath, QWidget *parent, const QString &title,
                                    const QString &text, StandardButton buttons = Ok,
                                    StandardButton defaultButton = NoButton, int iconSize = 48);

    // Instance methods
    explicit CustomMessageBox(QWidget *parent = nullptr);
    explicit CustomMessageBox(Icon icon, const QString &title, const QString &text,
                       StandardButton buttons = Ok, QWidget *parent = nullptr);
    explicit CustomMessageBox(const QString &iconPath, const QString &title, const QString &text,
                       StandardButton buttons = Ok, int iconSize = 48, QWidget *parent = nullptr);

    void setIcon(Icon icon);
    void setCustomIcon(const QString &iconPath, int size = 48);
    void setText(const QString &text);
    void setInformativeText(const QString &text);
    void setTitle(const QString &title);
    void setStandardButtons(StandardButton buttons);
    void setDefaultButton(StandardButton button);

    StandardButton standardButton(Button *button) const;
    Button *button(StandardButton which) const;

    // Execute and return the clicked standard button
    StandardButton execMessageBox();

private:
    void setupUi();
    void createButtons(StandardButton buttons);
    Button* createButton(StandardButton standardButton);
    QPixmap standardIcon(Icon icon) const;
    QString buttonText(StandardButton button) const;

    Icon m_icon;
    QString m_text;
    QString m_informativeText;
    StandardButton m_standardButtons;
    StandardButton m_defaultButton;
    StandardButton m_clickedButton;

    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_contentLayout;
    QHBoxLayout *m_buttonLayout;
    QLabel *m_iconLabel;
    QLabel *m_textLabel;
    QLabel *m_informativeTextLabel;

    QMap<StandardButton, Button*> m_buttonMap;

private slots:
    void buttonClicked();
};

// Allow StandardButton to be used with bitwise operators
inline CustomMessageBox::StandardButton operator|(CustomMessageBox::StandardButton a, CustomMessageBox::StandardButton b)
{
    return static_cast<CustomMessageBox::StandardButton>(static_cast<int>(a) | static_cast<int>(b));
}

#endif // CUSTOMMESSAGEBOX_H
