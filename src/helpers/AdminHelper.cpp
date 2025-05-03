#include "helpers/AdminHelper.h"

bool AdminHelper::IsRunningAsAdmin()
{
    BOOL isAdmin = FALSE;
    PSID adminGroup = NULL;
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    if (AllocateAndInitializeSid(&NtAuthority, 2,
        SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
        0, 0, 0, 0, 0, 0, &adminGroup))
    {
        CheckTokenMembership(NULL, adminGroup, &isAdmin);
        FreeSid(adminGroup);
    }
    return isAdmin;
}

bool AdminHelper::TryRunAsAdmin()
{
    TCHAR szPath[MAX_PATH];
    if (!GetModuleFileName(NULL, szPath, MAX_PATH))
        return false;

    SHELLEXECUTEINFO sei;
    ZeroMemory(&sei, sizeof(sei));
    sei.cbSize = sizeof(sei);
    sei.lpVerb = L"runas";
    sei.lpFile = szPath;
    sei.nShow = SW_NORMAL;
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;

    if (!ShellExecuteEx(&sei))
    {
        DWORD dwError = GetLastError();
        if (dwError == ERROR_CANCELLED)
        {
            return false; // 用户拒绝
        }
    }
    return true;
}

void AdminHelper::RequestAdminLoop()
{
    while (!IsRunningAsAdmin())
    {
        if (TryRunAsAdmin())
        {
            exit(0); // 启动了管理员版本，当前版本退出
        }
        else
        {
            int ret = QMessageBox::question(nullptr, "需要管理员权限",
                                            "获取管理员权限失败，必须以管理员身份运行。是否再次尝试？",
                                            QMessageBox::Yes | QMessageBox::No);
            if (ret == QMessageBox::No)
            {
                QMessageBox::critical(nullptr, "无法继续",
                                      "程序需要管理员权限才能继续，即将退出。");
                exit(0);
            }
        }
    }
}
