/*
 * @Author: Libra
 * @Date: 2025-05-06 16:39:22
 * @LastEditors: Libra
 * @Description:
 */
#include "helpers/SettingsHelper.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

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

void SettingsHelper::saveSiteInfo(const ISite& site)
{
    QJsonObject siteObj;
    siteObj["siteId"] = site.siteId;
    siteObj["name"] = site.name;
    siteObj["province"] = site.province;
    siteObj["city"] = site.city;
    siteObj["priority"] = site.priority;
    siteObj["district"] = site.district;
    siteObj["address"] = site.address;
    siteObj["traffic"] = site.traffic;
    siteObj["manager"] = site.manager;
    siteObj["managerMobile"] = site.managerMobile;
    siteObj["itContract"] = site.itContract;
    siteObj["itMobile"] = site.itMobile;
    siteObj["note"] = site.note;
    siteObj["locked"] = site.locked;

    QJsonDocument doc(siteObj);
    setValue("siteInfo", doc.toJson(QJsonDocument::Compact));
}

ISite SettingsHelper::getSiteInfo()
{
    ISite site;
    QByteArray jsonData = value("siteInfo").toByteArray();

    if (!jsonData.isEmpty()) {
        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            site.siteId = obj["siteId"].toString();
            site.name = obj["name"].toString();
            site.province = obj["province"].toString();
            site.city = obj["city"].toString();
            site.priority = obj["priority"].toInt();
            site.district = obj["district"].toString();
            site.address = obj["address"].toString();
            site.traffic = obj["traffic"].toString();
            site.manager = obj["manager"].toString();
            site.managerMobile = obj["managerMobile"].toString();
            site.itContract = obj["itContract"].toString();
            site.itMobile = obj["itMobile"].toString();
            site.note = obj["note"].toString();
            site.locked = obj["locked"].toBool();
        }
    }

    return site;
}

void SettingsHelper::saveRoomInfo(const IRoom& room)
{
    QJsonObject roomObj;
    roomObj["siteId"] = room.siteId;
    roomObj["roomId"] = room.roomId;
    roomObj["name"] = room.name;
    roomObj["sort"] = room.sort;
    roomObj["address"] = room.address;
    roomObj["note"] = room.note;
    roomObj["capacity"] = room.capacity;
    roomObj["available"] = room.available;
    roomObj["loginCode"] = room.loginCode;
    roomObj["cloudSystem"] = room.cloudSystem;
    roomObj["localCamera"] = room.localCamera;

    QJsonDocument doc(roomObj);
    setValue("roomInfo", doc.toJson(QJsonDocument::Compact));
}

IRoom SettingsHelper::getRoomInfo()
{
    IRoom room;
    QByteArray jsonData = value("roomInfo").toByteArray();

    if (!jsonData.isEmpty()) {
        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            room.siteId = obj["siteId"].toString();
            room.roomId = obj["roomId"].toString();
            room.name = obj["name"].toString();
            room.sort = obj["sort"].toInt();
            room.address = obj["address"].toString();
            room.note = obj["note"].toString();
            room.capacity = obj["capacity"].toInt();
            room.available = obj["available"].toInt();
            room.loginCode = obj["loginCode"].toString();
            room.cloudSystem = obj["cloudSystem"].toBool();
            room.localCamera = obj["localCamera"].toBool();
        }
    }

    return room;
}
