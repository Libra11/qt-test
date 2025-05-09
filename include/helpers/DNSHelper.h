#ifndef DNSHELPER_H
#define DNSHELPER_H

#include <QString>

class DNSHelper
{
public:
    // 启用断外网（设置DNS为无效地址）
    static bool EnableDnsBlock();

    // 禁用断外网（恢复自动获取DNS）
    static bool DisableDnsBlock();
};

#endif // DNSHELPER_H
