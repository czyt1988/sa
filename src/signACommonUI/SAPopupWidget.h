#ifndef SAPOPUPWIDGET_H
#define SAPOPUPWIDGET_H

#include "SACommonUIGlobal.h"
class SAPopupWidgetPrivate;
class QHideEvent;
#include <QFrame>

///
/// \brief 一个类似模态对话框的popup窗口
///
class SA_COMMON_UI_EXPORT SAPopupWidget : public QFrame
{
    Q_OBJECT
    SA_IMPL(SAPopupWidget)
public:
    SAPopupWidget(QWidget* par);
    ~SAPopupWidget();
    int exec();
protected:
    void hideEvent(QHideEvent *e);
signals:
    void hided();
};

#endif // SAPOPUPWIDGET_H
