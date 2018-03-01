#include "SAPopupWidget.h"
#include <QEventLoop>
#include <QHideEvent>
class SAPopupWidgetPrivate
{
    SA_IMPL_PUBLIC(SAPopupWidget)
public:
    SAPopupWidgetPrivate(SAPopupWidget* p)
    :q_ptr(p)
    ,eventLoop(nullptr)
    {

    }

    QEventLoop* eventLoop;
};

SAPopupWidget::SAPopupWidget(QWidget *par):QFrame(par, Qt::Popup)
  ,d_ptr(new SAPopupWidgetPrivate(this))
{
    setFocusPolicy(Qt::StrongFocus);
}

SAPopupWidget::~SAPopupWidget()
{
    if (d_ptr->eventLoop)
        d_ptr->eventLoop->exit();
}

int SAPopupWidget::exec()
{
    show();

    QEventLoop e;
    d_ptr->eventLoop = &e;
    int r = e.exec();
    d_ptr->eventLoop = nullptr;
    return r;
}

void SAPopupWidget::hideEvent(QHideEvent *e)
{
    if (d_ptr->eventLoop) {
        d_ptr->eventLoop->exit();
    }
    setFocus();
    emit hided();
    QFrame::hideEvent(e);
}

