#ifndef SYSTEMCONTROLHELPER_H
#define SYSTEMCONTROLHELPER_H

#pragma once

// 系统控制相关工具类
class SystemControlHelper {
public:
    // 禁用任务管理器
    static void DisableTaskManager();
    // 启用任务管理器
    static void EnableTaskManager();

    // 安装键盘钩子
    static void InstallHook();
    // 卸载键盘钩子
    static void UninstallHook();
};

#endif // SYSTEMCONTROLHELPER_H
