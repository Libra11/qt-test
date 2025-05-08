/*
 * @Author: Libra
 * @Date: 2025-05-08 14:48:00
 * @LastEditors: Libra
 * @Description: 
 */
#ifndef PAGEBASE_H
#define PAGEBASE_H

#include <QWidget>

class PageBase : public QWidget
{
    Q_OBJECT
public:
    explicit PageBase(QWidget *parent = nullptr) : QWidget(parent) {}

signals:
    // 页面跳转信号
    void routeTo(const QString &pageName);
};

#endif // PAGEBASE_H
