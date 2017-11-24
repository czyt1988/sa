#include "SAAbstractPlotEditor.h"
#include <QEvent>
#include <QMouseEvent>
#include <QKeyEvent>

SAAbstractPlotEditor::SAAbstractPlotEditor(QwtPlot *parent)
    :QObject(parent)
    ,m_isEnable(false)
    ,m_isSpaceLongPressed(false)
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
        //空格长按下，鼠标事件不处理
            case QEvent::MouseButtonPress:
            {
                const QMouseEvent* mouseEvent =
                        dynamic_cast<QMouseEvent* >( event );
                if(mouseEvent && !m_isSpaceLongPressed)
                {
                    return mousePressEvent( mouseEvent);
                }
                else
                {
                    return false;
                }
                break;
            }
            case QEvent::MouseMove:
            {
                const QMouseEvent* mouseEvent =
                        dynamic_cast< QMouseEvent* >( event );
                if(mouseEvent && !m_isSpaceLongPressed)
                {
                    return mouseMovedEvent( mouseEvent );
                }
                else
                {
                    return false;
                }
                break;
            }
            case QEvent::MouseButtonRelease:
            {
                const QMouseEvent* mouseEvent =
                        dynamic_cast<QMouseEvent* >( event );
                if(mouseEvent && !m_isSpaceLongPressed)
                {
                    return mouseReleasedEvent( mouseEvent );
                }
                else
                {
                    return false;
                }
                break;
            }
            case QEvent::KeyPress:
            {
                const QKeyEvent* keyEvent =
                    dynamic_cast<QKeyEvent* >( event );
                if(keyEvent)
                {
                    if(keyEvent->isAutoRepeat() && Qt::Key_Space == keyEvent->key())
                    {
                        m_isSpaceLongPressed = true;
                        return false;//空格长按键屏蔽
                    }
                    else
                    {
                        m_isSpaceLongPressed = false;
                        return keyPressEvent(keyEvent);
                    }
                }
                break;
            }
            case QEvent::KeyRelease:
            {
                const QKeyEvent* keyEvent =
                    dynamic_cast<QKeyEvent* >( event );
                if(keyEvent)
                {
                    m_isSpaceLongPressed = false;
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
