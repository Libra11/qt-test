// FormDropDown.h
#ifndef FORMDROPDOWN_H
#define FORMDROPDOWN_H

#include "Form.h"

class FormDropDownHelper {
public:
    static void addDropDown(Form* form, const QString& key, const QStringList& options, const QString& placeholder);
};

#endif // FORMDROPDOWN_H
