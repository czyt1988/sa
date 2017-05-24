#ifndef SAWIDGET_H
#define SAWIDGET_H
#include <czyQtUI.h>
#include "SAGlobals.h"
class SAWidget : public czy::QtUI::QWidgetEx
{
    Q_OBJECT
public:
    SAWidget(QWidget *parent = 0);
    ~SAWidget(){}
//    enum InfoType{
//        NormalInfo,
//        WarningInfo,
//        QuessionInfo,
//        ErrorInfo
//    };

signals:
    ///
    /// \brief 抛出的信息，sawidget可以通过此消息发送信息，让message给主窗口
    /// \param info 消息
    /// \param widget 抛出信息的窗口
    /// \param messageType 消息的类型，含有四种消息
    /// \param interval 消息需要显示的时间间隔 ms
    /// \see SAWidget::InfoType
    ///
    void postInfoMessage(const QString& info,QWidget *widget,SA::MeaasgeType messageType,int interval);
};

#endif // SAWIDGET_H
