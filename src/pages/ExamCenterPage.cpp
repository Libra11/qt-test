/*
 * @Author: Libra
 * @Date: 2025-05-15 10:30:00
 * @LastEditors: Libra
 * @Description: 考试中心页面
 */
#include "pages/ExamCenterPage.h"
#include <QGridLayout>
#include <QFrame>
#include "helpers/NetworkHelper.h"
#include "components/base/CardWidget.h"
#include "components/base/CustomMessageBox.h"
#include "helpers/SettingsHelper.h"
#include "api/ExamApi.h"

ExamCenterPage::ExamCenterPage(QWidget *parent) : PageBase(parent),
    m_titleLabel(nullptr),
    m_examPointValue(nullptr),
    m_examCapacityValue(nullptr)
{
    setupUI();
}

// 添加一个新方法，在页面显示时调用
void ExamCenterPage::showEvent(QShowEvent *event)
{
    PageBase::showEvent(event);
    loadSiteAndRoomInfo();
}

void ExamCenterPage::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    // 头部信息
    createExamHeaderSection();
    mainLayout->addSpacing(20);

    // 标题
    m_titleLabel = new QLabel("加载中...", this);
    QFont titleFont = m_titleLabel->font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_titleLabel);

    // 考点信息部分
    createExamInfoSection();
    mainLayout->addSpacing(20);

    // 考试阶段部分
    createExamStagesSection();
    mainLayout->addStretch();
}

void ExamCenterPage::createExamHeaderSection()
{
    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(20);
    headerLayout->setContentsMargins(50, 0, 50, 0);

    // 返回按钮
    Button *backBtn = new Button(QIcon(":/icons/arrow.svg"), "返回登录页");
    backBtn->setVariant(Button::Variant::Ghost);
    QObject::connect(backBtn, &Button::clicked, [=](){
        emit routeTo("login");
    });
    headerLayout->addWidget(backBtn, 0, Qt::AlignLeft);

    // 返回按钮
    Button *backBtn2 = new Button(QIcon(":/icons/arrow.svg"), "返回登录页");
    backBtn2->setVariant(Button::Variant::Ghost);
    QObject::connect(backBtn2, &Button::clicked, [=](){
        NetworkHelper::instance()->setToken("");
        emit routeTo("login");
    });
    headerLayout->addStretch(1);
    headerLayout->addWidget(backBtn2, 0, Qt::AlignLeft);

    // 添加到主布局
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(layout());
    if (mainLayout) {
        mainLayout->addLayout(headerLayout);
    }
}

void ExamCenterPage::createExamInfoSection()
{
    // 创建一个水平布局用于考点信息表格
    QHBoxLayout *infoLayout = new QHBoxLayout();
    infoLayout->setContentsMargins(50, 0, 50, 0);

    // 创建表格布局
    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setSpacing(0);

    // 表头样式
    QString headerStyle = "background-color: #f5f5f5; padding: 10px; border: 1px solid #e0e0e0; font-weight: bold;";
    QString cellStyle = "padding: 10px; border: 1px solid #e0e0e0;";

    // 添加表头
    QLabel *examPointHeader = new QLabel("考点");
    examPointHeader->setStyleSheet(headerStyle);
    examPointHeader->setAlignment(Qt::AlignCenter);
    gridLayout->addWidget(examPointHeader, 0, 0);

    QLabel *examCapacityHeader = new QLabel("考试机位");
    examCapacityHeader->setStyleSheet(headerStyle);
    examCapacityHeader->setAlignment(Qt::AlignCenter);
    gridLayout->addWidget(examCapacityHeader, 0, 1);

    // 添加数据
    m_examPointValue = new QLabel("加载中...");
    m_examPointValue->setStyleSheet(cellStyle);
    m_examPointValue->setAlignment(Qt::AlignCenter);
    gridLayout->addWidget(m_examPointValue, 1, 0);

    m_examCapacityValue = new QLabel("加载中...");
    m_examCapacityValue->setStyleSheet(cellStyle);
    m_examCapacityValue->setAlignment(Qt::AlignCenter);
    gridLayout->addWidget(m_examCapacityValue, 1, 1);

    // 设置列宽比例
    gridLayout->setColumnStretch(0, 2);
    gridLayout->setColumnStretch(1, 1);

    infoLayout->addStretch();
    infoLayout->addLayout(gridLayout);
    infoLayout->addStretch();

    // 添加到主布局
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(layout());
    if (mainLayout) {
        mainLayout->addLayout(infoLayout);
    }
}

void ExamCenterPage::createExamStagesSection()
{
    // 创建水平布局用于考试阶段卡片
    QHBoxLayout *stagesLayout = new QHBoxLayout();
    stagesLayout->setSpacing(20);
    stagesLayout->setContentsMargins(50, 0, 50, 0);

    CardWidget *card = new CardWidget;
    card->setIcon(":/icons/arrow.svg");
    card->setTitle("考前封场");
    card->setDescription("考试机管理：注册、编排、管理考试机\n考前准备：机位数、版本、摄像头检测、试考、封场");

    CardWidget *card2 = new CardWidget;
    card2->setIcon(":/icons/arrow.svg");
    card2->setTitle("考前封场");
    card2->setDescription("考试机管理：注册、编排、管理考试机\n考前准备：机位数、版本、摄像头检测、试考、封场");
    stagesLayout->addWidget(card);
    stagesLayout->addWidget(card2);

    // 添加到主布局
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(layout());
    if (mainLayout) {
        mainLayout->addLayout(stagesLayout);
    }
}

void ExamCenterPage::loadSiteAndRoomInfo()
{
    // 从服务器获取最新的考点信息
    ExamApi::getSiteInfo(
        [this](QJsonObject obj) {
            if (obj["code"].toInt() == 0) {
                QJsonObject data = obj["data"].toObject();

                // 解析数据到结构体
                m_siteInfo.siteId = data["siteId"].toString();
                m_siteInfo.name = data["name"].toString();
                m_siteInfo.province = data["province"].toString();
                m_siteInfo.city = data["city"].toString();
                m_siteInfo.priority = data["priority"].toInt();
                m_siteInfo.district = data["district"].toString();
                m_siteInfo.address = data["address"].toString();
                m_siteInfo.traffic = data["traffic"].toString();
                m_siteInfo.manager = data["manager"].toString();
                m_siteInfo.managerMobile = data["managerMobile"].toString();
                m_siteInfo.itContract = data["itContract"].toString();
                m_siteInfo.itMobile = data["itMobile"].toString();
                m_siteInfo.note = data["note"].toString();
                m_siteInfo.locked = data["locked"].toBool();

                // 保存到本地
                SettingsHelper::saveSiteInfo(m_siteInfo);

                // 获取考场信息
                loadRoomInfo();
            } else {
                CustomMessageBox::warning(this, "获取考点信息失败", obj["message"].toString());
            }
        },
        [this](QString err) {
            CustomMessageBox::warning(this, "网络错误", "获取考点信息失败: " + err);

            // 尝试获取考场信息
            loadRoomInfo();
        }
    );
}

// 辅助方法：加载考场信息
void ExamCenterPage::loadRoomInfo()
{
    ExamApi::getRoomInfo(
        [this](QJsonObject obj) {
            if (obj["code"].toInt() == 0) {
                QJsonObject data = obj["data"].toObject();

                // 解析数据到结构体
                m_roomInfo.siteId = data["siteId"].toString();
                m_roomInfo.roomId = data["roomId"].toString();
                m_roomInfo.name = data["name"].toString();
                m_roomInfo.sort = data["sort"].toInt();
                m_roomInfo.address = data["address"].toString();
                m_roomInfo.note = data["note"].toString();
                m_roomInfo.capacity = data["capacity"].toInt();
                m_roomInfo.available = data["available"].toInt();
                m_roomInfo.loginCode = data["loginCode"].toString();
                m_roomInfo.cloudSystem = data["cloudSystem"].toBool();
                m_roomInfo.localCamera = data["localCamera"].toBool();

                // 保存到本地
                SettingsHelper::saveRoomInfo(m_roomInfo);

                // 更新UI
                updateUIWithSiteAndRoomInfo();
            } else {
                CustomMessageBox::warning(this, "获取考场信息失败", obj["message"].toString());
            }
        },
        [this](QString err) {
            CustomMessageBox::warning(this, "网络错误", "获取考场信息失败: " + err);
        }
    );
}

// 辅助方法：更新UI显示
void ExamCenterPage::updateUIWithSiteAndRoomInfo()
{
    if (m_titleLabel && !m_siteInfo.name.isEmpty() && !m_roomInfo.address.isEmpty()) {
        // 标题： site.name + room.address
        m_titleLabel->setText(m_siteInfo.name + m_roomInfo.address);
    }

    if (m_examPointValue && !m_siteInfo.name.isEmpty()) {
        // 考点： site.name
        m_examPointValue->setText(m_siteInfo.name);
    }

    if (m_examCapacityValue && m_roomInfo.available > 0) {
        // 考试机位：room.available
        m_examCapacityValue->setText(QString::number(m_roomInfo.available) + "台");
    }
}
