/*
 * @Author: Libra
 * @Date: 2025-05-06 13:48:43
 * @LastEditors: Libra
 * @Description: 
 */
#ifndef FORMITEM_H
#define FORMITEM_H

#include <QString>
#include <QStringList>
#include <QWidget>
#include <functional>

struct FormItem {
    QString key;
    QString type; // "input"、"select"、"custom"
    QString placeholder;
    QStringList options; // 仅select用
    bool required;
    std::function<QWidget*(QWidget* parent)> customWidgetFactory = nullptr;
};

#endif // FORMITEM_H 