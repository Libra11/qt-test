#include "components/base/CardWidget.h"
#include <QSvgWidget>
#include <QFileInfo>
#include <QPainterPath>
#include <QStyleOption>
#include <QSvgRenderer>

CardWidget::CardWidget(QWidget *parent) : QWidget(parent) {
    this->setFixedHeight(140);
    this->setObjectName("card");
    this->setStyleSheet(R"(
        QLabel#card {
            color: #333;
        }
        QLabel#titleLabel {
            font-weight: bold;
            font-size: 18px;
        }
        QLabel#descLabel {
            font-size: 14px;
            color: #666;
        }
    )");

    // 圆角 + 阴影背景
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setAttribute(Qt::WA_Hover, true);  // 必须要加这行
    this->setStyleSheet(this->styleSheet() + R"(
        QWidget#card {
            border: 1px solid #eee;
            border-radius: 12px;
            background: #fff;
        }
        QWidget#card:hover {
            background: #eee;
        }
    )");
    iconLabel = new QLabel;
    iconLabel->setFixedSize(100, 100);
    iconLabel->setScaledContents(true);

    titleLabel = new QLabel("标题");
    titleLabel->setObjectName("titleLabel");

    descLabel = new QLabel("描述内容");
    descLabel->setWordWrap(true);
    descLabel->setObjectName("descLabel");

    QVBoxLayout *textLayout = new QVBoxLayout;
    textLayout->addWidget(titleLabel);
    textLayout->addWidget(descLabel);
    textLayout->setSpacing(6);
    textLayout->setContentsMargins(0, 10, 10, 10);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(iconLabel, 0, Qt::AlignCenter);
    mainLayout->addLayout(textLayout);
    mainLayout->setContentsMargins(15, 10, 15, 10);
    mainLayout->setSpacing(20);
}

void CardWidget::setIcon(const QString &iconPath) {
    QFileInfo info(iconPath);
    if (info.suffix() == "svg") {
        // 使用 svg 显示库，如果是 svg 的话
        QPixmap pix(iconLabel->size());
        pix.fill(Qt::transparent);
        QPainter painter(&pix);
        QSvgRenderer renderer(iconPath);
        renderer.render(&painter);
        iconLabel->setPixmap(pix);
    } else {
        iconLabel->setPixmap(QPixmap(iconPath));
    }
}

void CardWidget::setTitle(const QString &title) {
    titleLabel->setText(title);
}

void CardWidget::setDescription(const QString &desc) {
    descLabel->setText(desc);
}

void CardWidget::paintEvent(QPaintEvent *event) {
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
void CardWidget::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
      emit clicked();
  }
  QWidget::mousePressEvent(event); // 保持默认行为
}

