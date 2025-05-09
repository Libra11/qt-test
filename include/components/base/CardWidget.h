/*
 * @Author: Libra
 * @Date: 2025-05-09 14:59:30
 * @LastEditors: Libra
 * @Description: 
 */
#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPixmap>

class CardWidget : public QWidget {
    Q_OBJECT
public:
    explicit CardWidget(QWidget *parent = nullptr);

    void setIcon(const QString &iconPath);  // 支持 SVG 或 PNG
    void setTitle(const QString &title);
    void setDescription(const QString &desc);

protected:
    void paintEvent(QPaintEvent *) override;

private:
    QLabel *iconLabel;
    QLabel *titleLabel;
    QLabel *descLabel;
};

#endif // CARDWIDGET_H
