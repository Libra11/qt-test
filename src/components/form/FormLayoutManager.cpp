/*
 * @Author: Libra
 * @Date: 2025-05-07 17:56:18
 * @LastEditors: Libra
 * @Description: 
 */
#include "components/form/FormLayoutManager.h"
#include <QLayoutItem>
#include <QLayout>

void FormLayoutManager::updateLayout(Form* form) {
    QLayout* oldLayout = form->layout;
    form->layout = nullptr;

    if (oldLayout) {
        QLayoutItem* item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            delete item;
        }
        delete oldLayout;
    }

    switch (form->m_layout) {
        case Form::Layout::Horizontal: {
            form->layout = new QHBoxLayout(form);
            break;
        }
        case Form::Layout::Grid: {
            auto gridLayout = new QGridLayout(form);
            int row = 0;

            for (const QString& key : form->m_fieldOrder) {
                QLabel* label = form->fieldLabels.value(key);
                QWidget* widget = nullptr;
                QLabel* errorLabel = form->errorLabels.contains(key) ? form->errorLabels[key] : nullptr;

                if (form->inputs.contains(key)) {
                    widget = form->inputs[key];
                } else if (form->selects.contains(key)) {
                    widget = form->selects[key];
                } else {
                    widget = form->findChild<QWidget*>(key);
                }

                if (label && widget) {
                    if (form->m_labelPosition == Form::LabelPosition::Left) {
                        label->setFixedWidth(form->m_labelWidth);
                        gridLayout->addWidget(label, row, 0);
                        gridLayout->addWidget(widget, row, 1);
                        if (errorLabel) {
                            gridLayout->addWidget(errorLabel, row, 2);
                        }
                    } else {
                        gridLayout->addWidget(label, row, 0, 1, -1);
                        row++;
                        gridLayout->addWidget(widget, row, 0);
                        if (errorLabel) {
                            gridLayout->addWidget(errorLabel, row, 1);
                        }
                    }
                    row++;
                }
            }

            gridLayout->addWidget(form->submitBtn, row, 0, 1, -1, Qt::AlignCenter);
            form->layout = gridLayout;
            break;
        }
        default: {
            form->layout = new QVBoxLayout(form);

            for (const QString& key : form->m_fieldOrder) {
                QLabel* label = form->fieldLabels.value(key);
                QWidget* widget = nullptr;
                QLabel* errorLabel = form->errorLabels.contains(key) ? form->errorLabels[key] : nullptr;

                if (form->inputs.contains(key)) {
                    widget = form->inputs[key];
                } else if (form->selects.contains(key)) {
                    widget = form->selects[key];
                } else {
                    widget = form->findChild<QWidget*>(key);
                }

                if (label && widget) {
                    if (form->m_labelPosition == Form::LabelPosition::Left) {
                        auto container = new QWidget;
                        auto hLayout = new QHBoxLayout(container);
                        label->setFixedWidth(form->m_labelWidth);
                        hLayout->addWidget(label);
                        hLayout->addWidget(widget);
                        if (errorLabel) {
                            hLayout->addWidget(errorLabel);
                        }
                        hLayout->setContentsMargins(0, 0, 0, 0);
                        form->layout->addWidget(container);
                    } else {
                        form->layout->addWidget(label);
                        form->layout->addWidget(widget);
                        if (errorLabel) {
                            form->layout->addWidget(errorLabel);
                        }
                    }
                }
            }

            form->layout->addWidget(form->submitBtn);
            break;
        }
    }

    form->layout->setSpacing(10);
    form->layout->setContentsMargins(10, 10, 10, 10);
}
