/*
 * @Author: Libra
 * @Date: 2025-05-06 14:44:11
 * @LastEditors: Libra
 * @Description: 
 */
#include "pages/PageManager.h"

PageManager::PageManager(QStackedWidget* stack, QObject* parent)
    : QObject(parent), m_stack(stack)
{}

void PageManager::registerPage(const QString& name, QWidget* page)
{
    int idx = m_stack->addWidget(page);
    m_pageMap[name] = idx;
}

void PageManager::switchTo(const QString& name)
{
    if (m_pageMap.contains(name))
        m_stack->setCurrentIndex(m_pageMap[name]);
}

QWidget* PageManager::getPage(const QString& name) const
{
    if (m_pageMap.contains(name))
        return m_stack->widget(m_pageMap[name]);
    return nullptr;
}

QMap<QString, int> PageManager::getPageMap() const
{
    return m_pageMap;
} 