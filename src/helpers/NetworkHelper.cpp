/*
 * @Author: Libra
 * @Date: 2025-05-06 15:42:46
 * @LastEditors: Libra
 * @Description: 
 */
#include "helpers/NetworkHelper.h"
#include <QNetworkRequest>
#include <QUrl>

NetworkHelper* NetworkHelper::instance()
{
    static NetworkHelper inst;
    return &inst;
}

NetworkHelper::NetworkHelper(QObject* parent)
    : QObject(parent)
{
    m_manager = new QNetworkAccessManager(this);
}

void NetworkHelper::setToken(const QString& token) { m_token = token; }
QString NetworkHelper::getToken() const { return m_token; }

void NetworkHelper::get(const QString& url, std::function<void(QJsonObject)> onSuccess, std::function<void(QString)> onError)
{
    // 确保 QUrl 是有效的
    QUrl qUrl(url);
    if (!qUrl.isValid()) {
        // 处理无效 URL
        if (onError) {
            onError("Invalid URL");
        }
        return;
    }
    
    QNetworkRequest req(qUrl);
    if (!m_token.isEmpty()) {
        req.setRawHeader("Authorization", "Bearer "+ m_token.toUtf8());
    }
    QNetworkReply* reply = m_manager->get(req);
    QObject::connect(reply, &QNetworkReply::finished, [reply, onSuccess, onError]() {
        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isObject()) {
            onSuccess(doc.object());
        } else if (onError) {
            onError(QString::fromUtf8(data));
        }
        reply->deleteLater();
    });
}

void NetworkHelper::post(const QString& url, const QJsonObject& data, std::function<void(QJsonObject)> onSuccess, std::function<void(QString)> onError)
{
    // 确保 QUrl 是有效的
    QUrl qUrl(url);
    if (!qUrl.isValid()) {
        // 处理无效 URL
        if (onError) {
            onError("Invalid URL");
        }
        return;
    }

    QNetworkRequest req(qUrl);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    if (!m_token.isEmpty()) {
        req.setRawHeader("Authorization", m_token.toUtf8());
    }
    QNetworkReply* reply = m_manager->post(req, QJsonDocument(data).toJson());
    QObject::connect(reply, &QNetworkReply::finished, [reply, onSuccess, onError]() {
        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isObject()) {
            onSuccess(doc.object());
        } else if (onError) {
            onError(QString::fromUtf8(data));
        }
        reply->deleteLater();
    });
}
