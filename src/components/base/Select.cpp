#include "components/base/Select.h"
#include "components/base/ColorManager.h"

Select::Select(QWidget *parent)
    : QComboBox(parent)
{
    setMinimumHeight(36);
    setStyleSheet(
        QString(
            "QComboBox {"
            "  border: 1px solid %1;"
            "  border-radius: 6px;"
            "  padding: 6px 12px;"
            "  font-size: 14px;"
            "  background: %2;"
            "}"
            "QComboBox:focus {"
            "  border-color: %3;"
            "}"
            "QComboBox:disabled {"
            "  background: %4;"
            "  color: %5;"
            "}"
        ).arg(
            ColorManager::outline(),
            ColorManager::white(),
            ColorManager::primary(),
            ColorManager::secondaryBg(),
            ColorManager::outline()
        )
    );
}

void Select::addOption(const QString &text, const QVariant &value)
{
    addItem(text, value);
}

void Select::setValue(const QVariant &value)
{
    int idx = findData(value);
    if (idx >= 0) {
        setCurrentIndex(idx);
    }
}

QVariant Select::value() const
{
    return currentData();
} 