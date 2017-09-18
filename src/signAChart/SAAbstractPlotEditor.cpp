#include "SAAbstractPlotEditor.h"

SAAbstractPlotEditor::SAAbstractPlotEditor(QwtPlot *parent)
    :QObject(parent)
{

}

SAAbstractPlotEditor::~SAAbstractPlotEditor()
{

}

const QwtPlot *SAAbstractPlotEditor::plot() const
{
    return qobject_cast<const QwtPlot *>( parent() );
}

QwtPlot *SAAbstractPlotEditor::plot()
{
    return qobject_cast<QwtPlot *>( parent() );
}
