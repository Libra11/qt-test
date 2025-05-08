/*
 * @Author: Libra
 * @Date: 2025-05-08 15:43:21
 * @LastEditors: Libra
 * @Description: 
 */
#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include <QObject>
#include <QTranslator>
#include <QLocale>

class LanguageManager : public QObject
{
    Q_OBJECT
public:
    static LanguageManager* instance();

    void loadLanguage(const QString& languageCode);
    QString currentLanguage() const;

signals:
    void languageChanged();

private:
    explicit LanguageManager(QObject *parent = nullptr);
    QTranslator m_translator;
    QString m_currentLanguage;
};

#endif // LANGUAGEMANAGER_H
