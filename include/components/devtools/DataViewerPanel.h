/*
 * @Author: Libra
 * @Date: 2025-05-08 10:50:10
 * @LastEditors: Libra
 * @Description: 
 */
#ifndef DATAVIEWERPANEL_H
#define DATAVIEWERPANEL_H

#include <QWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QSettings>

class DataViewerPanel : public QWidget {
    Q_OBJECT
public:
    explicit DataViewerPanel(QWidget *parent = nullptr);

    // 加载 QSettings 数据
    void loadSettings(QSettings *settings);

    // 预留：未来可以扩展其他数据源
    // void loadDatabaseTable(...);
    // void loadJsonFile(...);

private:
    void addSettingsGroup(QSettings *settings, QTreeWidgetItem *parent, const QString &group);

    QTreeWidget *treeWidget;
};

#endif // DATAVIEWERPANEL_H
