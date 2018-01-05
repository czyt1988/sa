#include "SAQwtPlotItemSetWidget.h"
#include "SALineEditPropertyItem.h"
#include "SADoubleSpinBoxPropertyItem.h"
#include "SACheckBoxPropertyItem.h"
#include "SAAxisSelectComboBoxPropertyItem.h"
#include "SAChartAxisSetSelect.h"
#include "qwt_plot_item.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_barchart.h"
#include <QVBoxLayout>
#include "SAQwtPlotCurveItemSetWidget.h"
#include "SAQwtPlotBarChartItemSetWidget.h"
SAQwtPlotItemSetWidget::SAQwtPlotItemSetWidget(QwtPlotItem *plotItem, QWidget* parent)
    :QWidget(parent)
    ,m_itemPtr(plotItem)
{
    m_layout = new QVBoxLayout(this);
    //visible
    m_visibleItem = new SACheckBoxPropertyItem;
    m_visibleItem->setObjectName("visible");
    connect(m_visibleItem,&SACheckBoxPropertyItem::stateChanged
                 ,this,[plotItem](int state){
        plotItem->setVisible(state==Qt::Checked);
    });
    m_layout->addWidget(m_visibleItem);
    //title
    m_titleItem = new SALineEditPropertyItem;
    m_titleItem->setObjectName("title");
    connect(m_titleItem,&SALineEditPropertyItem::textChanged
                 ,this,[this,plotItem](const QString& text){
        plotItem->setTitle(text);
    });
    connect(m_titleItem,&SALineEditPropertyItem::textChanged
                 ,this,&SAQwtPlotItemSetWidget::plotItemTitleChanged);
    m_layout->addWidget(m_titleItem);
    retranslateUi();
    upDateData();
}

void SAQwtPlotItemSetWidget::retranslateUi()
{
    m_visibleItem->setText(tr("Visible"));
    m_titleItem->setText(tr("Title"));
}

void SAQwtPlotItemSetWidget::upDateData(bool downLoad)
{
    if(downLoad)
    {
        m_visibleItem->setCheckState(m_itemPtr->isVisible() ? Qt::Checked : Qt::Unchecked);
        m_titleItem->setEditText(m_itemPtr->title().text());
    }
    else
    {
        m_itemPtr->setVisible(m_visibleItem->checkState() == Qt::Checked);
        m_itemPtr->setTitle(m_titleItem->getEditText());
    }
}

SAQwtPlotItemSetWidget *SAQwtPlotItemSetWidget::createQwtPlotItemSetWidget(QwtPlotItem *plotItem, QWidget *parent)
{
    if(QwtPlotCurve* cur = dynamic_cast<QwtPlotCurve*>(plotItem))
    {
        return new SAQwtPlotCurveItemSetWidget(cur,parent);
    }
    else if(QwtPlotBarChart* bar = dynamic_cast<QwtPlotBarChart*>(plotItem))
    {
        return new SAQwtPlotBarChartItemSetWidget(bar,parent);
    }
    return nullptr;
}


void SAQwtPlotItemSetWidget::addWidget(QWidget *w, int stretch, Qt::Alignment alignment)
{
    m_layout->addWidget(w,stretch,alignment);
}

void SAQwtPlotItemSetWidget::addLayout(QLayout *layout, int stretch)
{
    m_layout->addLayout(layout,stretch);
}
