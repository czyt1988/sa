#include "SAMultWidget.h"
#include <QGridLayout>
SAMultWidget::SAMultWidget(QWidget *parent) :
    QWidget(parent)
  ,m_currentWidget(nullptr)
{
    m_mainLayout = new QGridLayout(this);
    m_mainLayout->setObjectName(QString("SAMultWidgetGridLayout"));
    m_mainLayout->setSpacing(6);
    m_mainLayout->setContentsMargins(6, 6, 6, 6);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);

}
///
/// \brief 添加多布局窗体
/// \param widget 要添加的窗体指针
/// \param fromRow 行
/// \param fromColumn 列
/// \param rowSpan 行的占用
/// \param columnSpan 列的占用
/// \param alignment 对齐方式
///
void SAMultWidget::addWidget(QWidget* widget, int fromRow, int fromColumn, int rowSpan, int columnSpan, Qt::Alignment alignment)
{
    m_mainLayout->addWidget (widget,fromRow,fromColumn,rowSpan,columnSpan,alignment);
    m_currentWidget = widget;
}
///
/// \brief 获取内部的grid布局
/// \return 如果没有grid布局返回nullptr
///
QGridLayout *SAMultWidget::getGridLayout() const
{
    return qobject_cast<QGridLayout *>(layout());
}

///
/// \brief 获取最后操作的窗体指针
/// \return
///
QWidget *SAMultWidget::currentWidget() const
{
    return m_currentWidget;
}
