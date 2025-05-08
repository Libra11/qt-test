#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include "components/base/Button.h"
#include "components/form/Form.h"
#include "components/form/FormItem.h"
#include "PageBase.h"

class SettingsPage : public PageBase
{
    Q_OBJECT
public:
    explicit SettingsPage(QWidget *parent = nullptr);

private:
    void setupUI();
    void setupConnections();
};

#endif // SETTINGSPAGE_H 
