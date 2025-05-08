/*
 * @Author: Libra
 * @Date: 2025-05-06 16:39:22
 * @LastEditors: Libra
 * @Description: 
 */
#include "helpers/SettingsHelper.h"

QSettings& SettingsHelper::instance()
{
    static QSettings settings("YourCompany", "YourApp");
    return settings;
}

void SettingsHelper::setValue(const QString& key, const QVariant& value)
{
    instance().setValue(key, value);
}

QVariant SettingsHelper::value(const QString& key, const QVariant& defaultValue)
{
    return instance().value(key, defaultValue);
}
