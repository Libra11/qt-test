#ifndef CONTENTPROTECTIONWIDGET_H
#define CONTENTPROTECTIONWIDGET_H

#pragma once
#include <QWidget>

class ContentProtectionHelper {
public:
    // 设置内容保护
    static void setContentProtection(QWidget* w, bool enable);
};
#endif // CONTENTPROTECTIONWIDGET_H
