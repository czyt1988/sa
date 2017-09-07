#include "SARectSelectEditor.h"
#include <QEvent>
#include <QMouseEvent>
SARectSelectEditor::SARectSelectEditor(QwtPlot *parent):QObject(parent)
{

}

SARectSelectEditor::~SARectSelectEditor()
{

}

const QwtPlot *SARectSelectEditor::plot() const
{
    return qobject_cast<const QwtPlot *>( parent() );
}

QwtPlot *SARectSelectEditor::plot()
{
    return qobject_cast<QwtPlot *>( parent() );
}
//TODO
--
bool SARectSelectEditor::eventFilter(QObject *object, QEvent *event)
{
    QwtPlot *plot = qobject_cast<QwtPlot *>( parent() );
    if ( plot && object == plot->canvas() )
    {
        switch( event->type() )
        {
            case QEvent::MouseButtonPress:
            {
                const QMouseEvent* mouseEvent =
                                dynamic_cast<QMouseEvent* >( event );


            }
        }
    }
}

bool SARectSelectEditor::pressed(const QPoint &p)
{

}

bool SARectSelectEditor::moved(const QPoint &p)
{

}

void SARectSelectEditor::released(const QPoint &p)
{

}

SARectSelectEditor::SelectionMode SARectSelectEditor::selectionMode() const
{
    return m_selectionMode;
}

void SARectSelectEditor::setSelectionMode(const SelectionMode &selectionMode)
{
    m_selectionMode = selectionMode;
}
