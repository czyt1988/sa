#ifndef SACOLORSETPROPERTYITEM_H
#define SACOLORSETPROPERTYITEM_H
#include "SAPropertyItemContainer.h"
#include <QWidget>
class QtColorPicker;
class SAColorSetPropertyItem : public SAPropertyItemContainer
{
    Q_OBJECT
public:
    explicit SAColorSetPropertyItem(QWidget *parent = nullptr);
    ~SAColorSetPropertyItem();

    //
    void setCurrentColor(const QColor& clr);
    //
    QColor getCurrentColor() const;
    //
    QtColorPicker* getColorPickerButton();
Q_SIGNALS:
    void colorChanged(const QColor &);
private slots:
    void onColorChanged(const QColor &c);
    void onCurrentPenColorAlphaChanged(int v);
private:
    class UI;
    SAColorSetPropertyItem::UI *ui;
};

#endif // SACOLORSETPROPERTYITEM_H
