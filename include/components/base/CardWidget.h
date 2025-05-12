#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>

class CardWidget : public QWidget {
    Q_OBJECT
public:
    explicit CardWidget(QWidget *parent = nullptr);

    void setIcon(const QString &iconPath);  // 支持 SVG 或 PNG
    void setTitle(const QString &title);
    void setDescription(const QString &desc);

protected:
    void paintEvent(QPaintEvent *event) override;
signals:
    void clicked();
protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    QLabel *iconLabel;
    QLabel *titleLabel;
    QLabel *descLabel;
};

#endif // CARDWIDGET_H
