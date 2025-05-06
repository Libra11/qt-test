#ifndef SETTINGSHELPER_H
#define SETTINGSHELPER_H

#include <QSettings>
#include <QString>

class SettingsHelper
{
public:
    static QSettings& instance();

    static void setValue(const QString& key, const QVariant& value);
    static QVariant value(const QString& key, const QVariant& defaultValue = QVariant());
};

#endif // SETTINGSHELPER_H