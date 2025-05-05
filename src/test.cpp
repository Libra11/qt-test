// main.cpp

#include <windows.h>
#include <shellapi.h>

#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <iostream>

bool IsRunningAsAdmin();
bool TryRunAsAdmin();
bool AddHostsEntry();
bool RemoveHostsEntry();

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // 1. 检测是否已具备管理员权限
    if (!IsRunningAsAdmin())
    {
        // 尝试以管理员身份重启自己
        if (TryRunAsAdmin())
            return 0; // 新进程会执行接下来的逻辑，当前进程直接退出
        else
        {
            std::cerr << "Error: Administrator privileges required, and elevation was cancelled.\n";
            return 1;
        }
    }

    // 2. 已经是管理员，执行 hosts 修改
    QStringList args = app.arguments();
    bool removeMode = args.contains(QStringLiteral("-r")) ||
                      args.contains(QStringLiteral("--remove"));

    bool ok = removeMode ? RemoveHostsEntry() : AddHostsEntry();
    if (!ok)
    {
        std::cerr << "Error: Failed to "
                  << (removeMode ? "remove" : "add")
                  << " hosts entry.\n";
        return 1;
    }

    std::cout << "Hosts file updated successfully.\n";
    return 0;
}

bool IsRunningAsAdmin()
{
    BOOL isAdmin = FALSE;
    PSID adminGroup = nullptr;
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
    if (AllocateAndInitializeSid(&ntAuthority, 2,
                                 SECURITY_BUILTIN_DOMAIN_RID,
                                 DOMAIN_ALIAS_RID_ADMINS,
                                 0,0,0,0,0,0,
                                 &adminGroup))
    {
        CheckTokenMembership(nullptr, adminGroup, &isAdmin);
        FreeSid(adminGroup);
    }
    return isAdmin == TRUE;
}

bool TryRunAsAdmin()
{
    // 1) exe 路径
    wchar_t szExePath[MAX_PATH];
    if (!GetModuleFileNameW(nullptr, szExePath, MAX_PATH))
        return false;

    // 2) 拿到原命令行参数列表，去掉第一个元素（exe 路径本身）
    QStringList args = QCoreApplication::arguments();
    args.removeFirst();
    // 拼成一个字符串，带上双引号以防参数里有空格
    QStringList quoted;
    for (auto &a : args)
        quoted << QStringLiteral("\"%1\"").arg(a);
    QString paramString = quoted.join(' ');

    // 3) 填充 SHELLEXECUTEINFO
    SHELLEXECUTEINFOW sei;
    ZeroMemory(&sei, sizeof(sei));
    sei.cbSize       = sizeof(sei);
    sei.fMask        = SEE_MASK_NOCLOSEPROCESS;
    sei.lpVerb       = L"runas";              // 提权
    sei.lpFile       = szExePath;             // 可执行文件
    // 关键：把参数传进去
    std::wstring wParams = paramString.toStdWString();
    sei.lpParameters = wParams.c_str();
    sei.nShow        = SW_NORMAL;

    // 4) 发起 UAC 提权
    return ShellExecuteExW(&sei) != FALSE;
}

bool AddHostsEntry()
{
    const QString path = QStringLiteral("C:/Windows/System32/drivers/etc/hosts");
    QFile file(path);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        return false;

    QTextStream ts(&file);
    QString content = ts.readAll();
    if (!content.contains(QStringLiteral("www.baidu.com")))
    {
        file.seek(file.size());
        ts << "\n127.0.0.123 www.baidu.com\n";
    }

    file.close();
    return true;
}



bool RemoveHostsEntry()
{
    const QString path = QStringLiteral("C:/Windows/System32/drivers/etc/hosts");

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QTextStream in(&file);
    QStringList lines = in.readAll().split('\n');
    file.close();

    QStringList filtered;
    for (const QString &line : lines) {
        if (!line.contains(QStringLiteral("www.baidu.com")) && !line.isEmpty())
            filtered << line;
    }

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        return false;
    QTextStream out(&file);

    // join 不会在末尾再加 \n
    out << filtered.join('\n');

    file.close();
    return true;
}

