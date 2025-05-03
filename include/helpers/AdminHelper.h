#ifndef ADMINHELPER_H
#define ADMINHELPER_H

#pragma once
#include <windows.h>
#include <QMessageBox>

class AdminHelper {
public:
    // 判断是否为管理员
    static bool IsRunningAsAdmin();

    // 尝试提升为管理员
    static bool TryRunAsAdmin();

    // 请求管理员权限（循环直到用户同意或者拒绝到底）
    static void RequestAdminLoop();
};

#endif // ADMINHELPER_H
