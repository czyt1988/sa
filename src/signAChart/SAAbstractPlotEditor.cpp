#include "SAAbstractPlotEditor.h"

SAAbstractPlotEditor::SAAbstractPlotEditor(QwtPlot *parent)
    :QObject(parent)
    ,m_isEnable(false)
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

void SAAbstractPlotEditor::setEnabled(bool on)
{
    if ( on == m_isEnable )
        return;

    QwtPlot *plot = qobject_cast<QwtPlot *>( parent() );
    if ( plot )
    {
        m_isEnable = on;

        if(plot->canvas())
        {
            if ( on )
            {
                plot->canvas()->installEventFilter( this );
            }
            else
            {
                plot->canvas()->removeEventFilter( this );
            }
        }
    }
}

bool SAAbstractPlotEditor::isEnabled() const
{
    return m_isEnable;
}
