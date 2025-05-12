// Table.cpp - Implementation for enhanced Table widget
#include "components/base/Table.h"
#include <QHeaderView>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QDebug>

Table::Table(QWidget *parent) : QTableWidget(parent)
{
    setupUI();
}

void Table::setupUI()
{
    setFrameShape(QFrame::NoFrame);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::MultiSelection);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setAlternatingRowColors(true);
    setShowGrid(false);

    setStyleSheet(R"(
        QTableWidget {
            background-color: #fff;
            alternate-background-color: #f9f9f9;
            border: 1px solid #ddd;
            border-radius: 6px;
        }
        QTableWidget::item {
            padding: 6px;
            border-bottom: 1px solid #eee;
        }
        QHeaderView::section {
            background-color: #f3f3f3;
            padding: 8px;
            font-weight: bold;
            border: none;
            border-bottom: 1px solid #ccc;
        }
        QTableWidget::item:selected {
            background-color: #dbeeff;
        }
    )");

    horizontalHeader()->setStretchLastSection(true);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    verticalHeader()->setVisible(false);

    connect(horizontalHeader(), &QHeaderView::sectionClicked, this, &Table::onHeaderClicked);
    connect(this, &QTableWidget::itemSelectionChanged, this, &Table::onItemSelectionChanged);
}

void Table::setColumns(const QList<Column> &columns)
{
    m_columns = columns;
    setColumnCount(columns.size());

    QStringList headers;
    for (int i = 0; i < columns.size(); ++i) {
        headers << columns[i].title;
        setColumnWidth(i, columns[i].width);
        m_columnKeyMap[i] = columns[i].key;
    }

    setHorizontalHeaderLabels(headers);
    setupHeaderCheckbox();
    setSortingEnabled(true);
    horizontalHeader()->setSortIndicatorShown(true);
}

void Table::setData(const QList<QMap<QString, QVariant>> &data)
{
    clearContents();
    setRowCount(data.size());

    for (int row = 0; row < data.size(); ++row) {
        for (int col = 0; col < m_columns.size(); ++col) {
            const QString &key = m_columns[col].key;
            QTableWidgetItem *item = new QTableWidgetItem(data[row].value(key).toString());
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            setItem(row, col, item);
        }
    }
    updateHeaderCheckbox();
}

QList<int> Table::getSelectedRows() const
{
    QList<int> selectedRows;
    for (int row = 0; row < rowCount(); ++row) {
        for (int col = 0; col < columnCount(); ++col) {
            QTableWidgetItem *it = item(row, col);
            if (it && it->isSelected()) {
                selectedRows.append(row);
                break;
            }
        }
    }
    return selectedRows;
}

void Table::clearSelection()
{
    QTableWidget::clearSelection();
    updateHeaderCheckbox();
}

void Table::setupHeaderCheckbox()
{
    if (!m_headerCheckbox) {
        m_headerCheckbox = new QCheckBox(this);
        m_headerCheckbox->setTristate(true);
        m_headerCheckbox->setStyleSheet("margin-left:6px");

        connect(m_headerCheckbox, &QCheckBox::stateChanged, this, &Table::onHeaderCheckboxToggled);

        QWidget *container = new QWidget(this);
        QHBoxLayout *layout = new QHBoxLayout(container);
        layout->addWidget(m_headerCheckbox);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setAlignment(Qt::AlignCenter);

        setHorizontalHeaderItem(0, new QTableWidgetItem(""));
        horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
        setCellWidget(-1, 0, container); // NOTE: fake placeholder, custom header preferred
    }
}

void Table::updateHeaderCheckbox()
{
    if (!m_headerCheckbox) return;

    int total = rowCount();
    int checked = getSelectedRows().count();

    if (checked == 0) {
        m_headerCheckbox->setCheckState(Qt::Unchecked);
    } else if (checked == total) {
        m_headerCheckbox->setCheckState(Qt::Checked);
    } else {
        m_headerCheckbox->setCheckState(Qt::PartiallyChecked);
    }
}

void Table::onHeaderCheckboxToggled(int state)
{
    if (m_updatingSelection) return;

    bool checked = (state == Qt::Checked);

    m_updatingSelection = true;
    blockSignals(true);

    for (int row = 0; row < rowCount(); ++row) {
        for (int col = 0; col < columnCount(); ++col) {
            if (QTableWidgetItem *it = item(row, col)) {
                it->setSelected(checked);
            }
        }
    }

    blockSignals(false);
    m_updatingSelection = false;
    emit selectionChanged(getSelectedRows());
}

void Table::onHeaderClicked(int index)
{
    if (index < 0 || index >= m_columns.size()) return;
    if (!m_columns[index].sortable) return;

    Qt::SortOrder order = horizontalHeader()->sortIndicatorOrder();
    sortByColumn(index, order);
    emit sortChanged(m_columns[index].key, order);
}

void Table::onItemSelectionChanged()
{
    if (m_updatingSelection) return;
    updateHeaderCheckbox();
    emit selectionChanged(getSelectedRows());
}
