/*
 * @Author: Libra
 * @Date: 2025-05-06 01:10:16
 * @LastEditors: Libra
 * @Description: 
 */
#include <windows.h>
#include <shellapi.h>

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <iostream>

bool IsRunningAsAdmin();
bool TryRunAsAdmin(const QStringList &args);
bool AddHostsEntry(const QString &ip, const QString &host);
bool RemoveHostsEntry(const QString &host);

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("HostsEditor");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Add or remove entries in hosts file");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption removeOption(QStringList() << "r" << "remove",
                                     "Remove hosts entry");
    parser.addOption(removeOption);

    QCommandLineOption ipOption(QStringList() << "i" << "ip",
                                 "IP address for entry", "ip", "127.0.0.123");
    parser.addOption(ipOption);

    parser.addPositionalArgument("host", "Hostname to add/remove", "[host]");

    parser.process(app);

    const QStringList positionalArgs = parser.positionalArguments();
    if (positionalArgs.isEmpty()) {
        std::cerr << "Error: Hostname not specified." << std::endl;
        parser.showHelp(1);
    }
    QString host = positionalArgs.first();
    QString ip = parser.value(ipOption);

    bool removeMode = parser.isSet(removeOption);

    if (!IsRunningAsAdmin()) {
        QStringList args = QCoreApplication::arguments();
        if (TryRunAsAdmin(args))
            return 0;
        else {
            std::cerr << "Error: Administrator privileges required, and elevation was cancelled." << std::endl;
            return 1;
        }
    }

    bool ok = removeMode ? RemoveHostsEntry(host)
                         : AddHostsEntry(ip, host);
    if (!ok) {
        std::cerr << "Error: Failed to "
                  << (removeMode ? "remove" : "add")
                  << " hosts entry for " << host.toStdString() << "." << std::endl;
        return 1;
    }

    std::cout << "Hosts file updated successfully for "
              << host.toStdString() << "." << std::endl;
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

bool TryRunAsAdmin(const QStringList &args)
{
    wchar_t szExePath[MAX_PATH];
    if (!GetModuleFileNameW(nullptr, szExePath, MAX_PATH))
        return false;

    QStringList argList = args;
    argList.removeFirst(); // remove executable path
    QStringList quoted;
    for (const QString &a : argList)
        quoted << QStringLiteral("\"%1\"").arg(a);
    QString paramString = quoted.join(' ');

    SHELLEXECUTEINFOW sei;
    ZeroMemory(&sei, sizeof(sei));
    sei.cbSize       = sizeof(sei);
    sei.fMask        = SEE_MASK_NOCLOSEPROCESS;
    sei.lpVerb       = L"runas";
    sei.lpFile       = szExePath;

    std::wstring wParams = paramString.toStdWString();
    sei.lpParameters = wParams.c_str();
    sei.nShow        = SW_NORMAL;

    return ShellExecuteExW(&sei) != FALSE;
}

bool AddHostsEntry(const QString &ip, const QString &host)
{
    const QString path = QStringLiteral("C:/Windows/System32/drivers/etc/hosts");
    QFile file(path);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        return false;

    QTextStream ts(&file);
    QString content = ts.readAll();
    QString entry = ip + " " + host;
    if (!content.contains(entry)) {
        file.seek(file.size());
        ts << "\n" << entry << "\n";
    }

    file.close();
    return true;
}

bool RemoveHostsEntry(const QString &host)
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
        if (line.contains(host) || line.trimmed().isEmpty())
            continue;
        filtered << line;
    }

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        return false;
    QTextStream out(&file);
    out << filtered.join('\n');
    if (!filtered.isEmpty())
        out << '\n';

    file.close();
    return true;
}
