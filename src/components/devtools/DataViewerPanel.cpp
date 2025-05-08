/*
 * @Author: Libra
 * @Date: 2025-05-08 10:50:36
 * @LastEditors: Libra
 * @Description: 
 */
#include "components/devtools/DataViewerPanel.h"

DataViewerPanel::DataViewerPanel(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowTitle("数据查看面板");
    this->resize(500, 600);

    treeWidget = new QTreeWidget(this);
    treeWidget->setHeaderLabels(QStringList() << "Key" << "Value");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(treeWidget);
}

void DataViewerPanel::loadSettings(QSettings *settings)
{
    treeWidget->clear();
    addSettingsGroup(settings, nullptr, QString());
}

void DataViewerPanel::addSettingsGroup(QSettings *settings, QTreeWidgetItem *parent, const QString &group)
{
    settings->beginGroup(group);

    QStringList keys = settings->childKeys();
    for (const QString &key : keys) {
        QString fullKey = group.isEmpty() ? key : group + "/" + key;
        QVariant value = settings->value(key);

        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, fullKey);
        item->setText(1, value.toString());

        if (parent) {
            parent->addChild(item);
        } else {
            treeWidget->addTopLevelItem(item);
        }
    }

    QStringList groups = settings->childGroups();
    for (const QString &subGroup : groups) {
        QTreeWidgetItem *groupItem = new QTreeWidgetItem();
        groupItem->setText(0, group.isEmpty() ? subGroup : group + "/" + subGroup);
        if (parent) {
            parent->addChild(groupItem);
        } else {
            treeWidget->addTopLevelItem(groupItem);
        }

        addSettingsGroup(settings, groupItem, subGroup);
    }

    settings->endGroup();
}
