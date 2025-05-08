/*
 * @Author: Libra
 * @Date: 2025-05-06 13:48:43
 * @LastEditors: Libra
 * @Description: 
 */
#include "components/form/Form.h"
#include "components/form/FormInput.h"
#include "components/form/FormSelect.h"
#include "components/form/FormCustomWidget.h"
#include "components/form/FormLayoutManager.h"
#include "components/form/FormValidation.h"
#include "components/form/FormSignals.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QDebug>

Form::Form(QWidget *parent) : QWidget(parent) {
    layout = new QVBoxLayout(this);
    submitBtn = new Button("提交");
    submitBtn->setVariant(Button::Variant::Default);
    connect(submitBtn, &Button::clicked, this, &Form::handleSubmit);
}

void Form::handleSubmit() {
    FormSignalHelper::handleSubmit(this);
}

void Form::handleValueChanged() {
    FormSignalHelper::handleValueChanged(this);
}

void Form::setupFieldConnections(const QString& key, QWidget* widget) {
    if (auto input = qobject_cast<Input*>(widget)) {
        connect(input, &Input::textChanged, this, &Form::handleValueChanged);
        connect(input, &Input::textChanged, [this, key]() {
            FormValidationHelper::validateField(this, key, inputs[key]->text());
        });
    } else if (auto select = qobject_cast<Select*>(widget)) {
        connect(select, &Select::currentTextChanged, this, &Form::handleValueChanged);
        connect(select, &Select::currentTextChanged, [this, key]() {
            FormValidationHelper::validateField(this, key, selects[key]->currentText());
        });
    } else {
        // ✅ 处理 customWidget, 目前只处理 Input
        if (Input* customInput = widget->findChild<Input*>(key)) {
            connect(customInput, &Input::textChanged, this, &Form::handleValueChanged);
            connect(customInput, &Input::textChanged, [=]() {
                FormValidationHelper::validateField(this, key, customInput->text());
            });
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

    for (auto it = selects.begin(); it != selects.end(); ++it) {
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
        } else if (selects.contains(key)) {
            selects[key]->setCurrentText(value);
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
                FormInputHelper::addInput(this, item.key, item.placeholder);
                break;
            case FormItemType::Select:
                FormSelectHelper::addSelect(this, item.key, item.options, item.placeholder);
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
}

void Form::updateLayout() {
    FormLayoutManager::updateLayout(this);
}

void Form::setDisabled(const QString& key, bool disabled) {
    if (inputs.contains(key)) {
        inputs[key]->setEnabled(!disabled);
    } else if (selects.contains(key)) {
        selects[key]->setEnabled(!disabled);
    }
}

void Form::setRequired(const QString& key, bool required) {
    requiredFields[key] = required;

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


