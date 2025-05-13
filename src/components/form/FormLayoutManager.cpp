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
    // 清除当前布局中的所有项目
    if (form->layout) {
        QLayoutItem* item;
        while ((item = form->layout->takeAt(0)) != nullptr) {
            if (item->widget()) {
                // 不删除控件，只从布局中移除
                item->widget()->setParent(nullptr);
            }
            delete item;
        }
    }

    // 根据布局类型添加项目
    switch (form->m_layout) {
        case Form::Layout::Horizontal: {
            // 确保是水平布局
            QHBoxLayout* hLayout = qobject_cast<QHBoxLayout*>(form->layout);
            if (!hLayout) {
                // 获取当前布局的父窗口
                QWidget* parent = nullptr;
                if (form->layout) {
                    parent = qobject_cast<QWidget*>(form->layout->parent());
                    delete form->layout;
                }

                // 创建新的水平布局
                hLayout = new QHBoxLayout(parent);
                form->layout = hLayout;
            }

            // 添加字段到水平布局
            for (const QString& key : form->m_fieldOrder) {
                QLabel* label = form->fieldLabels.value(key);
                QWidget* widget = nullptr;
                QLabel* errorLabel = form->errorLabels.contains(key) ? form->errorLabels[key] : nullptr;

                if (form->inputs.contains(key)) {
                    widget = form->inputs[key];
                } else if (form->dropDowns.contains(key)) {
                    widget = form->dropDowns[key];
                } else {
                    widget = form->findChild<QWidget*>(key);
                }

                if (label && widget) {
                    QHBoxLayout* rowLayout = new QHBoxLayout();
                    rowLayout->addWidget(label);
                    rowLayout->addWidget(widget);
                    if (errorLabel) {
                        rowLayout->addWidget(errorLabel);
                    }
                    hLayout->addLayout(rowLayout);
                }
            }
            break;
        }
        case Form::Layout::Grid: {
            // 如果当前不是网格布局，需要创建一个新的
            QGridLayout* gridLayout = qobject_cast<QGridLayout*>(form->layout);
            if (!gridLayout) {
                // 获取当前布局的父窗口
                QWidget* parent = nullptr;
                if (form->layout) {
                    parent = qobject_cast<QWidget*>(form->layout->parent());
                    delete form->layout;
                }

                // 创建新的网格布局
                gridLayout = new QGridLayout(parent);
                form->layout = gridLayout;
            }

            int row = 0;
            for (const QString& key : form->m_fieldOrder) {
                QLabel* label = form->fieldLabels.value(key);
                QWidget* widget = nullptr;
                QLabel* errorLabel = form->errorLabels.contains(key) ? form->errorLabels[key] : nullptr;

                if (form->inputs.contains(key)) {
                    widget = form->inputs[key];
                } else if (form->dropDowns.contains(key)) {
                    widget = form->dropDowns[key];
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
            break;
        }
        default: {
            // 确保是垂直布局
            QVBoxLayout* vLayout = qobject_cast<QVBoxLayout*>(form->layout);
            if (!vLayout) {
                // 获取当前布局的父窗口
                QWidget* parent = nullptr;
                if (form->layout) {
                    parent = qobject_cast<QWidget*>(form->layout->parent());
                    delete form->layout;
                }

                // 创建新的垂直布局
                vLayout = new QVBoxLayout(parent);
                form->layout = vLayout;
            }

            // 垂直布局（默认）
            for (const QString& key : form->m_fieldOrder) {
                QLabel* label = form->fieldLabels.value(key);
                QWidget* widget = nullptr;
                QLabel* errorLabel = form->errorLabels.contains(key) ? form->errorLabels[key] : nullptr;

                if (form->inputs.contains(key)) {
                    widget = form->inputs[key];
                } else if (form->dropDowns.contains(key)) {
                    widget = form->dropDowns[key];
                } else {
                    widget = form->findChild<QWidget*>(key);
                }

                if (label && widget) {
                    // 创建一个字段容器：内部垂直布局，间距10
                    auto fieldContainer = new QWidget;
                    auto fieldLayout = new QVBoxLayout(fieldContainer);
                    fieldLayout->setSpacing(10);
                    fieldLayout->setContentsMargins(0, 0, 0, 0);

                    if (form->m_labelPosition == Form::LabelPosition::Left) {
                        auto rowContainer = new QWidget;
                        auto hLayout = new QHBoxLayout(rowContainer);
                        label->setFixedWidth(form->m_labelWidth);
                        hLayout->addWidget(label);
                        hLayout->addWidget(widget);
                        if (errorLabel) {
                            hLayout->addWidget(errorLabel);
                        }
                        hLayout->setSpacing(10);
                        hLayout->setContentsMargins(0, 0, 0, 0);
                        fieldLayout->addWidget(rowContainer);
                    } else {
                        fieldLayout->addWidget(label);
                        fieldLayout->addWidget(widget);
                        if (errorLabel) {
                            fieldLayout->addWidget(errorLabel);
                        }
                    }

                    vLayout->addWidget(fieldContainer);
                }
            }
            break;
        }
    }
}
