#ifndef SAINFORMATIONSTATUSWIDGET_H
#define SAINFORMATIONSTATUSWIDGET_H

#include <QWidget>
#include <QColor>
#include <QQueue>
#include "../global/SAGlobals.h"
class QTimer;
namespace Ui {
class SAInformationStatusWidget;
}
///
/// \brief 用于状态栏，根据显示的信息，实现颜色的变化
///
class SAInformationStatusWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SAInformationStatusWidget(QWidget *parent = 0);
    ~SAInformationStatusWidget();
    //下面四个接口时马上进行显示的接口内容并不存在队列中
    void showWarningInfo(const QString& info, int interval = 2000);
    void showErrorInfo(const QString& info, int interval = 2000);
    void showNormalInfo(const QString& info, int interval = 2000);
    void showQuesstionInfo(const QString& info, int interval = 2000);

    void setFadeIn(bool enable, int fadeTime = 1000, int fps = 18);         //设置渐显
    void setFadeOut(bool enable, int fadeTime = 1000, int fps = 18);        //设置渐出
    void fadeIn();                                                          //渐出，注意次函数会调用show()
    void fadeOut();

    void postInfo(const QString& info, SA::MeaasgeType type = SA::NormalMessage, int interval = 800);

private slots:
    void onhideTimeOut();
    void onTimeOutFadeIn();
    void onTimeOutFadeOut();
    void onQueueTimeout();

private:
    QString colorText(const QString& text, const QColor& clr);
    void _setOpacity(double op);
    double _getOpacity() const;
    void startShowQueue();
    void setUIMode(SA::MeaasgeType type);
    void setupQueueTimer();

private:
    class Info
    {
public:
        Info(const QString& s, SA::MeaasgeType t = SA::NormalMessage, int i = 2000)
            : info(s)
            , interval(i)
            , type(t)
        {}
        QString info;
        int interval;
        SA::MeaasgeType type;
    };
    QQueue<Info> m_infoQueue;
    Ui::SAInformationStatusWidget *ui;
    QTimer *m_timer;
    QTimer *m_fadeInTimer;          //渐显渐隐效果的timer
    QTimer *m_fadeOutTimer;         //渐显渐隐效果的timer
    QTimer *m_queueTimer;           ///< 队列的timer
    int m_fadeInTimeInterval;       //渐变间隔
    int m_fadeOutTimeInterval;
    double m_inOpacityInterval;     //每一步的透明度添加值
    double m_outOpacityInterval;
    int m_fadeOutCountLimit;
    int m_fadeOutCount;
    bool m_startQueue;      ///< 标记队列是否开始执行
    bool m_showing;         ///< 标记是否正在显示
};

#endif // SAINFORMATIONSTATUSWIDGET_H
