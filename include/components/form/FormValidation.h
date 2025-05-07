#ifndef FORMVALIDATION_H
#define FORMVALIDATION_H

#include "Form.h"

class FormValidationHelper {
public:
    static bool validateField(Form* form, const QString& key, const QString& value);
};

#endif // FORMVALIDATION_H
