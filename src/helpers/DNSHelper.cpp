#include "helpers/DNSHelper.h"
#include <QProcess>
#include <QDebug>

bool DNSHelper::EnableDnsBlock()
{
    // 获取活跃网络接口（这里只改全部接口）
    // 设置DNS为本地无效地址（如 127.0.0.1 或 0.0.0.0）
    QString cmd = "netsh interface ip set dns name=\"以太网\" static 127.0.0.1";
    QProcess process;

    process.start("netsh", QStringList() << "interface" << "show" << "interface");
    process.waitForFinished();
    QString output = process.readAllStandardOutput();

    QStringList lines = output.split("\n");
    bool success = true;

    for (QString line : lines) {
        if (line.contains("已连接") || line.contains("Connected")) {
            QString interfaceName = line.mid(line.lastIndexOf(" ") + 1).trimmed();
            qDebug() << "设置DNS:" << interfaceName;

            QString cmd = QString("interface ip set dns name=\"%1\" static 127.0.0.1").arg(interfaceName);
            QProcess::execute("netsh " + cmd);

            QString altCmd = QString("interface ip add dns name=\"%1\" 127.0.0.2 index=2").arg(interfaceName);
            QProcess::execute("netsh " + altCmd);
        }
    }

    return success;
}

bool DNSHelper::DisableDnsBlock()
{
    // 恢复为自动获取DNS
    QProcess process;

    process.start("netsh", QStringList() << "interface" << "show" << "interface");
    process.waitForFinished();
    QString output = process.readAllStandardOutput();

    QStringList lines = output.split("\n");
    bool success = true;

    for (QString line : lines) {
        if (line.contains("已连接") || line.contains("Connected")) {
            QString interfaceName = line.mid(line.lastIndexOf(" ") + 1).trimmed();
            qDebug() << "恢复DNS:" << interfaceName;

            QString cmd = QString("interface ip set dns name=\"%1\" source=dhcp").arg(interfaceName);
            QProcess::execute("netsh " + cmd);
        }
    }

    return success;
}
