// FormInput.cpp
#include "components/form/FormInput.h"

void FormInputHelper::addInput(Form* form, const QString& key, const QString& placeholder) {
    QLabel* label = new QLabel(key, form);
    form->fieldLabels[key] = label;

    Input* input = new Input();
    input->setPlaceholder(placeholder);
    form->inputs[key] = input;
    form->requiredFields[key] = false;
    form->m_fieldOrder.append(key);

    QLabel* errorLabel = new QLabel(form);
    errorLabel->setStyleSheet("color: #ef4444; font-size: 12px; margin-left: 4px;");
    errorLabel->setVisible(false);
    form->errorLabels[key] = errorLabel;

    form->initialValues[key] = "";

    form->setupFieldConnections(key, input);
    form->updateLayout();
}
