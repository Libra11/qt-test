/*
 * @Author: Libra
 * @Date: 2025-05-03 14:30:26
 * @LastEditors: Libra
 * @Description: 
 */
#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QVBoxLayout>
#include <QMap>
#include <QString>
#include <QLabel>
#include "components/base/Input.h"
#include "components/base/Button.h"
#include "components/base/Select.h"
#include "components/form/FormItem.h"

class Form : public QWidget {
    Q_OBJECT
public:
    explicit Form(QWidget *parent = nullptr);

    // 添加输入框，key为字段名，placeholder为占位符
    void addInput(const QString &key, const QString &placeholder = "");
    // 添加下拉框，key为字段名，options为选项列表，placeholder为占位符
    void addSelect(const QString &key, const QStringList &options, const QString &placeholder = "");
    // 设置输入框为必填
    void setRequired(const QString &key, bool required = true);
    // 获取输入框内容
    QString value(const QString &key) const;
    // 设置提交按钮文本
    void setSubmitText(const QString &text);
    void setupByConfig(const QList<FormItem>& items);
    QWidget* getCustomWidget(const QString& key) const;

signals:
    // 表单提交信号，参数为所有字段的键值对
    void submitted(const QMap<QString, QString> &values);

private slots:
    void handleSubmit();

private:
    QVBoxLayout *layout;
    QMap<QString, Input*> inputs;
    QMap<QString, Select*> selects;
    QMap<QString, QLabel*> errorLabels;
    QMap<QString, bool> requiredFields;
    Button *submitBtn;
};

#endif // FORM_H 