/*
 * @Author: Libra
 * @Date: 2025-05-06 14:29:59
 * @LastEditors: Libra
 * @Description: 使用SID比较方法判断Guest用户
 */
#include "helpers/UserHelper.h"
#include <lmcons.h>
#include <QDebug>
#include <sddl.h>
#include <vector>

bool UserHelper::IsGuestUser()
{
    HANDLE hToken = nullptr;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
        return false;

    // 1) 获取当前用户的SID
    DWORD cb = 0;
    GetTokenInformation(hToken, TokenUser, nullptr, 0, &cb);
    std::vector<BYTE> buffer(cb);
    if (!GetTokenInformation(hToken, TokenUser, buffer.data(), cb, &cb)) {
        CloseHandle(hToken);
        return false;
    }
    PSID pCurrentSid = reinterpret_cast<PTOKEN_USER>(buffer.data())->User.Sid;

    // 2) 生成Guest账户的SID
    BYTE guestSidBuf[SECURITY_MAX_SID_SIZE];
    PSID pGuestSid = reinterpret_cast<PSID>(guestSidBuf);
    DWORD sidSize = sizeof(guestSidBuf);
    if (!CreateWellKnownSid(WinAccountGuestSid, nullptr, pGuestSid, &sidSize)) {
        CloseHandle(hToken);
        return false;
    }

    // 3) 比较SID
    bool isGuest = (EqualSid(pCurrentSid, pGuestSid) != FALSE);
    CloseHandle(hToken);
    return isGuest;
}

QString UserHelper::GetCurrentUsername()
{
    wchar_t username[UNLEN + 1];
    DWORD size = UNLEN + 1;
    
    if (GetUserNameW(username, &size)) {
        return QString::fromWCharArray(username);
    }
    
    return QString();
} 