#ifndef EXAMAPI_H
#define EXAMAPI_H

#include <QObject>
#include <QJsonObject>
#include <functional>

namespace ExamApi {
    const QString baseUrl = "https://supernova-api.iguokao.com/exam/api/v1/";

    // 获取验证码
    void getLoginImageCode(std::function<void(QJsonObject)> onSuccess, std::function<void(QString)> onError = nullptr);

    // 登录
    void login(const QString& loginCode, const QString& imageCode, const QString& key, std::function<void(QJsonObject)> onSuccess, std::function<void(QString)> onError = nullptr);
}

#endif // EXAMAPI_H 