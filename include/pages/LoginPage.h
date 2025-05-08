#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>
#include "PageBase.h"

class LoginPage : public PageBase
{
    Q_OBJECT
public:
    explicit LoginPage(QWidget *parent = nullptr);

private:
    QString m_imageKey;
};

#endif // LOGINPAGE_H 
