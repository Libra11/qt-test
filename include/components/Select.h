#ifndef SELECT_H
#define SELECT_H

#include <QComboBox>
#include <QString>
#include <QWidget>
#include <QVariant>

class Select : public QComboBox {
    Q_OBJECT
public:
    explicit Select(QWidget *parent = nullptr);

    // 添加选项
    void addOption(const QString &text, const QVariant &value = QVariant());
    // 设置当前值
    void setValue(const QVariant &value);
    // 获取当前值
    QVariant value() const;
};

#endif // SELECT_H 