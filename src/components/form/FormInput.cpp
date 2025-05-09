// FormInput.cpp
#include "components/form/FormInput.h"

void FormInputHelper::addInput(Form* form, const QString& key, const QString& placeholder, FormItemType type) {
    QLabel* label = new QLabel(key, form);
    form->fieldLabels[key] = label;

    Input* input = new Input();
    input->setPlaceholder(placeholder);

    // Set input type based on FormItemType
    if (type == FormItemType::Password) {
        input->setType(Input::Type::Password);
    } else {
        input->setType(Input::Type::Text);
    }

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
