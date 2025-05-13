#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>
#include <QLabel>
#include <QResizeEvent>
#include "PageBase.h"
#include "components/form/Form.h"

class LoginPage : public PageBase
{
    Q_OBJECT
public:
    explicit LoginPage(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QString m_imageKey;
    Form* form;
    QLabel* m_imgLabel;         // 保存图片标签指针以便在resize事件中使用
    QString m_loginImagePath;   // 保存图片路径
};

#endif // LOGINPAGE_H
