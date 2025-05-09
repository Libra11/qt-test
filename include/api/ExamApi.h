#ifndef EXAMAPI_H
#define EXAMAPI_H

#include <QObject>
#include <QJsonObject>
#include <functional>

// 考点信息接口
struct ISite {
    QString siteId;
    QString name;
    QString province;
    QString city;
    int priority;
    QString district;
    QString address;
    QString traffic;
    QString manager;
    QString managerMobile;
    QString itContract;
    QString itMobile;
    QString note;
    bool locked;
};

// 考场信息接口
struct IRoom {
    QString siteId;
    QString roomId;
    QString name;
    int sort;
    QString address;
    QString note;
    int capacity;
    int available;
    QString loginCode;
    bool cloudSystem;
    bool localCamera;
};

namespace ExamApi {
    const QString baseUrl = "https://supernova-api.iguokao.com/exam/api/v1/";

    // 获取验证码
    void getLoginImageCode(std::function<void(QJsonObject)> onSuccess, std::function<void(QString)> onError = nullptr);

    // 登录
    void login(const QString& loginCode, const QString& imageCode, const QString& key, std::function<void(QJsonObject)> onSuccess, std::function<void(QString)> onError = nullptr);

    // 获取考点信息
    void getSiteInfo(std::function<void(QJsonObject)> onSuccess, std::function<void(QString)> onError = nullptr);

    // 获取考场信息
    void getRoomInfo(std::function<void(QJsonObject)> onSuccess, std::function<void(QString)> onError = nullptr);
}

#endif // EXAMAPI_H