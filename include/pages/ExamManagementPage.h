/*
 * @Author: Libra
 * @Date: 2025-05-12 13:57:13
 * @LastEditors: Libra
 * @Description: 
 */
#ifndef EXAMMANAGEMENTPAGE_H
#define EXAMMANAGEMENTPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include "pages/PageBase.h"
#include "components/base/Button.h"
#include "components/base/Tab.h"
#include "components/base/Table.h"
#include "components/base/DropDown.h"

class ExamManagementPage : public PageBase
{
    Q_OBJECT

public:
    explicit ExamManagementPage(QWidget *parent = nullptr);
    virtual ~ExamManagementPage() = default;

private:
    void setupUI();
    void setupLeftPanel();
    void setupMainContent();
    void setupTopTabs();
    void setupFilterTabs();
    void setupActionButtons();
    void setupTable();
    void loadTableData();

private:
    QWidget *m_leftPanel;
    QWidget *m_mainContent;
    
    // UI组件
    Tab *m_topTabs;
    Tab *m_filterTabs;
    Button *m_homeButton;
    Button *m_batchExamButton;
    Button *m_relateProductButton;
    Button *m_refreshButton;
    DropDown *m_moreActionsCombo;
    Table *m_table;
};

#endif // EXAMMANAGEMENTPAGE_H

