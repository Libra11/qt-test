/*
 * @Author: Libra
 * @Date: 2025-05-12 13:55:29
 * @LastEditors: Libra
 * @Description: 
 */
#include "components/base/Tab.h"
#include <QDebug>

Tab::Tab(QWidget *parent) : QWidget(parent), m_currentTab(""), m_style(Style::Primary)
{
    setupUI();
}

void Tab::setupUI()
{
    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    setLayout(m_layout);
}

void Tab::addTab(const QString &key, const QString &text)
{
    if (m_tabs.contains(key)) {
        qDebug() << "Tab with key" << key << "already exists";
        return;
    }

    QPushButton *tab = new QPushButton(text, this);
    tab->setCheckable(true);
    tab->setAutoExclusive(true);
    
    // 如果是第一个标签，设置为选中状态
    if (m_tabs.isEmpty()) {
        tab->setChecked(true);
        m_currentTab = key;
    }
    
    connect(tab, &QPushButton::clicked, [this, key]() {
        m_currentTab = key;
        emit tabChanged(key);
    });
    
    m_tabs[key] = tab;
    m_layout->addWidget(tab);
    
    updateTabStyle();
}

void Tab::setStyle(Style style)
{
    m_style = style;
    updateTabStyle();
}

QString Tab::currentTab() const
{
    return m_currentTab;
}

void Tab::updateTabStyle()
{
    QString baseStyle;
    QString checkedStyle;
    
    if (m_style == Style::Primary) {
        baseStyle = "QPushButton { \
            background-color: transparent; \
            border: none; \
            padding: 10px 20px; \
            font-size: 14px; \
            color: #666; \
        } \
        QPushButton:hover { \
            color: #1890ff; \
        }";
        
        checkedStyle = "QPushButton:checked { \
            color: #1890ff; \
            border-bottom: 2px solid #1890ff; \
            font-weight: bold; \
        }";
    } else {
        baseStyle = "QPushButton { \
            background-color: transparent; \
            border: 1px solid #d9d9d9; \
            border-radius: 4px; \
            padding: 5px 12px; \
            margin-right: 8px; \
            font-size: 12px; \
            color: #666; \
        } \
        QPushButton:hover { \
            color: #1890ff; \
            border-color: #1890ff; \
        }";
        
        checkedStyle = "QPushButton:checked { \
            color: #1890ff; \
            background-color: #e6f7ff; \
            border-color: #1890ff; \
        }";
    }
    
    setStyleSheet(baseStyle + checkedStyle);
}
