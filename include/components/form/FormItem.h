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

// 表单项类型枚举
enum class FormItemType {
    Input,
    Select,
    Custom
};

// 表单验证规则
struct FormValidationRule {
    std::function<bool(const QString&)> validator;
    QString errorMessage;
};

struct FormItem {
    QString key;
    FormItemType type = FormItemType::Input;  // 默认为 Input 类型
    QString placeholder;
    QString label;                            // 添加标签文本
    QStringList options;                      // 仅select用
    bool required = false;
    bool disabled = false;                    // 添加禁用状态
    std::function<QWidget*(QWidget* parent)> customWidgetFactory = nullptr;
    QList<FormValidationRule> validationRules;  // 添加验证规则列表
FormItem(QString key, FormItemType type, QString label, QString placeholder, bool required, std::function<QWidget*(QWidget* parent)> customWidgetFactory = nullptr)
        : key(key), type(type),  placeholder(placeholder), label(label), required(required), customWidgetFactory(customWidgetFactory) {}
};

#endif // FORMITEM_H
