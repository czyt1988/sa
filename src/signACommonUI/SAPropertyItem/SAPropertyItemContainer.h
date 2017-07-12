#ifndef SAPROPERTYITEMCONTAINER_H
#define SAPROPERTYITEMCONTAINER_H

#include <QWidget>
#include <QIcon>
class QPaintEvent;
class SAPropertyItemContainer : public QWidget
{
public:
    explicit SAPropertyItemContainer(QWidget *parent = nullptr);
    ~SAPropertyItemContainer();
    //设置文字描述
    void setText(const QString& text);
    //获取文字描述
    QString getText() const;
    //设置图标
    QIcon getIcon() const;
    void setIcon(const QIcon &icon);

protected:
    void addWidget(QWidget* w, int stretch = 0, Qt::Alignment alignment = Qt::Alignment());
    //
    void addLayout(QLayout *layout, int stretch = 0);
    //
    void paintEvent(QPaintEvent *event);
private:
    class UI;
    SAPropertyItemContainer::UI *ui;
    QIcon* m_icon;
    QRect* m_iconRect;
};

#endif // SAPROPERTYITEMCONTAINER_H
