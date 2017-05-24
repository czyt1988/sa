#include "SAInformationStatusWidget.h"
#include "ui_SAInformationStatusWidget.h"
#include <QPixmap>
#include <QPalette>
#include <QTimer>
#include <QGraphicsOpacityEffect>
SAInformationStatusWidget::SAInformationStatusWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SAInformationStatusWidget)
  ,m_fadeInTimer(nullptr)
  ,m_fadeOutTimer(nullptr)
  ,m_fadeInTimeInterval(1000/18)
  ,m_fadeOutTimeInterval(1000/18)
  ,m_inOpacityInterval(0.05555555555555555555555555555556)
  ,m_outOpacityInterval(0.05555555555555555555555555555556)
  ,m_fadeOutCountLimit(100)
  ,m_fadeOutCount(0)
  ,m_startQueue(false)
  ,m_queueTimer(nullptr)
{
    ui->setupUi(this);
    m_timer = new QTimer(this);
    connect (m_timer,&QTimer::timeout,this,&SAInformationStatusWidget::onhideTimeOut);
}

SAInformationStatusWidget::~SAInformationStatusWidget()
{
    delete ui;
}

void SAInformationStatusWidget::showWarningInfo(const QString& info, int interval)
{
    setUIMode(SA::WarningMessage);
    ui->label_info->setText (colorText(info,QColor(0,0,0)));
    m_showing = true;
    fadeIn();
    m_timer->start (interval);
}

void SAInformationStatusWidget::showErrorInfo(const QString& info, int interval)
{
    setUIMode(SA::ErrorMessage);
    ui->label_info->setText (colorText(info,QColor(255,255,255)));
    m_showing = true;
    fadeIn();
    m_timer->start (interval);
}

void SAInformationStatusWidget::showNormalInfo(const QString& info, int interval)
{
    setUIMode(SA::NormalMessage);
    ui->label_info->setText (colorText(info,QColor(0,0,0)));
    m_showing = true;
    fadeIn();
    m_timer->start (interval);
}

void SAInformationStatusWidget::showQuesstionInfo(const QString& info, int interval)
{
    setUIMode(SA::QuesstionMessage);
    ui->label_info->setText (colorText(info,QColor(255,255,255)));
    m_showing = true;
    fadeIn();
    m_timer->start (interval);
}
///
/// \brief 渐显设置
/// \param enable
/// \param fadeTime 渐变持续时间，1000ms，默认为1s
/// \param fps 每秒帧数，越高效果越光滑，cpu消耗也越大
///
void SAInformationStatusWidget::setFadeIn(bool enable, int fadeTime, int fps)
{
    if(enable)
    {
        if(nullptr==m_fadeInTimer)
        {
            m_fadeInTimer = new QTimer(this);
            connect (m_fadeInTimer,&QTimer::timeout,this,&SAInformationStatusWidget::onTimeOutFadeIn);
        }
        m_fadeInTimeInterval = double(fadeTime)/double(fps);
        m_inOpacityInterval = 1.0 / ((double(fadeTime) / 1000.0) * double(fps));//计算每一步需要增加的透明度
    }
    else
    {
        if(m_fadeInTimer)
        {
            delete m_fadeInTimer;
            m_fadeInTimer = nullptr;
        }
    }
}
///
/// \brief 渐隐藏设置
/// \param enable
/// \param fadeTime 渐变持续时间，1000ms，默认为1s
/// \param fps 每秒帧数，越高效果越光滑，cpu消耗也越大
///
void SAInformationStatusWidget::setFadeOut(bool enable, int fadeTime, int fps)
{
    if(enable)
    {
        if(nullptr==m_fadeOutTimer)
        {
            m_fadeOutTimer = new QTimer(this);
            connect (m_fadeOutTimer,&QTimer::timeout,this,&SAInformationStatusWidget::onTimeOutFadeOut);
        }
        m_fadeOutCount=0;
        m_fadeOutCountLimit = fps * double(fadeTime) / 1000.0;
        m_fadeOutTimeInterval = double(fadeTime)/double(fps);
        m_outOpacityInterval = 1.0 / ((double(fadeTime) / 1000.0) * double(fps));//计算每一步需要增加的透明度
    }
    else
    {
        if(m_fadeOutTimer)
        {
            delete m_fadeOutTimer;
            m_fadeOutTimer = nullptr;
        }
    }
}

void SAInformationStatusWidget::fadeIn()
{
    if(nullptr == m_fadeInTimer)
    {
        this->show ();
        return;
    }
    _setOpacity(0.0);
    this->show ();
    m_fadeInTimer->start (m_fadeInTimeInterval);
}

void SAInformationStatusWidget::fadeOut()
{
    if(nullptr == m_fadeOutTimer)
    {
        this->hide ();
        return;
    }
    _setOpacity(1.0);
    m_fadeOutCount = 0;
    m_fadeOutTimer->start (m_fadeOutTimeInterval);
}
///
/// \brief 添加信息，此函数会把消息添加到队列中顺序执行，一直执行到队列清空
/// \param info 内容
/// \param type 显示的信息类别
/// \param interval 信息停留时间
/// \see SAInformationStatusWidget::InfoType
///
void SAInformationStatusWidget::postInfo(const QString& info, SA::MeaasgeType type, int interval)
{
    m_infoQueue.append (SAInformationStatusWidget::Info(info,type,interval));
    startShowQueue();
}

void SAInformationStatusWidget::onhideTimeOut()
{
    fadeOut();
}

void SAInformationStatusWidget::onTimeOutFadeIn()
{
    double op = _getOpacity();
    if(op >= 1)
    {
        if(m_fadeInTimer)
        {
            m_fadeInTimer->stop ();
            return;
        }
    }
    op += m_inOpacityInterval;
    _setOpacity(op);
}

void SAInformationStatusWidget::onTimeOutFadeOut()
{
    double op = _getOpacity();
    ++m_fadeOutCount;//防止不支持windowOpacity的在不停的触发
    if(op <= 0 || m_fadeOutCount >= m_fadeOutCountLimit)
    {
        if(m_fadeOutTimer)
        {
            m_fadeOutTimer->stop ();
            this->hide ();
            m_showing = false;
            return;
        }
    }
    op -= m_outOpacityInterval;
    _setOpacity(op);
}

void SAInformationStatusWidget::onQueueTimeout()
{
    m_queueTimer->stop ();
    m_startQueue = true;
    if(0 == m_infoQueue.size ())
    {//说明队列已经清空,把开始队列显示设置为false
        m_startQueue = false;
        return;
    }
    if(m_showing)//说明还没显示结束
    {
        m_queueTimer->start (30);//隔30ms过来再观察
    }
    //出队

    SAInformationStatusWidget::Info info =  m_infoQueue.dequeue ();
    switch (info.type) {
    case SA::NormalMessage:
        showNormalInfo (info.info,info.interval);
        break;
    case SA::WarningMessage:
        showWarningInfo (info.info,info.interval);
        break;
    case SA::ErrorMessage:
        showErrorInfo (info.info,info.interval);
        break;
    case SA::QuesstionMessage:
        showQuesstionInfo (info.info,info.interval);
        break;
    default:
        break;
    }
    m_queueTimer->start (info.interval);//下次触发时间
}

QString SAInformationStatusWidget::colorText(const QString& text,const QColor& clr)
{
    return QStringLiteral("<html><head/><body><p><span style=\" color:%1;\">%2</span></p></body></html>")
            .arg(clr.name ())
            .arg(text);
}

void SAInformationStatusWidget::_setOpacity(double op)
{
    QGraphicsOpacityEffect* effect = qobject_cast<QGraphicsOpacityEffect*>(graphicsEffect ());
    if(nullptr == effect)
    {
        effect = new QGraphicsOpacityEffect(this);
        setGraphicsEffect (effect);
    }
    effect->setOpacity (op);
}

double SAInformationStatusWidget::_getOpacity() const
{
    QGraphicsOpacityEffect* effect = qobject_cast<QGraphicsOpacityEffect*>(graphicsEffect ());
    if(nullptr == effect)
    {
        return 1;
    }
    return effect->opacity ();
}

void SAInformationStatusWidget::startShowQueue()
{
    if(!m_startQueue)
    {
        setupQueueTimer();
        m_queueTimer->start(0);//马上进入队列
        m_startQueue = true;
    }
}

void SAInformationStatusWidget::setUIMode(SA::MeaasgeType type)
{
    switch (type) {
    case SA::NormalMessage:
    {
        ui->label_icon->setPixmap (QPixmap(":/info/res_imageInfomation/normalInfo.png"));
        ui->label_icon->setStyleSheet ("");
        ui->label_info->setStyleSheet("");
    }break;
    case SA::WarningMessage:
    {
        QColor clr(239,176,60);
        ui->label_icon->setStyleSheet (QString("border-color: %1").arg (clr.name ()));
        ui->label_icon->setPixmap (QPixmap(":/info/res_imageInfomation/warningInfo.png"));
        ui->label_info->setStyleSheet(QString("background-color: %1").arg (clr.name ()));
    }break;
    case SA::ErrorMessage:
    {
        QColor clr(200,0,0);
        ui->label_icon->setStyleSheet (QString("border-color: %1").arg (clr.name ()));
        ui->label_icon->setPixmap (QPixmap(":/info/res_imageInfomation/errorInfo.png"));
        ui->label_info->setStyleSheet(QString("background-color: %1").arg (clr.name ()));
    }break;
    case SA::QuesstionMessage:
    {
        QColor clr(78,160,221);
        ui->label_icon->setStyleSheet (QString("border-color: %1").arg (clr.name ()));
        ui->label_icon->setPixmap (QPixmap(":/info/res_imageInfomation/warningInfo.png"));
        ui->label_info->setStyleSheet(QString("background-color: %1").arg (clr.name ()));

    }break;
    default:
        break;
    }
}

void SAInformationStatusWidget::setupQueueTimer()
{
    if(nullptr == m_queueTimer)
    {
        m_queueTimer = new QTimer(this);
        connect (m_queueTimer,&QTimer::timeout,this,&SAInformationStatusWidget::onQueueTimeout);
    }
}
