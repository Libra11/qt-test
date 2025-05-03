/*
 * @Author: Libra
 * @Date: 2025-05-03 14:30:45
 * @LastEditors: Libra
 * @Description: 
 */
#include "components/form/Form.h"
#include <QMap>
#include <QLabel>
#include "components/form/FormItem.h"

Form::Form(QWidget *parent)
    : QWidget(parent)
{
    layout = new QVBoxLayout(this);
    submitBtn = new Button("提交");
    submitBtn->setVariant(Button::Variant::Default);
    layout->addWidget(submitBtn);
    connect(submitBtn, &Button::clicked, this, &Form::handleSubmit);
}

void Form::addInput(const QString &key, const QString &placeholder)
{
    Input *input = new Input();
    input->setPlaceholder(placeholder);
    layout->insertWidget(layout->count() - 1, input); // 插入到提交按钮前
    inputs[key] = input;
    requiredFields[key] = false;

    QLabel *errorLabel = new QLabel;
    errorLabel->setText("");
    errorLabel->setStyleSheet("color: #ef4444; font-size: 12px; margin-left: 4px;");
    errorLabel->setVisible(false);
    layout->insertWidget(layout->count() - 1, errorLabel); // 紧跟在 input 后面
    errorLabels[key] = errorLabel;
}

void Form::addSelect(const QString &key, const QStringList &options, const QString &placeholder)
{
    Select *select = new Select();
    if (!placeholder.isEmpty()) {
        select->addOption(placeholder, "");
    }
    for (const auto &opt : options) {
        select->addOption(opt, opt);
    }
    layout->insertWidget(layout->count() - 1, select);
    selects[key] = select;
    requiredFields[key] = false;

    QLabel *errorLabel = new QLabel;
    errorLabel->setText("");
    errorLabel->setStyleSheet("color: #ef4444; font-size: 12px; margin-left: 4px;");
    errorLabel->setVisible(false);
    layout->insertWidget(layout->count() - 1, errorLabel);
    errorLabels[key] = errorLabel;
}

void Form::setRequired(const QString &key, bool required)
{
    requiredFields[key] = required;
}

QString Form::value(const QString &key) const
{
    if (inputs.contains(key)) {
        return inputs[key]->text();
    }
    if (selects.contains(key)) {
        return selects[key]->currentText();
    }
    return "";
}

void Form::setSubmitText(const QString &text)
{
    submitBtn->setText(text);
}

void Form::handleSubmit()
{
    bool valid = true;
    QMap<QString, QString> values;
    // 校验输入框
    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        QString key = it.key();
        Input *input = it.value();
        QString val = input->text();
        bool required = requiredFields.value(key, false);
        QLabel *errorLabel = errorLabels.value(key, nullptr);

        if (required && val.trimmed().isEmpty()) {
            input->setError(true);
            if (errorLabel) {
                errorLabel->setText("这是必填项");
                errorLabel->setVisible(true);
            }
            valid = false;
        } else {
            input->setError(false);
            if (errorLabel) {
                errorLabel->setText("");
                errorLabel->setVisible(false);
            }
        }
        values[key] = val;
    }
    // 校验下拉框
    for (auto it = selects.begin(); it != selects.end(); ++it) {
        QString key = it.key();
        Select *select = it.value();
        QString val = select->currentText();
        bool required = requiredFields.value(key, false);
        QLabel *errorLabel = errorLabels.value(key, nullptr);
        if (required && (val.isEmpty() || val == select->itemText(0))) {
            if (errorLabel) {
                errorLabel->setText("这是必填项");
                errorLabel->setVisible(true);
            }
            valid = false;
        } else {
            if (errorLabel) {
                errorLabel->setText("");
                errorLabel->setVisible(false);
            }
        }
        values[key] = val;
    }
    if (valid) {
        emit submitted(values);
    }
}

void Form::setupByConfig(const QList<FormItem>& items)
{
    for (const auto& item : items) {
        if (item.type == "input") {
            addInput(item.key, item.placeholder);
        } else if (item.type == "select") {
            addSelect(item.key, item.options, item.placeholder);
        }
        setRequired(item.key, item.required);
    }
} 