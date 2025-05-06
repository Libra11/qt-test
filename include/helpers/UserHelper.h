#ifndef USERHELPER_H
#define USERHELPER_H

#pragma once
#include <windows.h>
#include <QString>

class UserHelper {
public:
    // 判断当前用户是否为Guest用户
    static bool IsGuestUser();
    
    // 获取当前用户名
    static QString GetCurrentUsername();
};

#endif // USERHELPER_H 