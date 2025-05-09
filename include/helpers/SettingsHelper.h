#ifndef SETTINGSHELPER_H
#define SETTINGSHELPER_H

#include <QSettings>
#include <QString>
#include <QJsonObject>
#include "api/ExamApi.h"

class SettingsHelper
{
public:
    static QSettings& instance();

    static void setValue(const QString& key, const QVariant& value);
    static QVariant value(const QString& key, const QVariant& defaultValue = QVariant());

    // 存储考点信息
    static void saveSiteInfo(const ISite& site);
    // 获取考点信息
    static ISite getSiteInfo();

    // 存储考场信息
    static void saveRoomInfo(const IRoom& room);
    // 获取考场信息
    static IRoom getRoomInfo();
};

#endif // SETTINGSHELPER_H