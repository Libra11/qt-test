#include "helpers/HostsHelper.h"
#include <QFile>
#include <QTextStream>

bool HostsHelper::AddHostsEntry()
{
    QFile file("C:/Windows/System32/drivers/etc/hosts");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        return false;

    QTextStream in(&file);
    QString content = in.readAll();

    if (!content.contains("sec.sginput.qq.com"))
    {
        in.seek(file.size());
        in << "\n127.0.0.123 sec.sginput.qq.com\n";
    }

    file.close();
    return true;
}

bool HostsHelper::RemoveHostsEntry()
{
    QFile file("C:/Windows/System32/drivers/etc/hosts");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        return false;

    QTextStream in(&file);
    QString content = in.readAll();

    QStringList lines = content.split("\n");
    QString newContent;
    for (const QString &line : lines)
    {
        if (!line.contains("sec.sginput.qq.com"))
            newContent += line + "\n";
    }

    file.resize(0);
    in.seek(0);
    in << newContent;

    file.close();
    return true;
}
