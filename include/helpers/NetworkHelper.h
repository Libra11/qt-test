/*
 * @Author: Libra
 * @Date: 2025-05-06 15:42:21
 * @LastEditors: Libra
 * @Description: 
 */
#ifndef NETWORKHELPER_H
#define NETWORKHELPER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrlQuery>
#include <functional>

class NetworkHelper : public QObject
{
    Q_OBJECT
public:
    static NetworkHelper* instance();

    // GET 请求
    void get(const QString& url, std::function<void(QJsonObject)> onSuccess, std::function<void(QString)> onError = nullptr);

    // POST 请求
    void post(const QString& url, const QJsonObject& data, std::function<void(QJsonObject)> onSuccess, std::function<void(QString)> onError = nullptr);

private:
    explicit NetworkHelper(QObject* parent = nullptr);
    QNetworkAccessManager* m_manager;
};

#endif // NETWORKHELPER_H 