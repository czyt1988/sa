#include "SAQwtPlotItemVGroupBox.h"
#include <QApplication>
#include <QVBoxLayout>
#include "qwt_plot_curve.h"
#include "qwt_plot_item.h"
#include "SALineEditPropertyItem.h"
#include "SADoubleSpinBoxPropertyItem.h"
#include "SACheckBoxPropertyItem.h"
#include "SAAxisSelectComboBoxPropertyItem.h"
#include "SAChartAxisSetSelect.h"
#include "SAPenSetWidget.h"
#include <QCheckBox>
#include "SAQwtSymbolComboBox.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#define TR(str) QApplication::translate("SAQwtPlotItemVGroupBoxPropertyItem", str, 0)
class QwtPlotCurve;
class SAQwtPlotItemVGroupBox::UI
{
public:
    QwtPlotItem* itemPtr;
    SACheckBoxPropertyItem* visibleItem;
    SALineEditPropertyItem* titleItem;
    SADoubleSpinBoxPropertyItem* zValueItem;
    SAChartAxisSetSelect* axisSetItem;
    QLabel* labelAxisSet;
    QHBoxLayout* hAxisSetlayout;
    void setupUI(QwtPlotItem* plotItem,SAQwtPlotItemVGroupBox* par)
    {
        labelAxisSet = nullptr;
        QLayout* vlayout = par->layout();
        //visible
        visibleItem = new SACheckBoxPropertyItem(par);
        visibleItem->setObjectName("visible");
        par->connect(visibleItem,&SACheckBoxPropertyItem::stateChanged
                     ,par,[plotItem](int state){
            plotItem->setVisible(state==Qt::Checked);
        });
        vlayout->addWidget(visibleItem);
        //title
        titleItem = new SALineEditPropertyItem(par);
        titleItem->setObjectName("title");
        par->connect(titleItem,&SALineEditPropertyItem::textChanged
                     ,par,[par,plotItem](const QString& text){
            plotItem->setTitle(text);
            par->setTitle(text);
        });
        vlayout->addWidget(titleItem);
/* 这里有其他内容设置，暂时不开启
        if(SAQwtPlotItemVGroupBox::ShowAll == showMode)
        {
            //
            zValueItem = new SADoubleSpinBoxPropertyItem(par);
            zValueItem->setObjectName("z");
            par->connect(zValueItem,&SADoubleSpinBoxPropertyItem::valueChanged
                         ,par,[plotItem](double z){
                plotItem->setZ(z);
            });
            vlayout->addWidget(zValueItem);

            //
            labelAxisSet = new QLabel(par);
            labelAxisSet->setAlignment(Qt::AlignTop|Qt::AlignLeft);
            hAxisSetlayout = new QHBoxLayout();
            axisSetItem = new SAChartAxisSetSelect(par);
            axisSetItem->setObjectName("axisSetItem");
            hAxisSetlayout->addWidget(labelAxisSet);
            hAxisSetlayout->addWidget(axisSetItem);
            hAxisSetlayout->addStretch();
            par->connect(axisSetItem,&SAChartAxisSetSelect::axisChanged
                         ,par,[plotItem](int x,int y){
                plotItem->setAxes(x,y);
            });
            vlayout->addItem(hAxisSetlayout);
        }
*/
        //
        upDateItem(plotItem);
        retranslateUi(par);
    }
    void retranslateUi(SAQwtPlotItemVGroupBox *w)
    {
        w->setTitle(titleItem->getEditText());
        visibleItem->setText(TR("Visible"));
        titleItem->setText(TR("Title"));
//        if(SAQwtPlotItemVGroupBox::ShowAll == showMode)
//        {
//            zValueItem->setText(TR("Z Value"));
//            labelAxisSet->setText(TR("Axis Set"));
//        }
    }

    void upDateItem(const QwtPlotItem* plotItem)
    {
        this->itemPtr = const_cast<QwtPlotItem*>(plotItem);
        visibleItem->setCheckState(plotItem->isVisible() ? Qt::Checked : Qt::Unchecked);
        titleItem->setEditText(plotItem->title().text());
//        if(SAQwtPlotItemVGroupBox::ShowAll == showMode)
//        {
//            zValueItem->setValue(plotItem->z());
//            axisSetItem->setCurrentSelectAxis(plotItem->xAxis(),plotItem->yAxis());
//        }
    }
};



SAQwtPlotItemVGroupBox::SAQwtPlotItemVGroupBox(QwtPlotItem *plotItem, QWidget* par)
    :SAVCollapsibleGroupBox(par)
    ,ui(new SAQwtPlotItemVGroupBox::UI)
{
    ui->setupUI(plotItem,this);
}

SAQwtPlotItemVGroupBox::~SAQwtPlotItemVGroupBox()
{
    delete ui;
}

QwtPlotItem *SAQwtPlotItemVGroupBox::getPlotItem() const
{
    return ui->itemPtr;
}

SAQwtPlotItemVGroupBox *SAQwtPlotItemVGroupBox::createQwtPlotItemVGroupBox(QwtPlotItem *plotItem, QWidget *par)
{
    switch(plotItem->rtti())
    {
    case QwtPlotItem::Rtti_PlotCurve:
        return new SAQwtPlotCurveItemVGroupBox(static_cast<QwtPlotCurve *>(plotItem),par);
    default:
        return new SAQwtPlotItemVGroupBox(plotItem,par);
    }
    return nullptr;
}


//------------------------------------


SAQwtPlotCurveItemVGroupBox::SAQwtPlotCurveItemVGroupBox(QwtPlotCurve *plotItem, QWidget *par)
    :SAQwtPlotItemVGroupBox(plotItem,par)
    ,m_curveItem(plotItem)
    ,m_showAll(false)
{
    m_isSPlineCheckBox = nullptr;
    //set pen -> setPen
    m_PenStyle = new SAPenSetWidget();
    addWidget(m_PenStyle);
    //
    QHBoxLayout* hlayoutSymbolComboBox = new QHBoxLayout;
    m_labelSymbolSet = new QLabel;
    m_symbolComboBox = new SAQwtSymbolComboBox();
    m_symbolSetButton = new QPushButton;
    m_symbolSetButton->setMinimumWidth(15);
    m_symbolSetButton->setMaximumWidth(15);
    m_symbolSetButton->setText(">");
    hlayoutSymbolComboBox->addWidget(m_labelSymbolSet);
    hlayoutSymbolComboBox->addWidget(m_symbolComboBox);
    hlayoutSymbolComboBox->addWidget(m_symbolSetButton);
    addLayout(hlayoutSymbolComboBox);
    //update value
    updateData();
    //connect
    connect(m_PenStyle,&SAPenSetWidget::penChanged
            ,this,&SAQwtPlotCurveItemVGroupBox::onPenChenged);
    connect(m_symbolComboBox,&SAQwtSymbolComboBox::symbolSelectChanged
            ,this,&SAQwtPlotCurveItemVGroupBox::onSymbolComboBoxChanged);
    connect(m_symbolSetButton,&QPushButton::clicked
            ,this,&SAQwtPlotCurveItemVGroupBox::onBaseLineSpinBoxValueChanged);

}

void SAQwtPlotCurveItemVGroupBox::showAll(bool enable)
{
    //fitter curve -> setCurveAttribute
    m_isSPlineCheckBox = new QCheckBox();
    m_isSPlineCheckBox->setToolTip(tr(""));
    addWidget(m_isSPlineCheckBox);
    //base Line
    m_baseLineSpinBox = new SADoubleSpinBoxPropertyItem();
    addWidget(m_baseLineSpinBox);
    //update value
    updateData();
    //connect
    connect(m_isSPlineCheckBox,&QCheckBox::stateChanged
            ,this,&SAQwtPlotCurveItemVGroupBox::onSPlineCheckBoxStateChanged);
    connect(m_baseLineSpinBox,&SADoubleSpinBoxPropertyItem::valueChanged
            ,this,&SAQwtPlotCurveItemVGroupBox::onSymbolSetButtonClicked);
}

void SAQwtPlotCurveItemVGroupBox::retranslateUi()
{
    m_labelSymbolSet->setText(tr("Symbol Style"));
    if(m_showAll)
    {
        m_isSPlineCheckBox->setText(tr("fitter curve"));
        m_baseLineSpinBox->setText(tr("base line"));
    }
}

void SAQwtPlotCurveItemVGroupBox::updateData()
{
    m_PenStyle->setPen(m_curveItem->pen());
    const QwtSymbol* s = m_curveItem->symbol();
    if(s)
    {
        m_symbolComboBox->setStyle(s->style());
    }
    else
    {
        m_symbolComboBox->setStyle(QwtSymbol::NoSymbol);
    }

    if(m_showAll)
    {
        m_isSPlineCheckBox->setChecked(m_curveItem->testCurveAttribute(QwtPlotCurve::Fitted));
        m_baseLineSpinBox->setValue(m_curveItem->baseline());
    }
}

void SAQwtPlotCurveItemVGroupBox::onPenChenged(const QPen &pen)
{
    m_curveItem->setPen(pen);
}

void SAQwtPlotCurveItemVGroupBox::onSPlineCheckBoxStateChanged(int state)
{
    m_curveItem->setCurveAttribute(Qt::Checked == state
                                   ? QwtPlotCurve::Fitted
                                   : QwtPlotCurve::Inverted);
}

void SAQwtPlotCurveItemVGroupBox::onBaseLineSpinBoxValueChanged(double v)
{
    m_curveItem->setBaseline(v);
}

void SAQwtPlotCurveItemVGroupBox::onSymbolComboBoxChanged(QwtSymbol::Style style)
{
    m_symbolSetButton->setEnabled(QwtSymbol::NoSymbol != style);
    m_curveItem->setSymbol(new QwtSymbol(style));
}

void SAQwtPlotCurveItemVGroupBox::onSymbolSetButtonClicked()
{

}
