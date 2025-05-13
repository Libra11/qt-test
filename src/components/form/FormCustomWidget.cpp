// FormCustomWidget.cpp
#include "components/form/FormCustomWidget.h"

void FormCustomWidgetHelper::addCustomWidget(Form* form, const FormItem& item) {
    if (item.customWidgetFactory) {
        QWidget* customWidget = item.customWidgetFactory(form);
        customWidget->setObjectName(item.key);

        if (!item.label.isEmpty()) {
            QLabel* label = new QLabel(item.label, form);
            form->fieldLabels[item.key] = label;
        }

        QLabel* errorLabel = new QLabel(form);
        errorLabel->setVisible(false);
        form->errorLabels[item.key] = errorLabel;

        form->initialValues[item.key] = "";

        form->setupFieldConnections(item.key, customWidget);
        form->m_fieldOrder.append(item.key);
    }
}
