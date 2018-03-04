#ifndef SAFONTSETWIDGET_H
#define SAFONTSETWIDGET_H

#include <QWidget>
#include "SACommonUIGlobal.h"
namespace Ui {
class SAFontSetWidget;
}

class SA_COMMON_UI_EXPORT SAFontSetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SAFontSetWidget(QWidget *parent = 0);
    ~SAFontSetWidget();
    void setCurrentFont(const QFont& font);
    QFont getFont() const;
private slots:
    Q_SLOT void onToolButtonBoldToggled(bool checked);
    Q_SLOT void onToolButtonItalicToggled(bool checked);
    Q_SLOT void onToolButtonFontSetPressed();
    Q_SLOT void onFontChanged(const QFont& newfont);
signals:
    void fontChanged(const QFont& font);
private:
    Ui::SAFontSetWidget *ui;
};

#endif // SAFONTSETWIDGET_H
