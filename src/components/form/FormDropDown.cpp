// FormDropDown.cpp
#include "components/form/FormDropDown.h"

void FormDropDownHelper::addDropDown(Form* form, const QString& key, const QStringList& options, const QString& placeholder) {
    QLabel* label = new QLabel(key, form);
    form->fieldLabels[key] = label;

    DropDown* dropDown = new DropDown();
    if (!placeholder.isEmpty()) {
        dropDown->addOption(placeholder, "");
    }
    for (const auto& opt : options) {
        dropDown->addOption(opt, opt);
    }
    form->dropDowns[key] = dropDown;
    form->requiredFields[key] = false;
    form->m_fieldOrder.append(key);

    QLabel* errorLabel = new QLabel(form);
    errorLabel->setStyleSheet("color: #ef4444; font-size: 12px; margin-left: 4px;");
    errorLabel->setVisible(false);
    form->errorLabels[key] = errorLabel;

    form->initialValues[key] = "";

    form->setupFieldConnections(key, dropDown);
    form->updateLayout();
}
