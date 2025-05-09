/*
 * @Author: Libra
 * @Date: 2025-05-15 10:30:00
 * @LastEditors: Libra
 * @Description: 考试中心页面
 */
#ifndef EXAMCENTERPAGE_H
#define EXAMCENTERPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include "components/base/Button.h"
#include "PageBase.h"
#include "api/ExamApi.h"

class ExamCenterPage : public PageBase
{
    Q_OBJECT
public:
    explicit ExamCenterPage(QWidget *parent = nullptr);

protected:
    void showEvent(QShowEvent *event) override;

private:
    void setupUI();
    void createExamHeaderSection();
    void createExamInfoSection();
    void createExamStagesSection();
    void loadSiteAndRoomInfo();
    void loadRoomInfo();
    void updateUIWithSiteAndRoomInfo();

    // 页面元素
    QLabel *m_titleLabel;
    QLabel *m_examPointValue;
    QLabel *m_examCapacityValue;

    // 数据
    ISite m_siteInfo;
    IRoom m_roomInfo;
};

#endif // EXAMCENTERPAGE_H
