#ifndef DROPDOWN_H
#define DROPDOWN_H

#include <QComboBox>
#include <QString>
#include <QWidget>
#include <QVariant>

class DropDown : public QComboBox {
    Q_OBJECT
public:
    explicit DropDown(QWidget *parent = nullptr);

    // 添加选项
    void addOption(const QString &text, const QVariant &value = QVariant());
    // 设置当前值
    void setValue(const QVariant &value);
    // 获取当前值
    QVariant value() const;
};

#endif // DROPDOWN_H 