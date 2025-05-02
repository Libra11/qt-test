#ifndef HOSTHELPER_H
#define HOSTHELPER_H

#pragma once
#include <QString>

class HostsHelper {
public:
    // 添加hosts记录
    static bool AddHostsEntry();

    // 删除hosts记录
    static bool RemoveHostsEntry();
};

#endif // HOSTHELPER_H
