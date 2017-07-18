#ifndef SAFONTCOMBOBOXPROPERTYITEM_H
#define SAFONTCOMBOBOXPROPERTYITEM_H

#include "SAPropertyItemContainer.h"
class QFontComboBox;
class SAFontComboBoxPropertyItem : public SAPropertyItemContainer
{
    Q_OBJECT
public:
    explicit SAFontComboBoxPropertyItem(QWidget* par = nullptr);
    ~SAFontComboBoxPropertyItem();
    QFontComboBox* getFontComboBox() const;
public slots:
    void setCurrentFont(const QFont &f);
signals:
    void currentFontChanged(const QFont &font);
private:
    class UI;
    SAFontComboBoxPropertyItem::UI* ui;
};

#endif // SAFONTCOMBOBOXPROPERTYITEM_H
