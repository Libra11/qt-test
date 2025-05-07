#ifndef FORMSIGNALS_H
#define FORMSIGNALS_H

#include "Form.h"

class FormSignalHelper {
public:
    static void handleValueChanged(Form* form);
    static void handleSubmit(Form* form);
};

#endif // FORMSIGNALS_H
