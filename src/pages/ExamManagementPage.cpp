#include "pages/ExamManagementPage.h"
#include <QSplitter>
#include <QFrame>
#include <QLabel>
#include <QPixmap>
#include <QIcon>
#include <QDebug>
#include "components/base/DropDown.h"

ExamManagementPage::ExamManagementPage(QWidget *parent) : PageBase(parent)
{
    setupUI();
    loadTableData();
}

void ExamManagementPage::setupUI()
{
    // 创建主布局
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // 创建左侧面板和主内容区域
    setupLeftPanel();
    setupMainContent();
    
    // 添加到主布局
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(m_leftPanel);
    splitter->addWidget(m_mainContent);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 4);
    splitter->setHandleWidth(1);
    splitter->setStyleSheet("QSplitter::handle { background-color: #e0e0e0; }");
    
    mainLayout->addWidget(splitter);
}

void ExamManagementPage::setupLeftPanel()
{
    m_leftPanel = new QWidget(this);
    m_leftPanel->setObjectName("leftPanel");
    m_leftPanel->setStyleSheet("#leftPanel { background-color: white; border-right: 1px solid #e0e0e0; }");
    m_leftPanel->setMinimumWidth(200);
    m_leftPanel->setMaximumWidth(300);
    
    QVBoxLayout *leftLayout = new QVBoxLayout(m_leftPanel);
    leftLayout->setContentsMargins(20, 20, 20, 20);
    
    // 添加Logo
    QLabel *logoLabel = new QLabel(m_leftPanel);
    QPixmap logo(":/icons/logo.png");
    logoLabel->setPixmap(logo.scaled(160, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    leftLayout->addWidget(logoLabel);
    leftLayout->addSpacing(30);
    
    // 添加标题
    QLabel *titleLabel = new QLabel("考前封场", m_leftPanel);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    leftLayout->addWidget(titleLabel);
    leftLayout->addSpacing(20);
    
    // 添加图标
    QLabel *iconLabel = new QLabel(m_leftPanel);
    QPixmap icon(":/icons/exam.png");
    iconLabel->setPixmap(icon.scaled(160, 160, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    iconLabel->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(iconLabel);
    
    // 添加考点信息
    QLabel *siteInfoLabel = new QLabel("四川大学通识考点", m_leftPanel);
    siteInfoLabel->setAlignment(Qt::AlignCenter);
    QFont siteFont = siteInfoLabel->font();
    siteFont.setPointSize(14);
    siteInfoLabel->setFont(siteFont);
    leftLayout->addWidget(siteInfoLabel);
    
    QLabel *roomInfoLabel = new QLabel("东校区(146台)", m_leftPanel);
    roomInfoLabel->setAlignment(Qt::AlignCenter);
    roomInfoLabel->setStyleSheet("color: #666;");
    leftLayout->addWidget(roomInfoLabel);
    
    leftLayout->addStretch();
    
    // 添加底部按钮
    Button *startExamButton = new Button("切换到正式考试");
    startExamButton->setVariant(Button::Variant::Default);
    leftLayout->addWidget(startExamButton);

    Button *quit = new Button("退出账号");
    quit->setVariant(Button::Variant::Outline);
    leftLayout->addWidget(quit);
    
}

void ExamManagementPage::setupMainContent()
{
    m_mainContent = new QWidget(this);
    m_mainContent->setObjectName("mainContent");
    m_mainContent->setStyleSheet("#mainContent { background-color: #f5f5f5; }");
    
    QVBoxLayout *contentLayout = new QVBoxLayout(m_mainContent);
    contentLayout->setContentsMargins(20, 20, 20, 20);
    contentLayout->setSpacing(20);
    
    // 设置顶部标签页
    setupTopTabs();
    contentLayout->addWidget(m_topTabs);
    
    // 设置筛选标签页和操作按钮
    QWidget *filterActionWidget = new QWidget(m_mainContent);
    QHBoxLayout *filterActionLayout = new QHBoxLayout(filterActionWidget);
    filterActionLayout->setContentsMargins(0, 0, 0, 0);
    
    setupFilterTabs();
    setupActionButtons();
    
    filterActionLayout->addWidget(m_filterTabs);
    filterActionLayout->addStretch();
    
    QHBoxLayout *actionButtonsLayout = new QHBoxLayout();
    actionButtonsLayout->addWidget(m_batchExamButton);
    actionButtonsLayout->addWidget(m_relateProductButton);
    actionButtonsLayout->addWidget(m_refreshButton);
    actionButtonsLayout->addWidget(m_moreActionsCombo);
    
    filterActionLayout->addLayout(actionButtonsLayout);
    
    contentLayout->addWidget(filterActionWidget);
    
    // 设置表格
    setupTable();
    contentLayout->addWidget(m_table, 1); // 表格占据剩余空间
}

void ExamManagementPage::setupTopTabs()
{
    // 创建顶部标签页和主页按钮的容器
    QWidget *topTabsContainer = new QWidget(m_mainContent);
    QHBoxLayout *topTabsLayout = new QHBoxLayout(topTabsContainer);
    topTabsLayout->setContentsMargins(0, 0, 0, 0);
    
    // 创建顶部标签页
    m_topTabs = new Tab(topTabsContainer);
    m_topTabs->setStyle(Tab::Style::Primary);
    m_topTabs->addTab("exam", "考试机管理");
    m_topTabs->addTab("project", "项目检测");
    
    // 创建主页按钮
    m_homeButton = new Button(QIcon(":/icons/home.svg"), "主页");
    m_homeButton->setVariant(Button::Variant::Ghost);
    
    // 添加到布局
    topTabsLayout->addWidget(m_topTabs, 1);
    topTabsLayout->addWidget(m_homeButton);
}

void ExamManagementPage::setupFilterTabs()
{
    m_filterTabs = new Tab(m_mainContent);
    m_filterTabs->setStyle(Tab::Style::Secondary);
    m_filterTabs->addTab("all", "全部");
    m_filterTabs->addTab("online", "在线");
    m_filterTabs->addTab("offline", "离线");
    m_filterTabs->addTab("started", "已开启");
}

void ExamManagementPage::setupActionButtons()
{
    // 批量考试按钮
    m_batchExamButton = new Button(QIcon(":/icons/exam.svg"), "批量考试");
    m_batchExamButton->setVariant(Button::Variant::Default);
    
    // 关联产品按钮
    m_relateProductButton = new Button(QIcon(":/icons/link.svg"), "关联产品");
    m_relateProductButton->setVariant(Button::Variant::Default);
    
    // 刷新按钮
    m_refreshButton = new Button(QIcon(":/icons/refresh.svg"), "刷新");
    m_refreshButton->setVariant(Button::Variant::Default);
    
    // 更多操作下拉菜单
    m_moreActionsCombo = new DropDown(m_mainContent);
    m_moreActionsCombo->addOption("更多");
    m_moreActionsCombo->addOption("导出");
    m_moreActionsCombo->addOption("批量删除");
    m_moreActionsCombo->addOption("批量重启");
}

void ExamManagementPage::setupTable()
{
    m_table = new Table(m_mainContent);
    
    // 设置列
    QList<Table::Column> columns = {
        {"machineNo", "机位号", 100, true},
        {"ipAddress", "IP地址", 150, true},
        {"status", "状态", 100, true},
        {"version", "版本号", 100, true},
        {"operation", "操作", 150, false}
    };
    
    m_table->setColumns(columns);
}

void ExamManagementPage::loadTableData()
{
    // 模拟数据
    QList<QMap<QString, QVariant>> data;
    
    // 添加一些示例数据
    for (int i = 1; i <= 10; i++) {
        QMap<QString, QVariant> row;
        row["machineNo"] = QString::number(i);
        row["ipAddress"] = QString("192.168.1.%1").arg(i);
        row["status"] = (i % 3 == 0) ? "离线" : "在线";
        row["version"] = "v1.0.0";
        row["operation"] = "操作";
        data.append(row);
    }
    
    m_table->setData(data);
}
