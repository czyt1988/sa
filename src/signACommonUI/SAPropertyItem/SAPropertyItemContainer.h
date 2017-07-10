#ifndef SAPROPERTYITEMCONTAINER_H
#define SAPROPERTYITEMCONTAINER_H

#include <QWidget>
class SAPropertyItemContainer : public QWidget
{
public:
    explicit SAPropertyItemContainer(QWidget *parent = nullptr);
    ~SAPropertyItemContainer();
    //
    void setText(const QString& text);
    //
    QString getText() const;
    //
    QWidget* widget() const;
protected:
    void setWidget(QWidget* w);
    //

private:
    class UI;
    SAPropertyItemContainer::UI *ui;
    QWidget* m_w;
};

#endif // SAPROPERTYITEMCONTAINER_H
