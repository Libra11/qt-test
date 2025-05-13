// FormInput.cpp
#include "components/form/FormInput.h"

void FormInputHelper::addInput(Form* form, const FormItem& item) {
    QLabel* label = new QLabel(item.label, form);
    form->fieldLabels[item.key] = label;

    Input* input = new Input();
    input->setPlaceholder(item.placeholder);

    // Set input type based on FormItemType
    if (item.type == FormItemType::Password) {
        input->setType(Input::Type::Password);
    } else {
        input->setType(Input::Type::Text);
    }

    form->inputs[item.key] = input;
    form->requiredFields[item.key] = false;
    form->m_fieldOrder.append(item.key);

    QLabel* errorLabel = new QLabel(form);
    errorLabel->setVisible(false);
    form->errorLabels[item.key] = errorLabel;

    form->initialValues[item.key] = "";

    form->setupFieldConnections(item.key, input);
    form->updateLayout();
}
