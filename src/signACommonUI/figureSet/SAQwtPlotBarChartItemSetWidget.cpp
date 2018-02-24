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
#if SAQwtPlotBarChartItemSetWidget_Show_SymbolStyle
    SAComboBoxPropertyItem* symbolStyleItem;
#endif
    SASpinBoxPropertyItem* lineWidthItem;
    SAColorSetPropertyItem* barPaintColor;
    SAColorSetPropertyItem* barFrameColor;
    SAQwtPlotBarChartItemSetWidgetPrivate(QwtPlotBarChart *bar,SAQwtPlotBarChartItemSetWidget* par)
        :q_ptr(par)
        ,barChart(bar)
    {
        spacingItem = new SASpinBoxPropertyItem(par);
        spacingItem->setMinMax(0,999);
        marginItem = new SASpinBoxPropertyItem(par);
        marginItem->setMinMax(0,999);
#if SAQwtPlotBarChartItemSetWidget_Show_SymbolStyle
        symbolStyleItem = new SAComboBoxPropertyItem(par);
#endif
        frameStyleItem = new SAComboBoxPropertyItem(par);
        lineWidthItem = new SASpinBoxPropertyItem(par);
        lineWidthItem->setMinMax(0,999);
        barPaintColor = new SAColorSetPropertyItem(par);
        barFrameColor = new SAColorSetPropertyItem(par);

        par->addWidget(spacingItem);
        par->addWidget(marginItem);
#if SAQwtPlotBarChartItemSetWidget_Show_SymbolStyle
        par->addWidget(symbolStyleItem);
#endif
        par->addWidget(frameStyleItem);
        par->addWidget(barPaintColor);
        par->addWidget(lineWidthItem);
        par->addWidget(barFrameColor);

        retranslateUI();
        updateData(true);
        connect();
    }

    void connect()
    {
        q_ptr->connect(spacingItem,&SASpinBoxPropertyItem::valueChanged
                     ,q_ptr,&SAQwtPlotBarChartItemSetWidget::onSpacingValueChanged);
        q_ptr->connect(marginItem,&SASpinBoxPropertyItem::valueChanged
                     ,q_ptr,&SAQwtPlotBarChartItemSetWidget::onMarginValueChanged);
#if SAQwtPlotBarChartItemSetWidget_Show_SymbolStyle
        q_ptr->connect(symbolStyleItem,static_cast<void(SAComboBoxPropertyItem::*)(int)>(&SAComboBoxPropertyItem::currentIndexChanged)
                     ,q_ptr,&SAQwtPlotBarChartItemSetWidget::onSymbolStyleComboBoxIndexChanged);
#endif
        q_ptr->connect(frameStyleItem,static_cast<void(SAComboBoxPropertyItem::*)(int)>(&SAComboBoxPropertyItem::currentIndexChanged)
                     ,q_ptr,&SAQwtPlotBarChartItemSetWidget::onFrameStyleComboBoxIndexChanged);
        q_ptr->connect(lineWidthItem,&SASpinBoxPropertyItem::valueChanged
                     ,q_ptr,&SAQwtPlotBarChartItemSetWidget::onLineWidthValueChanged);
        q_ptr->connect(barPaintColor,&SAColorSetPropertyItem::colorChanged
                     ,q_ptr,&SAQwtPlotBarChartItemSetWidget::onBarPaintColorChanged);
        q_ptr->connect(barFrameColor,&SAColorSetPropertyItem::colorChanged
                     ,q_ptr,&SAQwtPlotBarChartItemSetWidget::onBarFrameColorChanged);
    }

    void retranslateUI()
    {
        spacingItem->setText(TR("spacing"));
        marginItem->setText(TR("margin"));
#if SAQwtPlotBarChartItemSetWidget_Show_SymbolStyle
        symbolStyleItem->clearAllItem();
        symbolStyleItem->setText(TR("Symbol Style"));
        symbolStyleItem->addItem(TR("No Style"),QwtColumnSymbol::NoStyle);
        symbolStyleItem->addItem(TR("Box"),QwtColumnSymbol::Box);
        symbolStyleItem->setToolTip(TR("No Style: the symbol draws nothing\n"
                                       "Box Style:The column is painted with a frame depending on the Frame Style and line Width")
                                    );
#endif
        frameStyleItem->clearAllItem();
        frameStyleItem->setText(TR("Frame Style"));
        frameStyleItem->addItem(TR("No Frame"),QwtColumnSymbol::NoFrame);
        frameStyleItem->addItem(TR("Plain"),QwtColumnSymbol::Plain);
        frameStyleItem->addItem(TR("Raised"),QwtColumnSymbol::Raised);
        lineWidthItem->setText(TR("Line Width"));
        barPaintColor->setText(TR("Plain Color"));
        barFrameColor->setText(TR("Frame Color"));
    }

    void updateData(bool downLoad)
    {
        if(nullptr == this->barChart)
        {
            spacingItem->setValue(0);
            marginItem->setValue(0);
            frameStyleItem->setEnabled(false);
            lineWidthItem->setEnabled(false);
            barFrameColor->setEnabled(false);
            lineWidthItem->setValue(1);
#if SAQwtPlotBarChartItemSetWidget_Show_SymbolStyle
            symbolStyleItem->setCurrentIndex(0);
#endif
            frameStyleItem->setCurrentIndex(0);
            return;
        }
        if(downLoad)
        {
            spacingItem->setValue(barChart->spacing());
            marginItem->setValue(barChart->margin());
            const QwtColumnSymbol* symbol = barChart->symbol();
            if(symbol)
            {
#if SAQwtPlotBarChartItemSetWidget_Show_SymbolStyle

                switch(symbol->style())
                {
                case QwtColumnSymbol::NoStyle:
                    symbolStyleItem->setCurrentIndex(0);
                    break;
                case QwtColumnSymbol::Box:
                    symbolStyleItem->setCurrentIndex(1);
                    break;
                default:
                    symbolStyleItem->setCurrentIndex(0);
                    break;
                }
#endif
                switch(symbol->frameStyle())
                {
                case QwtColumnSymbol::NoFrame:
                    frameStyleItem->setCurrentIndex(0);
                    lineWidthItem->setVisible(false);
                    barFrameColor->setVisible(false);
                    break;
                case QwtColumnSymbol::Plain:
                    frameStyleItem->setCurrentIndex(1);
                    lineWidthItem->setVisible(true);
                    barFrameColor->setVisible(true);
                    break;
                case QwtColumnSymbol::Raised:
                    frameStyleItem->setCurrentIndex(2);
                    lineWidthItem->setVisible(true);
                    barFrameColor->setVisible(true);
                    break;
                default:
                    frameStyleItem->setCurrentIndex(0);
                    lineWidthItem->setVisible(true);
                    barFrameColor->setVisible(true);
                    break;
                }
                if(QwtColumnSymbol::NoFrame != symbol->frameStyle())
                {
                    QPalette pl = symbol->palette();
                    barPaintColor->setCurrentColor(pl.window().color());
                    qDebug() << "before";
                    qDebug() << "QPalette";
                    QBrush b = pl.dark();
                    qDebug() << "get b:";
                    qDebug() << "b.color()"<<b.color();
                    barFrameColor->setCurrentColor(b.color());
                    lineWidthItem->setValue(symbol->lineWidth());
                }
            }
        }
        else
        {
            q_ptr->onSpacingValueChanged(spacingItem->getValue());
            q_ptr->onMarginValueChanged(marginItem->getValue());
            q_ptr->onFrameStyleComboBoxIndexChanged(frameStyleItem->currentIndex());
#if SAQwtPlotBarChartItemSetWidget_Show_SymbolStyle
            q_ptr->onSymbolStyleComboBoxIndexChanged(symbolStyleItem->currentIndex());
#endif
            q_ptr->onLineWidthValueChanged(lineWidthItem->getValue());
            q_ptr->onBarPaintColorChanged(barPaintColor->getCurrentColor());
            q_ptr->onBarFrameColorChanged(barFrameColor->getCurrentColor());
        }
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

void SAQwtPlotBarChartItemSetWidget::upDateData(bool downLoad)
{
    SAQwtPlotItemSetWidget::upDateData(downLoad);
    d_ptr->updateData(downLoad);
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
        newSymbol->setFrameStyle(QwtColumnSymbol::NoFrame);
        d_ptr->lineWidthItem->setVisible(false);
        d_ptr->barFrameColor->setVisible(false);
        break;
    case QwtColumnSymbol::Plain:
        newSymbol->setFrameStyle(QwtColumnSymbol::Plain);
        d_ptr->lineWidthItem->setVisible(true);
        d_ptr->barFrameColor->setVisible(true);
        break;
    case QwtColumnSymbol::Raised:
        newSymbol->setFrameStyle(QwtColumnSymbol::Raised);
        d_ptr->lineWidthItem->setVisible(true);
        d_ptr->barFrameColor->setVisible(true);
        break;
    default:
        newSymbol->setFrameStyle(QwtColumnSymbol::Plain);
    }
    d_ptr->barChart->setSymbol(newSymbol);
}
#if SAQwtPlotBarChartItemSetWidget_Show_SymbolStyle
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
    d_ptr->frameStyleItem->setEnabled(QwtColumnSymbol::Box == newSymbol->style());
    d_ptr->lineWidthItem->setEnabled(QwtColumnSymbol::Box == newSymbol->style());
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
    d_ptr->frameStyleItem->setEnabled(QwtColumnSymbol::Box == newSymbol->style());
    d_ptr->lineWidthItem->setEnabled(QwtColumnSymbol::Box == newSymbol->style());
    d_ptr->barFrameColor->setEnabled(QwtColumnSymbol::Box == newSymbol->style());
    d_ptr->barChart->setSymbol(newSymbol);
}
#endif
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
    d_ptr->barChart->setSymbol(newSymbol);
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

void SAQwtPlotBarChartItemSetWidget::onBarFrameColorChanged(const QColor &v)
{
    if(nullptr == d_ptr->barChart)
    {
        return;
    }
    const QwtColumnSymbol* old = d_ptr->barChart->symbol();
    QwtColumnSymbol* newSymbol = new QwtColumnSymbol();
    *newSymbol << *old;
    QPalette p = newSymbol->palette();
    p.setColor(QPalette::Dark,v);
    newSymbol->setPalette(p);
    d_ptr->barChart->setSymbol(newSymbol);
}
