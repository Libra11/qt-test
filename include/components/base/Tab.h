/*
 * @Author: Libra
 * @Date: 2025-05-12 13:54:40
 * @LastEditors: Libra
 * @Description: 
 */
#ifndef TAB_H
#define TAB_H

#include <QWidget>
#include <QMap>
#include <QString>
#include <QHBoxLayout>
#include <QPushButton>

class Tab : public QWidget
{
    Q_OBJECT
public:
    enum class Style {
        Primary,
        Secondary
    };

    explicit Tab(QWidget *parent = nullptr);
    virtual ~Tab() = default;

    void addTab(const QString &key, const QString &text);
    void setStyle(Style style);
    QString currentTab() const;

signals:
    void tabChanged(const QString &key);

private:
    void setupUI();
    void updateTabStyle();

private:
    QHBoxLayout *m_layout;
    QMap<QString, QPushButton*> m_tabs;
    QString m_currentTab;
    Style m_style;
};

#endif // TAB_H
