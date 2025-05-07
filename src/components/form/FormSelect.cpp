// FormSelect.cpp
#include "components/form/FormSelect.h"

void FormSelectHelper::addSelect(Form* form, const QString& key, const QStringList& options, const QString& placeholder) {
    QLabel* label = new QLabel(key, form);
    form->fieldLabels[key] = label;

    Select* select = new Select();
    if (!placeholder.isEmpty()) {
        select->addOption(placeholder, "");
    }
    for (const auto& opt : options) {
        select->addOption(opt, opt);
    }
    form->selects[key] = select;
    form->requiredFields[key] = false;
    form->m_fieldOrder.append(key);

    QLabel* errorLabel = new QLabel(form);
    errorLabel->setStyleSheet("color: #ef4444; font-size: 12px; margin-left: 4px;");
    errorLabel->setVisible(false);
    form->errorLabels[key] = errorLabel;

    form->initialValues[key] = "";

    form->setupFieldConnections(key, select);
    form->updateLayout();
}
