#ifndef SACOLORSETPROPERTYITEM_H
#define SACOLORSETPROPERTYITEM_H

#include <QWidget>
class QtColorPicker;
class SAColorSetPropertyItem : public QWidget
{
    Q_OBJECT
public:
    explicit SAColorSetPropertyItem(QWidget *parent = 0);
    ~SAColorSetPropertyItem();
    //
    void setText(const QString& text);
    //
    QString getText() const;
    //
    void setCurrentColor(const QColor& clr);
    //
    QColor getCurrentColor() const;
    //
    QtColorPicker* getColorPickerButton();
Q_SIGNALS:
    void colorChanged(const QColor &);
private:
    class UI;
    SAColorSetPropertyItem::UI *ui;
};

#endif // SACOLORSETPROPERTYITEM_H
