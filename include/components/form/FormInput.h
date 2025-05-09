// FormInput.h
#ifndef FORMINPUT_H
#define FORMINPUT_H

#include "Form.h"

class FormInputHelper {
public:
    static void addInput(Form* form, const QString& key, const QString& placeholder, FormItemType type = FormItemType::Input);
};

#endif // FORMINPUT_H
