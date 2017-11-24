#include "SAAbstractPlotEditor.h"
#include <QEvent>
#include <QMouseEvent>
#include <QKeyEvent>
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

bool SAAbstractPlotEditor::eventFilter(QObject *object, QEvent *event)
{
    QwtPlot *plot = qobject_cast<QwtPlot *>( parent() );
    if ( plot && (object == plot->canvas()) )
    {
        switch( event->type() )
        {
            case QEvent::MouseButtonPress:
            {
                const QMouseEvent* mouseEvent =
                        dynamic_cast<QMouseEvent* >( event );
                if(mouseEvent)
                {
                    return mousePressEvent( mouseEvent);
                }
                break;
            }
            case QEvent::MouseMove:
            {
                const QMouseEvent* mouseEvent =
                        dynamic_cast< QMouseEvent* >( event );
                if(mouseEvent)
                {
                    return mouseMovedEvent( mouseEvent );
                }
                break;
            }
            case QEvent::MouseButtonRelease:
            {
                const QMouseEvent* mouseEvent =
                        dynamic_cast<QMouseEvent* >( event );
                if(mouseEvent)
                {
                    return mouseReleasedEvent( mouseEvent );
                }
                break;
            }
            case QEvent::KeyPress:
            {
                const QKeyEvent* keyEvent =
                    dynamic_cast<QKeyEvent* >( event );
                if(keyEvent)
                {
                    return keyPressEvent(keyEvent);
                }
                break;
            }
            case QEvent::KeyRelease:
            {
                const QKeyEvent* keyEvent =
                    dynamic_cast<QKeyEvent* >( event );
                if(keyEvent)
                {
                    return keyReleaseEvent(keyEvent);
                }
                break;
            }
            default:
                break;
        }
        return false;
    }
    return QObject::eventFilter( object, event );
}

bool SAAbstractPlotEditor::mousePressEvent(const QMouseEvent *e)
{
    Q_UNUSED(e);
    return false;
}

bool SAAbstractPlotEditor::mouseMovedEvent(const QMouseEvent *e)
{
    Q_UNUSED(e);
    return false;
}

bool SAAbstractPlotEditor::mouseReleasedEvent(const QMouseEvent *e)
{
    Q_UNUSED(e);
    return false;
}

bool SAAbstractPlotEditor::keyPressEvent(const QKeyEvent *e)
{
    Q_UNUSED(e);
    return false;
}

bool SAAbstractPlotEditor::keyReleaseEvent(const QKeyEvent *e)
{
    Q_UNUSED(e);
    return false;
}
