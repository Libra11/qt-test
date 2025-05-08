/*
 * @Author: Libra
 * @Date: 2025-05-08 15:43:57
 * @LastEditors: Libra
 * @Description: 
 */
#include "helpers/LanguageManager.h"
#include <QApplication>
#include <QDebug>

LanguageManager::LanguageManager(QObject *parent)
    : QObject(parent)
{
}

LanguageManager* LanguageManager::instance()
{
    static LanguageManager mgr;
    return &mgr;
}

void LanguageManager::loadLanguage(const QString& languageCode)
{
    if (m_currentLanguage == languageCode)
        return;

    qApp->removeTranslator(&m_translator);
    QString qmPath = QString(QCoreApplication::applicationDirPath() + "/i18n/%1.qm").arg(languageCode);
    if (m_translator.load(qmPath)) {
        qApp->installTranslator(&m_translator);
        m_currentLanguage = languageCode;
        emit languageChanged();
        qDebug() << "Loaded language:" << languageCode;
    } else {
        qDebug() << "Failed to load language:" << qmPath;
    }
}

QString LanguageManager::currentLanguage() const
{
    return m_currentLanguage;
}
