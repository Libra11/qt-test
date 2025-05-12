#include "layout/FlowLayout.h"
#include <QtWidgets>

FlowLayout::FlowLayout(QWidget *parent, int margin, int hSpacing, int vSpacing)
    : QLayout(parent), m_hSpace(hSpacing), m_vSpace(vSpacing)
{
    setContentsMargins(margin, margin, margin, margin);
}

FlowLayout::~FlowLayout()
{
    QLayoutItem *item;
    while ((item = takeAt(0)))
        delete item;
}

void FlowLayout::addItem(QLayoutItem *item)
{
    itemList.append(item);
}

int FlowLayout::horizontalSpacing() const
{
    return (m_hSpace >= 0) ? m_hSpace : smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
}

int FlowLayout::verticalSpacing() const
{
    return (m_vSpace >= 0) ? m_vSpace : smartSpacing(QStyle::PM_LayoutVerticalSpacing);
}

int FlowLayout::count() const
{
    return itemList.size();
}

QLayoutItem *FlowLayout::itemAt(int index) const
{
    return itemList.value(index);
}

QLayoutItem *FlowLayout::takeAt(int index)
{
    return index >= 0 && index < itemList.size() ? itemList.takeAt(index) : nullptr;
}

Qt::Orientations FlowLayout::expandingDirections() const
{
    return {};
}

bool FlowLayout::hasHeightForWidth() const
{
    return true;
}

int FlowLayout::heightForWidth(int width) const
{
    return doLayout(QRect(0, 0, width, 0), true);
}

QSize FlowLayout::minimumSize() const
{
    QSize size;
    const QList<QLayoutItem *> &items = itemList;
    for (QLayoutItem *item : items)
        size = size.expandedTo(item->minimumSize());
    size += QSize(2 * margin(), 2 * margin());
    return size;
}

void FlowLayout::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);
    doLayout(rect, false);
}

QSize FlowLayout::sizeHint() const
{
    return minimumSize();
}

int FlowLayout::doLayout(const QRect &rect, bool testOnly) const
{
    int x = rect.x();
    int y = rect.y();
    int lineHeight = 0;
    const QList<QLayoutItem *> &items = itemList;

    for (QLayoutItem *item : items) {
        int spaceX = horizontalSpacing();
        int spaceY = verticalSpacing();
        int nextX = x + item->sizeHint().width() + spaceX;
        if (nextX - spaceX > rect.right() && lineHeight > 0) {
            x = rect.x();
            y += lineHeight + spaceY;
            nextX = x + item->sizeHint().width() + spaceX;
            lineHeight = 0;
        }

        if (!testOnly)
            item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));

        x = nextX;
        lineHeight = qMax(lineHeight, item->sizeHint().height());
    }
    return y + lineHeight - rect.y();
}

int FlowLayout::smartSpacing(QStyle::PixelMetric pm) const
{
    QObject *parentObj = this->parent();
    if (!parentObj)
        return -1;

    if (parentObj->isWidgetType()) {
        QWidget *parentWidget = static_cast<QWidget *>(parentObj);
        return parentWidget->style()->pixelMetric(pm, nullptr, parentWidget);
    }

    // fallback: 返回默认值
    return 6; // 或者你可以 return -1;
}

