#include "components/form/FormSignals.h"
#include "components/form/FormValidation.h"

void FormSignalHelper::handleValueChanged(Form* form) {
    form->m_isDirty = true;
    emit form->formChanged();
}

void FormSignalHelper::handleSubmit(Form* form) {
    emit form->beforeSubmit();

    bool valid = true;
    QMap<QString, QString> values;

    for (auto it = form->inputs.begin(); it != form->inputs.end(); ++it) {
        QString key = it.key();
        QString val = it.value()->text();
        if (!FormValidationHelper::validateField(form, key, val)) {
            valid = false;
        }
        values[key] = val;
    }

    for (auto it = form->selects.begin(); it != form->selects.end(); ++it) {
        QString key = it.key();
        QString val = it.value()->currentText();
        if (!FormValidationHelper::validateField(form, key, val)) {
            valid = false;
        }
        values[key] = val;
    }

    for (auto it = form->requiredFields.begin(); it != form->requiredFields.end(); ++it) {
        const QString& key = it.key();
        bool required = it.value();

        if (required && !form->inputs.contains(key) && !form->selects.contains(key)) {
            QWidget* customWidget = form->findChild<QWidget*>(key);
            if (customWidget) {
                if (auto input = qobject_cast<Input*>(customWidget)) {
                    QString val = input->text();
                    if (!FormValidationHelper::validateField(form, key, val)) {
                        valid = false;
                    }
                    values[key] = val;
                }
            }
        }
    }

    if (valid) {
        emit form->submitted(values);
        emit form->afterSubmit();
        form->m_isDirty = false;
    }
}
