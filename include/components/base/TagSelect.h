#ifndef TAGSELECT_H
#define TAGSELECT_H

#include <QWidget>
#include <QMap>
#include <QPushButton>

class FlowLayout;

class TagSelect : public QWidget
{
    Q_OBJECT

public:
    explicit TagSelect(QWidget *parent = nullptr);
    void setOptions(const QStringList &options);
    QStringList selectedOptions() const;
    void clearSelection();

signals:
    void selectionChanged(const QStringList &selected);

private slots:
    void onTagClicked();

private:
    FlowLayout *m_layout;
    QMap<QPushButton *, QString> m_buttonMap;
};

#endif // TAGSELECT_H
