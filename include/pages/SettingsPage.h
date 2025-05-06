#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include "components/base/Button.h"
#include "components/form/Form.h"
#include "components/form/FormItem.h"

class SettingsPage : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsPage(QWidget *parent = nullptr);

signals:
    void backToHome();

private:
    void setupUI();
    void setupConnections();
};

#endif // SETTINGSPAGE_H 