// FormSelect.h
#ifndef FORMSELECT_H
#define FORMSELECT_H

#include "Form.h"

class FormSelectHelper {
public:
    static void addSelect(Form* form, const QString& key, const QStringList& options, const QString& placeholder);
};

#endif // FORMSELECT_H
