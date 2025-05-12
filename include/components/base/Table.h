// Enhanced Table.h - supports web-style multi-select and sorting
#ifndef TABLE_H
#define TABLE_H

#include <QTableWidget>
#include <QHeaderView>
#include <QCheckBox>
#include <QMap>
#include <QString>
#include <QVariant>

class Table : public QTableWidget
{
    Q_OBJECT
public:
    struct Column {
        QString key;
        QString title;
        int width;
        bool sortable;
    };

    explicit Table(QWidget *parent = nullptr);
    virtual ~Table() = default;

    void setColumns(const QList<Column> &columns);
    void setData(const QList<QMap<QString, QVariant>> &data);
    QList<int> getSelectedRows() const;
    void clearSelection();

signals:
    void selectionChanged(const QList<int> &selectedRows);
    void sortChanged(const QString &column, Qt::SortOrder order);

private:
    void setupUI();
    void setupHeaderCheckbox();
    void updateHeaderCheckbox();
    void onHeaderClicked(int index);
    void onItemSelectionChanged();
    void onHeaderCheckboxToggled(int state);

private:
    QList<Column> m_columns;
    QCheckBox *m_headerCheckbox = nullptr;
    QMap<int, QString> m_columnKeyMap;
    bool m_updatingSelection = false;
};

#endif // TABLE_H
