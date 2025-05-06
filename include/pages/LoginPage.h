#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>

class LoginPage : public QWidget
{
    Q_OBJECT
public:
    explicit LoginPage(QWidget *parent = nullptr);

signals:
    void loginSuccess();

private:
    QString m_imageKey;
};

#endif // LOGINPAGE_H 