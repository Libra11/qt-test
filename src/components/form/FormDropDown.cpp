// FormDropDown.cpp
#include "components/form/FormDropDown.h"

void FormDropDownHelper::addDropDown(Form* form, const FormItem& item) {
    QLabel* label = new QLabel(item.label, form);
    form->fieldLabels[item.key] = label;

    DropDown* dropDown = new DropDown();
    if (!item.placeholder.isEmpty()) {
        dropDown->addOption(item.placeholder, "");
    }
    for (const auto& opt : item.options) {
        dropDown->addOption(opt, opt);
    }
    form->dropDowns[item.key] = dropDown;
    form->requiredFields[item.key] = false;
    form->m_fieldOrder.append(item.key);

    QLabel* errorLabel = new QLabel(form);
    errorLabel->setVisible(false);
    form->errorLabels[item.key] = errorLabel;

    form->initialValues[item.key] = "";

    form->setupFieldConnections(item.key, dropDown);
    form->updateLayout();
}
