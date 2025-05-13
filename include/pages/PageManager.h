#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H

#include <QObject>
#include <QStackedWidget>
#include <QMap>
#include <QString>
#include <functional>
#include "PageBase.h"

class PageManager : public QObject
{
    Q_OBJECT
public:
    explicit PageManager(QStackedWidget* stack, QObject* parent = nullptr);

    void registerPage(const QString& name, std::function<PageBase*()> pageFactory);
    void switchTo(const QString& name);
    QWidget* getPage(const QString& name) const;

private:
    QStackedWidget* m_stack;
    QMap<QString, int> m_pageMap;
    QMap<QString, std::function<PageBase*()>> m_pageFactories;
};

#endif // PAGEMANAGER_H
