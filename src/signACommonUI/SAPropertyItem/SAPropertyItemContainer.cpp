#include "SAPropertyItemContainer.h"
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QPaintEvent>
#include <QPainter>
#include <QMargins>
class SAPropertyItemContainer::UI
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *labelName;
    void setupUi(QWidget *w)
    {
        if (w->objectName().isEmpty())
            w->setObjectName(QStringLiteral("SAColorSetPropertyItem"));
        w->resize(221, 23);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(w->sizePolicy().hasHeightForWidth());
        w->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(w);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        labelName = new QLabel(w);
        labelName->setObjectName(QStringLiteral("labelName"));

        horizontalLayout->addWidget(labelName);


    } // setupUi


};
SAPropertyItemContainer::SAPropertyItemContainer(QWidget *parent):QWidget(parent)
  ,ui(new SAPropertyItemContainer::UI)
  ,m_w(nullptr)
  ,m_icon(nullptr)
  ,m_iconRect(nullptr)
{
    ui->setupUi(this);
}

SAPropertyItemContainer::~SAPropertyItemContainer()
{
    delete ui;
}
///
/// \brief 设置文字描述
/// \param text
///
void SAPropertyItemContainer::setText(const QString &text)
{
    ui->labelName->setText(text);
}
///
/// \brief 获取文字描述
/// \return
///
QString SAPropertyItemContainer::getText() const
{
    return ui->labelName->text();
}
///
/// \brief 内部对应的控件窗口
/// \return
///
QWidget *SAPropertyItemContainer::widget() const
{
    return m_w;
}


///
/// \brief 设置内部对应的控件窗口
/// \param w
///
void SAPropertyItemContainer::setWidget(QWidget *w)
{
    ui->horizontalLayout->addWidget(w);
    m_w = w;
}

void SAPropertyItemContainer::paintEvent(QPaintEvent *event)
{
    if(m_iconRect && m_icon)
    {
        QPainter painter(this);
        m_icon->paint(&painter,*m_iconRect);
    }
    QWidget::paintEvent(event);
}

QIcon SAPropertyItemContainer::getIcon() const
{
    if(m_icon)
        return *m_icon;
    return QIcon();
}

void SAPropertyItemContainer::setIcon(const QIcon &icon)
{
    if(m_icon)
    {
        delete m_icon;
        m_icon = nullptr;
    }
    m_icon = new QIcon(icon);
    if(nullptr == m_iconRect)
    {
        m_iconRect = new QRect(2,2,20,20);
    }
    QMargins mag = ui->horizontalLayout->contentsMargins();
    mag.setLeft(m_iconRect->width()+4);
    ui->horizontalLayout->setContentsMargins(mag);
}
