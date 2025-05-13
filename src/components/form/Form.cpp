/*
 * @Author: Libra
 * @Date: 2025-05-06 13:48:43
 * @LastEditors: Libra
 * @Description:
 */
#include "components/form/Form.h"
#include "components/form/FormInput.h"
#include "components/form/FormDropDown.h"
#include "components/form/FormCustomWidget.h"
#include "components/form/FormLayoutManager.h"
#include "components/form/FormValidation.h"
#include "components/form/FormSignals.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QDebug>

Form::Form(QWidget *parent) : QWidget(parent) {
    // 创建主布局作为窗口的布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0); // 移除主布局的边距

    // 创建一个容器来放置表单字段
    QWidget *formContainer = new QWidget();
    layout = new QVBoxLayout(formContainer);

    // 创建提交按钮
    submitBtn = new Button("提交");
    submitBtn->setVariant(Button::Variant::Default);
    connect(submitBtn, &Button::clicked, this, &Form::handleSubmit);

    // 创建一个容器来放置按钮，使其宽度与表单一致
    QWidget *buttonContainer = new QWidget();
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonContainer);

    // 设置按钮宽度为100%
    submitBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // 确保按钮容器的边距与表单容器一致
    buttonLayout->addWidget(submitBtn);
    buttonLayout->setContentsMargins(layout->contentsMargins());

    // 将表单容器和按钮容器添加到主布局
    mainLayout->addWidget(formContainer, 1);
    mainLayout->addWidget(buttonContainer, 0);

    // 设置一些间距
    mainLayout->setSpacing(20);
}

void Form::handleSubmit() {
    // Set loading state before submitting
    qDebug() << "Button clicked!";
    setLoading(true);

    // Emit beforeSubmit signal
    emit beforeSubmit();

    // Handle form submission
    FormSignalHelper::handleSubmit(this);

    // Note: The loading state should be turned off by the consumer of the form
    // after the async operation completes, by calling setLoading(false)
}

void Form::handleValueChanged() {
    FormSignalHelper::handleValueChanged(this);
}

void Form::setupFieldConnections(const QString& key, QWidget* widget) {
    if (auto input = qobject_cast<Input*>(widget)) {
        // 连接文本变化信号
        connect(input, &Input::textChanged, this, &Form::handleValueChanged);
        connect(input, &Input::textChanged, [this, key]() {
            FormValidationHelper::validateField(this, key, inputs[key]->text());
        });

        // 连接回车键信号，按回车键提交表单
        connect(input, &QLineEdit::returnPressed, this, &Form::handleSubmit);
    } else if (auto select = qobject_cast<DropDown*>(widget)) {
        connect(select, &DropDown::currentTextChanged, this, &Form::handleValueChanged);
        connect(select, &DropDown::currentTextChanged, [this, key]() {
            FormValidationHelper::validateField(this, key, dropDowns[key]->currentText());
        });
    } else {
        // ✅ 处理 customWidget, 目前只处理 Input
        if (Input* customInput = widget->findChild<Input*>(key)) {
            connect(customInput, &Input::textChanged, this, &Form::handleValueChanged);
            connect(customInput, &Input::textChanged, [=]() {
                FormValidationHelper::validateField(this, key, customInput->text());
            });

            // 连接回车键信号，按回车键提交表单
            connect(customInput, &QLineEdit::returnPressed, this, &Form::handleSubmit);
        }
    }
}


void Form::setSubmitText(const QString &text) {
    submitBtn->setText(text);
}

void Form::setLayout(Layout layout) {
    if (m_layout != layout) {
        m_layout = layout;
        updateLayout();
    }
}

void Form::setLabelPosition(LabelPosition pos) {
    if (m_labelPosition != pos) {
        m_labelPosition = pos;
        updateLayout();
    }
}

void Form::setLabelWidth(int width) {
    if (m_labelWidth != width) {
        m_labelWidth = width;
        updateLayout();
    }
}

void Form::reset() {
    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        it.value()->setText(initialValues[it.key()]);
        it.value()->setError(false);
    }

    for (auto it = dropDowns.begin(); it != dropDowns.end(); ++it) {
        it.value()->setCurrentText(initialValues[it.key()]);
    }

    for (auto it = errorLabels.begin(); it != errorLabels.end(); ++it) {
        it.value()->setVisible(false);
    }

    m_isDirty = false;
}

void Form::setValues(const QMap<QString, QString>& values) {
    for (auto it = values.begin(); it != values.end(); ++it) {
        const QString& key = it.key();
        const QString& value = it.value();

        if (inputs.contains(key)) {
            inputs[key]->setText(value);
        } else if (dropDowns.contains(key)) {
            dropDowns[key]->setCurrentText(value);
        }
    }
}

QWidget* Form::getCustomWidget(const QString& key) const {
    return this->findChild<QWidget*>(key);
}

void Form::setupByConfig(const QList<FormItem>& items) {
    for (const auto& item : items) {
        switch (item.type) {
            case FormItemType::Input:
                FormInputHelper::addInput(this, item);
                break;
            case FormItemType::Password:
                FormInputHelper::addInput(this, item);
                break;
            case FormItemType::DropDown:
                FormDropDownHelper::addDropDown(this, item);
                break;
            case FormItemType::Custom:
                FormCustomWidgetHelper::addCustomWidget(this, item);
                break;
        }
        m_fieldOrder.append(item.key);

        if (!item.validationRules.isEmpty()) {
            validationRules[item.key] = item.validationRules;
        }
        if (item.required) {
            setRequired(item.key, true);
        }
        if (item.disabled) {
            setDisabled(item.key, true);
        }
    }

    updateLayout();
    applyStyleToFormItem();
}

void Form::updateLayout() {
    FormLayoutManager::updateLayout(this);
}

void Form::setDisabled(const QString& key, bool disabled) {
    if (inputs.contains(key)) {
        inputs[key]->setEnabled(!disabled);
    } else if (dropDowns.contains(key)) {
        dropDowns[key]->setEnabled(!disabled);
    }
}

void Form::setRequired(const QString& key, bool required) {
    requiredFields[key] = required;
    qDebug() << fieldLabels[key]->text();
    if (fieldLabels.contains(key)) {
        QString labelText = fieldLabels[key]->text();
        if (required && !labelText.endsWith(" *")) {
            fieldLabels[key]->setText(labelText + " *");
        } else if (!required && labelText.endsWith(" *")) {
            labelText.chop(2);
            fieldLabels[key]->setText(labelText);
        }
    }
}

void Form::setLoading(bool loading) {
    // Set the submit button loading state
    if (submitBtn) {
        submitBtn->setLoading(loading);
    }

    // Disable all inputs and selects while loading
    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        it.value()->setEnabled(!loading);
    }

    for (auto it = dropDowns.begin(); it != dropDowns.end(); ++it) {
        it.value()->setEnabled(!loading);
    }

    // Emit afterSubmit signal when loading is complete
    if (!loading) {
        emit afterSubmit();
    }
}

bool Form::isLoading() const {
    return submitBtn ? submitBtn->isLoading() : false;
}

void Form::applyStyleToFormItem()
{
    for (auto dropdown : dropDowns) {
        if (dropdown)
            dropdown->setStyleSheet("QComboBox { border: 1px solid #ccc; padding: 4px; }");
    }

    for (auto label : errorLabels) {
        if (label)
            label->setStyleSheet("color: red;");
    }

    for (auto label : fieldLabels) {
        if (label)
            label->setStyleSheet("color: #888; font-weight: bold; font-size: 16px;");
    }
}


