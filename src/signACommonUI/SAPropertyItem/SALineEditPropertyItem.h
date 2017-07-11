#ifndef SALINEEDITPROPERTYITEM_H
#define SALINEEDITPROPERTYITEM_H

#include "SAPropertyItemContainer.h"
class QLineEdit;
class SALineEditPropertyItem : public SAPropertyItemContainer
{
    Q_OBJECT
public:
    explicit SALineEditPropertyItem(QWidget *parent = nullptr);
    ~SALineEditPropertyItem();
    //
    void setEditText(const QString& text);
    //
    QString getEditText() const;

Q_SIGNALS:
    void textChanged(const QString &text);
private:
    class UI;
    SALineEditPropertyItem::UI *ui;
};

#endif // SALINEEDITPROPERTYITEM_H
