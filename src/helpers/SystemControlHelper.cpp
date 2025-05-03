/*
 * @Author: Libra
 * @Date: 2025-05-03 03:29:52
 * @LastEditors: Libra
 * @Description: 
 */
#include "helpers/SystemControlHelper.h"
#include <windows.h>

// 禁用任务管理器
void SystemControlHelper::DisableTaskManager() {
    HKEY hKey;
    RegCreateKeyEx(HKEY_CURRENT_USER,
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
        0, nullptr, 0, KEY_WRITE, nullptr, &hKey, nullptr);
    DWORD v = 1;
    RegSetValueEx(hKey, L"DisableTaskMgr", 0, REG_DWORD, (BYTE*)&v, sizeof(v));
    RegCloseKey(hKey);
}

// 启用任务管理器
void SystemControlHelper::EnableTaskManager() {
    HKEY hKey;
    RegCreateKeyEx(HKEY_CURRENT_USER,
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
        0, nullptr, 0, KEY_WRITE, nullptr, &hKey, nullptr);
    RegDeleteValue(hKey, L"DisableTaskMgr");
    RegCloseKey(hKey);
}

// 全局钩子句柄
static HHOOK g_hk = nullptr;

// 低级键盘钩子回调
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && wParam == WM_KEYDOWN) {
        auto p = (KBDLLHOOKSTRUCT*)lParam;
        if (p->vkCode == VK_LWIN || p->vkCode == VK_RWIN) return 1;
        bool alt = (GetAsyncKeyState(VK_MENU) & 0x8000);
        bool ctrl = (GetAsyncKeyState(VK_CONTROL) & 0x8000);
        if (alt && (p->vkCode == VK_TAB || p->vkCode == VK_F4)) return 1;
        if (ctrl && p->vkCode == VK_ESCAPE) return 1;
    }
    return CallNextHookEx(g_hk, nCode, wParam, lParam);
}

// 安装键盘钩子
void SystemControlHelper::InstallHook() {
    g_hk = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, nullptr, 0);
}

// 卸载键盘钩子
void SystemControlHelper::UninstallHook() {
    if (g_hk) { UnhookWindowsHookEx(g_hk); g_hk = nullptr; }
}
