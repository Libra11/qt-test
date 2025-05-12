#include "components/base/TagSelect.h"
#include "layout/FlowLayout.h"
#include <QDebug>

TagSelect::TagSelect(QWidget *parent)
    : QWidget(parent)
{
    m_layout = new FlowLayout(this, 0, 6, 6);
    setLayout(m_layout);
}

void TagSelect::setOptions(const QStringList &options)
{
    // 清除旧按钮
    QLayoutItem *child;
    while ((child = m_layout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->deleteLater();
        }
        delete child;
    }
    m_buttonMap.clear();

    // 添加新按钮
    for (const QString &opt : options) {
        QPushButton *btn = new QPushButton(opt, this);
        btn->setCheckable(true);
        btn->setStyleSheet(R"(
            QPushButton {
                border: 1px solid gray;
                border-radius: 4px;
                padding: 4px 8px;
                background-color: #f0f0f0;
            }
            QPushButton:checked {
                background-color: #0078D7;
                color: white;
            }
        )");
        connect(btn, &QPushButton::clicked, this, &TagSelect::onTagClicked);
        m_buttonMap.insert(btn, opt);
        m_layout->addWidget(btn);
    }
}

QStringList TagSelect::selectedOptions() const
{
    QStringList list;
    for (auto it = m_buttonMap.constBegin(); it != m_buttonMap.constEnd(); ++it) {
        if (it.key()->isChecked())
            list.append(it.value());
    }
    return list;
}

void TagSelect::clearSelection()
{
    for (auto btn : m_buttonMap.keys()) {
        btn->setChecked(false);
    }
    emit selectionChanged({});
}

void TagSelect::onTagClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    QStringList current = selectedOptions();
    qDebug() << "Selected options:" << current;
    emit selectionChanged(current);
}
