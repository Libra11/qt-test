// Select.cpp
#include "components/base/Select.h"
#include "components/base/ColorManager.h"

#include <QListView>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QFontMetrics>

namespace {
    class OptionDelegate : public QStyledItemDelegate {
    public:
        OptionDelegate(QObject* parent = nullptr)
            : QStyledItemDelegate(parent),
              m_left(12), m_top(4), m_right(12), m_bottom(4)
        {
            // 放在 Select 的构造函数里，setView 之前
            QPixmap pm(":/icons/arrow.svg");
            // 父控件是 QListView，想同步它的字体
            if (auto *lv = qobject_cast<QListView*>(parent)) {
                m_font = lv->font();
            }
        }

        QSize sizeHint(const QStyleOptionViewItem & /*opt*/, const QModelIndex & /*idx*/) const override {
            // 用我们的字体来测量文字高度
            QFontMetrics fm(m_font);
            int h = fm.height() + m_top + m_bottom;
            // 宽度不管，由 view 自己决定
            return QSize(0, h);
        }

        void paint(QPainter *painter,
                   const QStyleOptionViewItem &origOpt,
                   const QModelIndex &index) const override
        {
            // 拷贝 option
            QStyleOptionViewItem opt(origOpt);
            initStyleOption(&opt, index);

            QRect rect = origOpt.rect;

            // 1) 白底擦底
            painter->fillRect(rect, QColor(ColorManager::white()));

            // 2) hover/selected 背景
            if (origOpt.state & QStyle::State_Selected) {
                painter->fillRect(rect, QColor(ColorManager::primaryHover()));
                opt.palette.setColor(QPalette::Text, QColor(ColorManager::white()));
            }
            else if (origOpt.state & QStyle::State_MouseOver) {
                painter->fillRect(rect, QColor(ColorManager::primaryHover()));
                opt.palette.setColor(QPalette::Text, QColor(ColorManager::white()));
            }

            // 3) 清掉 QStyle 绘制状态
            opt.state &= ~QStyle::State_Selected;
            opt.state &= ~QStyle::State_MouseOver;

            // 4) 使用我们缓存的字体
            opt.font = m_font;

            // 5) 留出 padding 区域
            opt.rect = rect.adjusted(m_left, m_top, -m_right, -m_bottom);

            // 6) 最后由基类绘制文本和图标
            QStyledItemDelegate::paint(painter, opt, index);
        }

    private:
        const int m_left, m_top, m_right, m_bottom;
        QFont m_font;
    };
}

Select::Select(QWidget *parent)
    : QComboBox(parent)
{
    setMinimumHeight(50);

    // 强制使用 QListView，并打开鼠标追踪
    auto *lv = new QListView(this);
    lv->setMouseTracking(true);
    lv->viewport()->setMouseTracking(true);

    // 安装我们的委托
    lv->setItemDelegate(new OptionDelegate(lv));
    setView(lv);

    // ComboBox 本体样式（只负责边框、圆角、padding）
    setStyleSheet(QString(R"(
QComboBox {
    border: 1px solid %1;
    border-radius: 6px;
    padding: 6px 12px;
    font-size: 16px;
    background: %2;
}
QComboBox::drop-down {
    subcontrol-origin: padding;
    subcontrol-position: top right;
    border: none;
    background: transparent;
    width: 32px;
}
QComboBox::down-arrow {
  image: url(:/icons/arrow.svg);
  width: 16px;
  height: 16px;
}
    )")
    .arg(ColorManager::outline(),
         ColorManager::white()));

    // 给下拉列表一个统一背景 + 边框
    lv->setStyleSheet(QString(R"(
QListView {
    background: %1;
    border: 1px solid %2;
    outline: none;
}
    )")
    .arg(ColorManager::white(),
         ColorManager::outline()));
}

void Select::addOption(const QString &text, const QVariant &value)
{
    addItem(text, value);
}

void Select::setValue(const QVariant &value)
{
    int idx = findData(value);
    if (idx >= 0)
        setCurrentIndex(idx);
}

QVariant Select::value() const
{
    return currentData();
}
