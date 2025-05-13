#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QMap>
#include <QString>
#include <QLabel>
#include "components/base/Input.h"
#include "components/base/Button.h"
#include "components/base/DropDown.h"
#include "components/form/FormItem.h"

class Form : public QWidget {
    Q_OBJECT
public:
    enum class Layout { Vertical, Horizontal, Grid };
    enum class LabelPosition { Top, Left };

    explicit Form(QWidget *parent = nullptr);

    void addInput(const QString &key, const QString &placeholder = "");
    void addSelect(const QString &key, const QStringList &options, const QString &placeholder = "");
    void setRequired(const QString &key, bool required = true);
    QString value(const QString &key) const;
    void setSubmitText(const QString &text);
    void setupByConfig(const QList<FormItem>& items);
    QWidget* getCustomWidget(const QString& key) const;
    QLayout* layout;
    Button *submitBtn;
    void setLayout(Layout layout);
    void setLabelPosition(LabelPosition pos);
    void setLabelWidth(int width);
    void reset();
    void setValues(const QMap<QString, QString>& values);
    void setDisabled(const QString& key, bool disabled);
    void addValidationRule(const QString& key, const FormValidationRule& rule);

    // Loading state methods
    void setLoading(bool loading);
    bool isLoading() const;
    Layout m_layout = Layout::Vertical;
    LabelPosition m_labelPosition = LabelPosition::Top;
    int m_labelWidth = 100;
    QList<QString> m_fieldOrder;
    void updateLayout();
    bool validateField(const QString& key, const QString& value);
    void setupFieldConnections(const QString& key, QWidget* widget);
    void applyStyleToFormItem();

    QMap<QString, Input*> inputs;
    QMap<QString, DropDown*> dropDowns;
    QMap<QString, QLabel*> errorLabels;
    QMap<QString, QLabel*> fieldLabels;
    QMap<QString, bool> requiredFields;
    QMap<QString, QList<FormValidationRule>> validationRules;
    QMap<QString, QString> initialValues;
    bool m_isDirty = false;


signals:
    void submitted(const QMap<QString, QString> &values);
    void validationError(const QString& key, const QString& errorMessage);
    void beforeSubmit();
    void afterSubmit();
    void formChanged();

private slots:
    void handleSubmit();
    void handleValueChanged();
};

#endif // FORM_H
