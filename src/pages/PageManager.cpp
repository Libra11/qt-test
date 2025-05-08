#include "pages/PageManager.h"

PageManager::PageManager(QStackedWidget* stack, QObject* parent)
    : QObject(parent), m_stack(stack)
{}

void PageManager::registerPage(const QString& name, PageBase* page)
{
    m_stack->addWidget(page);
    m_pageMap[name] = m_stack->indexOf(page);

    // 统一监听 routeTo
    connect(page, &PageBase::routeTo, this, [=](const QString& pageName){
        this->switchTo(pageName);
    });
}

void PageManager::switchTo(const QString& name)
{
    if (m_pageMap.contains(name)) {
        m_stack->setCurrentIndex(m_pageMap[name]);
    }
}

QWidget* PageManager::getPage(const QString& name) const
{
    if (m_pageMap.contains(name)) {
        return m_stack->widget(m_pageMap[name]);
    }
    return nullptr;
}
