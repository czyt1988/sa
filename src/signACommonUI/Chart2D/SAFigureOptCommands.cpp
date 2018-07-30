#include "SAFigureOptCommands.h"
#include "SAFigureWindow.h"
#include "SAChart2D.h"
#include "SAChart.h"
#include "SAXYSeries.h"
#include "qwt_plot_item.h"
#include "qwt_plot_barchart.h"
#include "SAAbstractRegionSelectEditor.h"
#include "qwt_series_data.h"
#include "czyAlgorithm.h"


///
/// \brief SAFigureCreateSubWidget::SAFigureCreateSubWidget
/// \param fig
/// \param w
/// \param xPresent
/// \param yPresent
/// \param wPresent
/// \param hPresent
/// \param cmdName
/// \param parent
///
SAFigureCreateSubWidget::SAFigureCreateSubWidget(SAFigureWindow *fig
                                                 , QWidget *w
                                                 , float xPresent
                                                 , float yPresent
                                                 , float wPresent
                                                 , float hPresent
                                                 , const QString &cmdName
                                                 , QUndoCommand *parent)
    :SAFigureOptCommand(fig,cmdName,parent)
    ,m_widget(w)
    ,m_isNeedToDelete(false)
{
    fig->_addWidget(w,xPresent,yPresent,wPresent,hPresent);
}

///
/// \brief 析构此命令时需要对窗口进行删除处理否则会一直存在
///
SAFigureCreateSubWidget::~SAFigureCreateSubWidget()
{
    if(m_widget && m_isNeedToDelete)
    {
        //只有在m_isHidedByCmd是true时才delete，证明最后状态是处于undo状态
        delete m_widget;
    }
}

void SAFigureCreateSubWidget::redo()
{
    m_widget->show();
    m_isNeedToDelete = false;
}

void SAFigureCreateSubWidget::undo()
{
    m_widget->hide();
    m_isNeedToDelete = true;
}


SAFigureSubChartResize::SAFigureSubChartResize(
        SAFigureWindow *fig
        , QWidget *w
        , const QRect &size
        ,const QString &cmdName
        , QUndoCommand *parent)
    :SAFigureOptCommand(fig,cmdName,parent)
  ,m_widget(w)
  ,m_newSize(size)
{
    m_oldSize = w->frameGeometry();
}

SAFigureSubChartResize::SAFigureSubChartResize(
        SAFigureWindow *fig
        , QWidget *w
        , const QRect &oldSize
        , const QRect &newSize
        ,const QString &cmdName
        , QUndoCommand *parent)
 :SAFigureOptCommand(fig,cmdName,parent)
 ,m_widget(w)
 ,m_newSize(newSize)
 ,m_oldSize(oldSize)
{

}

SAFigureSubChartResize::~SAFigureSubChartResize()
{

}

void SAFigureSubChartResize::redo()
{
    m_widget->setGeometry(m_newSize);
}

void SAFigureSubChartResize::undo()
{
    m_widget->setGeometry(m_oldSize);
}


SAFigureChartItemAddCommand::SAFigureChartItemAddCommand(SAChart2D *chart, QwtPlotItem *ser, const QString &cmdName,QUndoCommand *parent)
    :SAFigureOptCommand(chart,cmdName,parent)
    ,m_item(ser)
{

}

SAFigureChartItemAddCommand::~SAFigureChartItemAddCommand()
{
    if(nullptr == m_item->plot())
    {
        delete m_item;
    }
}

void SAFigureChartItemAddCommand::redo()
{
    m_item->attach(chart2D());
}

void SAFigureChartItemAddCommand::undo()
{
    m_item->detach();
}

SAFigureChartItemDeleteCommand::SAFigureChartItemDeleteCommand(SAChart2D *chart, QwtPlotItem *item, const QString &cmdName,QUndoCommand *parent)
    :SAFigureOptCommand(chart,cmdName,parent)
    ,m_item(item)
{
}

SAFigureChartItemDeleteCommand::~SAFigureChartItemDeleteCommand()
{
    if(nullptr == m_item->plot())
    {
        delete m_item;
    }
}

void SAFigureChartItemDeleteCommand::redo()
{
    m_item->detach();
}

void SAFigureChartItemDeleteCommand::undo()
{
    m_item->attach(chart2D());
}

SAFigureChartItemListAddCommand::SAFigureChartItemListAddCommand(SAChart2D *chart, const QList<QwtPlotItem *> &itemList, const QString &cmdName,QUndoCommand *parent)
    :SAFigureOptCommand(chart,cmdName,parent)
    ,m_itemList(itemList)
{

}

SAFigureChartItemListAddCommand::~SAFigureChartItemListAddCommand()
{
    const int size = m_itemList.size();
    for(int i=0;i<size;++i)
    {
        if(nullptr == m_itemList[i]->plot())
        {
            delete m_itemList[i];
            m_itemList[i];
        }
    }
}


void SAFigureChartItemListAddCommand::redo()
{
    const int size = m_itemList.size();
    for(int i=0;i<size;++i)
    {
        m_itemList[i]->attach(chart2D());
    }
}

void SAFigureChartItemListAddCommand::undo()
{
    const int size = m_itemList.size();
    for(int i=0;i<size;++i)
    {
        m_itemList[i]->detach();
    }
}

SAFigureChartSelectionRegionAddCommand::SAFigureChartSelectionRegionAddCommand(SAChart2D *chart, const QPainterPath &newRegion, const QString &cmdName, QUndoCommand *parent)
    :SAFigureOptCommand(chart,cmdName,parent)
    ,m_newPainterPath(newRegion)
{
    m_oldPainterPath = chart2D()->getSelectionRange();
}

SAFigureChartSelectionRegionAddCommand::SAFigureChartSelectionRegionAddCommand(SAChart2D *chart, const QPainterPath &oldRegion, const QPainterPath &newRegion, const QString &cmdName, QUndoCommand *parent)
    :SAFigureOptCommand(chart,cmdName,parent)
    ,m_newPainterPath(newRegion)
    ,m_oldPainterPath(oldRegion)
{

}

void SAFigureChartSelectionRegionAddCommand::redo()
{
    chart2D()->setSelectionRange(m_newPainterPath);
}

void SAFigureChartSelectionRegionAddCommand::undo()
{
    chart2D()->setSelectionRange(m_oldPainterPath);
}





