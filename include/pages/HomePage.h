#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include "components/IME/IMESelectorWidget.h"
#include "helpers/AdminHelper.h"
#include "helpers/HostsHelper.h"
#include "helpers/ContentProtectionHelper.h"
#include "helpers/SystemControlHelper.h"
#include "helpers/UserHelper.h"
#include "components/base/Button.h"
#include "components/form/Form.h"
#include "components/form/FormItem.h"

class HomePage : public QWidget
{
    Q_OBJECT
public:
    explicit HomePage(QWidget *parent = nullptr);

private:
    void setupUI();
    void setupConnections();
};

#endif // HOMEPAGE_H 