#include "pages/PageManager.h"
#include <functional>
#include <QMap>

PageManager::PageManager(QStackedWidget* stack, QObject* parent)
    : QObject(parent), m_stack(stack)
{
    // 初始化页面工厂和页面映射
    m_pageFactories.clear();
    m_pageMap.clear();
}

void PageManager::registerPage(const QString& name, std::function<PageBase*()> pageFactory)
{
    // 存储页面工厂函数，不立即创建页面
    m_pageFactories[name] = pageFactory;
}

void PageManager::switchTo(const QString& name)
{
    // 检查页面是否已创建
    if (!m_pageMap.contains(name)) {
        // 如果页面未创建但有工厂函数，则创建页面
        if (m_pageFactories.contains(name)) {
            // 使用工厂函数创建页面
            PageBase* page = m_pageFactories[name]();

            // 添加页面到堆栈
            m_stack->addWidget(page);
            m_pageMap[name] = m_stack->indexOf(page);

            // 连接页面的 routeTo 信号
            connect(page, &PageBase::routeTo, this, [=](const QString& pageName){
                this->switchTo(pageName);
            });
        } else {
            // 如果没有对应的工厂函数，则无法切换
            return;
        }
    }

    // 切换到指定页面
    m_stack->setCurrentIndex(m_pageMap[name]);
}

QWidget* PageManager::getPage(const QString& name) const
{
    // 如果页面已创建，直接返回
    if (m_pageMap.contains(name)) {
        return m_stack->widget(m_pageMap[name]);
    }

    // 注意：这里不创建页面，因为 getPage 是 const 方法
    // 如果需要获取未创建的页面，应该先调用 switchTo
    return nullptr;
}
