/*
 * @Author: Libra
 * @Date: 2025-05-06 14:43:53
 * @LastEditors: Libra
 * @Description: 
 */
#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H

#include <QStackedWidget>
#include <QMap>
#include <QString>
#include <QWidget>

class PageManager : public QObject
{
    Q_OBJECT
public:
    explicit PageManager(QStackedWidget* stack, QObject* parent = nullptr);

    // 注册页面，name为唯一标识
    void registerPage(const QString& name, QWidget* page);

    // 切换到指定页面
    void switchTo(const QString& name);

    // 获取页面指针
    QWidget* getPage(const QString& name) const;

    // 获取页面名到索引的映射
    QMap<QString, int> getPageMap() const;

private:
    QStackedWidget* m_stack;
    QMap<QString, int> m_pageMap;
};

#endif // PAGEMANAGER_H 