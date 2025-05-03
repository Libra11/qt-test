/*
 * @Author: Libra
 * @Date: 2025-05-03 14:09:18
 * @LastEditors: Libra
 * @Description: 
 */
#ifndef COLORMANAGER_H
#define COLORMANAGER_H

#include <QString>

class ColorManager {
public:
    // 主色
    static QString primary()      { return "#18181b"; }
    static QString destructive()  { return "#ef4444"; }
    static QString outline()      { return "#e4e4e7"; }
    static QString secondaryBg()  { return "#f4f4f5"; }
    static QString white()        { return "#ffffff"; }
    static QString black()        { return "#000000"; }

    // hover 相关
    static QString primaryHover()      { return "#232326"; }
    static QString destructiveHover()  { return "#dc2626"; }
    static QString outlineHoverBg()    { return "#e4e4e7"; }
    static QString outlineHoverBorder(){ return "#18181b"; }
    static QString secondaryHoverBg()  { return "#e4e4e7"; }
    static QString ghostHoverBg()      { return "#f4f4f5"; }
    static QString linkHover()         { return "#6366f1"; }
};

#endif // COLORMANAGER_H 