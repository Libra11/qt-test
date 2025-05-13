/*
 * @Author: Libra
 * @Date: 2025-05-07 17:58:11
 * @LastEditors: Libra
 * @Description:
 */
#include "components/form/FormValidation.h"

bool FormValidationHelper::validateField(Form* form, const QString& key, const QString& value) {
    bool valid = true;
    QString errorMessage;

    if (form->requiredFields[key] && value.trimmed().isEmpty()) {
        valid = false;

        // 获取字段标签文本
        QString fieldLabel = "";
        if (form->fieldLabels.contains(key)) {
            fieldLabel = form->fieldLabels[key]->text();
            // 移除可能的必填标记 " *"
            if (fieldLabel.endsWith(" *")) {
                fieldLabel.chop(2);
            }
        }

        // 如果有标签文本，使用"请输入[字段名]"，否则使用通用消息
        if (!fieldLabel.isEmpty()) {
            errorMessage = QString("请输入%1").arg(fieldLabel);
        } else {
            errorMessage = "这是必填项";
        }
    }

    if (valid && form->validationRules.contains(key)) {
        for (const auto& rule : form->validationRules[key]) {
            if (!rule.validator(value)) {
                valid = false;
                errorMessage = rule.errorMessage;
                break;
            }
        }
    }

    if (form->inputs.contains(key)) {
        form->inputs[key]->setError(!valid);
    }

    if (form->errorLabels.contains(key)) {
        form->errorLabels[key]->setText(errorMessage);
        form->errorLabels[key]->setVisible(!valid);
    }

    if (!valid) {
        emit form->validationError(key, errorMessage);
    }

    return valid;
}
