#include "SAQwtPlotBarChartItemSetWidget.h"
#include "SASpinBoxPropertyItem.h"
#include "SAComboBoxPropertyItem.h"
#include "qwt_column_symbol.h"
#include "SAColorSetPropertyItem.h"
#include <QApplication>
#define TR(str) \
    QApplication::translate("SAQwtPlotBarChartItemSetWidget", str, Q_NULLPTR)

void operator <<(QwtColumnSymbol& newSymbol,const QwtColumnSymbol& old);
void operator <<(QwtColumnSymbol &newSymbol, const QwtColumnSymbol &old)
{
    newSymbol.setFrameStyle(old.frameStyle());
    newSymbol.setLineWidth(old.lineWidth());
    newSymbol.setPalette(old.palette());
    newSymbol.setStyle(old.style());
}


class SAQwtPlotBarChartItemSetWidgetPrivate
{
    SA_IMPL_PUBLIC(SAQwtPlotBarChartItemSetWidget)
public:
    QwtPlotBarChart *barChart;
    SASpinBoxPropertyItem* spacingItem;
    SASpinBoxPropertyItem* marginItem;
    SAComboBoxPropertyItem* frameStyleItem;
    SAComboBoxPropertyItem* symbolStyleItem;
    SASpinBoxPropertyItem* lineWidthItem;
    SAColorSetPropertyItem* barPaintColor;
    SAQwtPlotBarChartItemSetWidgetPrivate(QwtPlotBarChart *bar,SAQwtPlotBarChartItemSetWidget* par)
        :barChart(bar)
        ,q_ptr(par)
    {
        spacingItem = new SASpinBoxPropertyItem(par);
        spacingItem->setMinMax(0,999);
        marginItem = new SASpinBoxPropertyItem(par);
        marginItem->setMinMax(0,999);
        symbolStyleItem = new SAComboBoxPropertyItem(par);
        frameStyleItem = new SAComboBoxPropertyItem(par);
        lineWidthItem = new SASpinBoxPropertyItem(par);
        lineWidthItem->setMinMax(0,999);
        barPaintColor = new SAColorSetPropertyItem(par);

        par->addWidget(spacingItem);
        par->addWidget(marginItem);
        par->addWidget(symbolStyleItem);
        par->addWidget(frameStyleItem);
        par->addWidget(lineWidthItem);
        par->addWidget(barPaintColor);

        retranslateUI();
        spacingItem->setValue(bar->spacing());
        marginItem->setValue(bar->margin());
        const QwtColumnSymbol* symbol = bar->symbol();
        if(symbol)
        {
            barPaintColor->setCurrentColor(symbol->palette().window().color());
            lineWidthItem->setValue(symbol->lineWidth());
            switch(symbol->style())
            {
            case QwtColumnSymbol::NoStyle:symbolStyleItem->setCurrentIndex(0);break;
            case QwtColumnSymbol::Box:symbolStyleItem->setCurrentIndex(1);break;
            default:symbolStyleItem->setCurrentIndex(0);break;
            }
        }

        connect();
    }

    void connect()
    {
        q_ptr->connect(spacingItem,&SASpinBoxPropertyItem::valueChanged
                     ,q_ptr,&SAQwtPlotBarChartItemSetWidget::onSpacingValueChanged);
        q_ptr->connect(marginItem,&SASpinBoxPropertyItem::valueChanged
                     ,q_ptr,&SAQwtPlotBarChartItemSetWidget::onMarginValueChanged);
        q_ptr->connect(symbolStyleItem,static_cast<void(SAComboBoxPropertyItem::*)(int)>(&SAComboBoxPropertyItem::currentIndexChanged)
                     ,q_ptr,&SAQwtPlotBarChartItemSetWidget::onSymbolStyleComboBoxIndexChanged);
        q_ptr->connect(frameStyleItem,static_cast<void(SAComboBoxPropertyItem::*)(int)>(&SAComboBoxPropertyItem::currentIndexChanged)
                     ,q_ptr,&SAQwtPlotBarChartItemSetWidget::onFrameStyleComboBoxIndexChanged);
        q_ptr->connect(lineWidthItem,&SASpinBoxPropertyItem::valueChanged
                     ,q_ptr,&SAQwtPlotBarChartItemSetWidget::onLineWidthValueChanged);
        q_ptr->connect(barPaintColor,&SAColorSetPropertyItem::colorChanged
                     ,q_ptr,&SAQwtPlotBarChartItemSetWidget::onBarPaintColorChanged);
    }

    void retranslateUI()
    {
        spacingItem->setText(TR("spacing"));
        marginItem->setText(TR("margin"));
        symbolStyleItem->clearAllItem();
        symbolStyleItem->setText(TR("Symbol Style"));
        symbolStyleItem->addItem(TR("No Style"),QwtColumnSymbol::NoStyle);
        symbolStyleItem->addItem(TR("Box"),QwtColumnSymbol::Box);
        symbolStyleItem->setToolTip(TR("No Style: the symbol draws nothing\n"
                                       "Box Style:The column is painted with a frame depending on the Frame Style and line Width")
                                    );

        frameStyleItem->clearAllItem();
        frameStyleItem->setText(TR("Frame Style"));
        frameStyleItem->addItem(TR("No Frame"),QwtColumnSymbol::NoFrame);
        frameStyleItem->addItem(TR("Plain"),QwtColumnSymbol::Plain);
        frameStyleItem->addItem(TR("Raised"),QwtColumnSymbol::Raised);
        lineWidthItem->setText(TR("Line Width"));
        barPaintColor->setText(TR("Color"));
    }
};


SAQwtPlotBarChartItemSetWidget::SAQwtPlotBarChartItemSetWidget(QwtPlotBarChart *bar, QWidget *par)
    :SAQwtPlotItemSetWidget(bar,par)
    ,d_ptr(new SAQwtPlotBarChartItemSetWidgetPrivate(bar,this))
{

}

SAQwtPlotBarChartItemSetWidget::~SAQwtPlotBarChartItemSetWidget()
{

}

void SAQwtPlotBarChartItemSetWidget::onSpacingValueChanged(int v)
{
    if(d_ptr->barChart)
    {
        d_ptr->barChart->setSpacing(v);
    }
}

void SAQwtPlotBarChartItemSetWidget::onMarginValueChanged(int v)
{
    if(d_ptr->barChart)
    {
        d_ptr->barChart->setMargin(v);
    }
}

void SAQwtPlotBarChartItemSetWidget::onFrameStyleComboBoxIndexChanged(int v)
{
    if(nullptr == d_ptr->barChart)
    {
        return;
    }
    const QwtColumnSymbol* old = d_ptr->barChart->symbol();
    QwtColumnSymbol* newSymbol = new QwtColumnSymbol();
    *newSymbol << *old;

    switch(v)
    {
    case QwtColumnSymbol::NoFrame:
        newSymbol->setFrameStyle(QwtColumnSymbol::NoFrame);break;
    case QwtColumnSymbol::Plain:
        newSymbol->setFrameStyle(QwtColumnSymbol::Plain);break;
    case QwtColumnSymbol::Raised:
        newSymbol->setFrameStyle(QwtColumnSymbol::Raised);break;
    default:
        newSymbol->setFrameStyle(QwtColumnSymbol::Plain);
    }
    d_ptr->barChart->setSymbol(newSymbol);
}

void SAQwtPlotBarChartItemSetWidget::onSymbolStyleComboBoxIndexChanged(int v)
{
    if(nullptr == d_ptr->barChart)
    {
        return;
    }
    const QwtColumnSymbol* old = d_ptr->barChart->symbol();
    QwtColumnSymbol* newSymbol = new QwtColumnSymbol();
    *newSymbol << *old;
    switch(v)
    {
    case QwtColumnSymbol::NoStyle:
        newSymbol->setStyle(QwtColumnSymbol::NoStyle);break;
    case QwtColumnSymbol::Box:
        newSymbol->setStyle(QwtColumnSymbol::Box);break;
    default:
        newSymbol->setStyle(QwtColumnSymbol::Box);
    }
    if(QwtColumnSymbol::Box == newSymbol->style())
    {
        int fs = d_ptr->frameStyleItem->currentData().toInt();
        switch(fs)
        {
        case QwtColumnSymbol::NoFrame:
            newSymbol->setFrameStyle(QwtColumnSymbol::NoFrame);break;
        case QwtColumnSymbol::Plain:
            newSymbol->setFrameStyle(QwtColumnSymbol::Plain);break;
        case QwtColumnSymbol::Raised:
            newSymbol->setFrameStyle(QwtColumnSymbol::Raised);break;
        default:
            newSymbol->setFrameStyle(QwtColumnSymbol::Plain);
        }
        int lw = d_ptr->lineWidthItem->getValue();
        newSymbol->setLineWidth(lw);
        QColor clr = d_ptr->barPaintColor->getCurrentColor();
        QPalette p = newSymbol->palette();
        p.setColor(QPalette::Window,clr);
        newSymbol->setPalette(p);
    }
    d_ptr->barChart->setSymbol(newSymbol);
}

void SAQwtPlotBarChartItemSetWidget::onLineWidthValueChanged(int v)
{
    if(nullptr == d_ptr->barChart)
    {
        return;
    }
    const QwtColumnSymbol* old = d_ptr->barChart->symbol();
    QwtColumnSymbol* newSymbol = new QwtColumnSymbol();
    *newSymbol << *old;
    newSymbol->setLineWidth(v);
}

void SAQwtPlotBarChartItemSetWidget::onBarPaintColorChanged(const QColor &v)
{
    if(nullptr == d_ptr->barChart)
    {
        return;
    }
    const QwtColumnSymbol* old = d_ptr->barChart->symbol();
    QwtColumnSymbol* newSymbol = new QwtColumnSymbol();
    *newSymbol << *old;
    QPalette p = newSymbol->palette();
    p.setColor(QPalette::Window,v);
    newSymbol->setPalette(p);
    d_ptr->barChart->setSymbol(newSymbol);
}
