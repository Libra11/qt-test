#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>
#include "PageBase.h"
#include "components/form/Form.h"

class LoginPage : public PageBase
{
    Q_OBJECT
public:
    explicit LoginPage(QWidget *parent = nullptr);

private:
    QString m_imageKey;
    Form* form;
};

#endif // LOGINPAGE_H 
