#ifndef FORMITEM_H
#define FORMITEM_H

#include <QString>
#include <QStringList>

struct FormItem {
    QString key;
    QString type; // "input" 或 "select"
    QString placeholder;
    QStringList options; // 仅select用
    bool required;
};

#endif // FORMITEM_H 