/*
 * @Author: Libra
 * @Date: 2025-05-03 03:06:43
 * @LastEditors: Libra
 * @Description: 
 */
#include "helpers/ContentProtectionHelper.h"
#include <QOperatingSystemVersion>

#ifdef Q_OS_WIN
#  include <windows.h>
#  ifndef WDA_MONITOR
#    define WDA_MONITOR 1
#  endif
#  ifndef WDA_EXCLUDEFROMCAPTURE
#    define WDA_EXCLUDEFROMCAPTURE 0x11
#  endif
#  ifndef WDA_NONE
#    define WDA_NONE 0x0
#  endif
extern "C" BOOL WINAPI SetWindowDisplayAffinity(HWND hWnd, DWORD dwAffinity);
#endif

void ContentProtectionHelper::setContentProtection(QWidget* w, bool enable) {
#ifdef Q_OS_WIN
    HWND hwnd = reinterpret_cast<HWND>(w->winId());
    DWORD mode = WDA_NONE;
    if (enable) {
        // 用 QOperatingSystemVersion 判断系统版本
        if (QOperatingSystemVersion::current() >= QOperatingSystemVersion(QOperatingSystemVersion::Windows, 10, 0, 19041)) {
            mode = WDA_EXCLUDEFROMCAPTURE;
        } else {
            mode = WDA_MONITOR;
        }
    }
    SetWindowDisplayAffinity(hwnd, mode);
#else
    Q_UNUSED(w)
    Q_UNUSED(enable)
#endif
}
