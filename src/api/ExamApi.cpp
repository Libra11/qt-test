/*
 * @Author: Libra
 * @Date: 2025-05-06 15:43:32
 * @LastEditors: Libra
 * @Description: 
 */
/*
 * @Author: Libra
 * @Date: 2025-05-06 15:43:32
 * @LastEditors: Libra
 * @Description:
 */
#include "api/ExamApi.h"
#include "helpers/NetworkHelper.h"

void ExamApi::getLoginImageCode(std::function<void(QJsonObject)> onSuccess, std::function<void(QString)> onError)
{
    NetworkHelper::instance()->get(baseUrl + "other/validate/code", onSuccess, onError);
}

void ExamApi::login(const QString& loginCode, const QString& imageCode, const QString& key, std::function<void(QJsonObject)> onSuccess, std::function<void(QString)> onError)
{
    QJsonObject obj;
    obj["loginCode"] = loginCode;
    obj["imageCode"] = imageCode;
    obj["key"] = key;
    obj["version"] = "1.0.0";
    NetworkHelper::instance()->post(baseUrl + "client/login", obj, onSuccess, onError);
}

void ExamApi::getSiteInfo(std::function<void(QJsonObject)> onSuccess, std::function<void(QString)> onError)
{
    NetworkHelper::instance()->get(baseUrl + "client/site/info", onSuccess, onError);
}

void ExamApi::getRoomInfo(std::function<void(QJsonObject)> onSuccess, std::function<void(QString)> onError)
{
    NetworkHelper::instance()->get(baseUrl + "client/room/info", onSuccess, onError);
}